#include "../include/eventLoop.h"
#include "../include/macro.h"
#include "../include/configuration.h"

#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/eventfd.h>

#include <iostream>
#include <cstdlib>

using namespace std;

namespace wd
{

EventLoop::EventLoop(Acceptor& a, StopServerCb&& ssCb) 
    : _epFd(epollCreate()), _acceptor(a),
    _events(1024), _isLooping(false),
    _evFd(createEventFd()), _ssCb(move(ssCb)) {}

int EventLoop::epollCreate()
{
    int tmp = epoll_create(1);
    if (-1 == tmp) { ERROR_EXIT("epoll_create"); }
    return tmp;
}

void EventLoop::loop()
{
    _isLooping = true;

    auto configMap = Configuration::getInstance()->getConfigMap();

    _exitPiFd = atoi(configMap["exitPiFds[0]"].c_str());

    addEventRead(_exitPiFd);
    addEventRead(_acceptor.listenFdGet()); // 先注册连接接收器的listenFd
    addEventRead(_evFd);

    while (_isLooping) {
        epollWait();
    }
}

void EventLoop::unloop()
{
    if (_isLooping) {
        _isLooping = false;
    }
}

void EventLoop::epollWait()
{
    int readyNum;
    do {
        readyNum = epoll_wait(_epFd, &*_events.begin(), _events.size(), 5000);
    } while (readyNum == -1 && errno == EINTR);
    if (-1 == readyNum) {
        ERROR_EXIT("epoll_wait");
    } else if (0 == readyNum) {
        cout << "*****Time out*****" << endl;
    } else {
        if (readyNum == static_cast<int>(_events.size())) { _events.resize(readyNum * 2); }
        for (int i = 0; i < readyNum; ++i) {
            int fd = _events[i].data.fd;
            
            if (fd == _acceptor.listenFdGet()) {
                if (_events[i].events & EPOLLIN) {
                    handleNewConnect();
                }
            } else if (fd == _evFd) {
                uint64_t number;
                int ret = read(_evFd, &number, sizeof(number));
                if (sizeof(uint64_t) != ret) { ERROR_EXIT("read"); }

                doPendingFunc();
            } else if (fd == _exitPiFd) {
                _ssCb();
            } else {
                if (_events[i].events & EPOLLIN) {
                    handleMessage(fd);
                }
            }
        }
    }
}

void EventLoop::handleNewConnect()
{
    int newfd = _acceptor.accept();
    auto newConn = make_pair(newfd, make_shared<TcpConnection>(newfd, this));
    _conns.insert(newConn);
    addEventRead(newfd);
    
    auto spNewConn = _conns[newfd];

    spNewConn->setOnConnectCallBack(_onConnect);
    spNewConn->setOnMessageCallBack(_onMessage);
    spNewConn->setOnCloseCallBack(_onClose);

    _conns[newfd]->doOnConnectCallBack();
}

void EventLoop::addEventRead(int newfd)
{
    struct epoll_event tmp;
    memset(&tmp, 0, sizeof(struct epoll_event));
    tmp.events = EPOLLIN;
    tmp.data.fd = newfd;

    int ret = epoll_ctl(_epFd, EPOLL_CTL_ADD, newfd, &tmp);
    if (ret == -1) { ERROR_EXIT("epoll_ctl"); }
}

void EventLoop::handleMessage(int fd)
{
    bool closed = isConnectionClosed(fd);
    auto retIter = _conns.find(fd);
    assert(_conns.end() != retIter);
    
    if (true == closed) {
        handleConnectClose(fd);
    } else {
        auto spConn = _conns[fd];
        spConn->doOnMessageCallBack();
    }
}

bool EventLoop::isConnectionClosed(int fd)
{
    int ret;
    do {
        char buf[1024];
        ret = recv(fd, buf, sizeof(buf), MSG_PEEK);
    } while (-1 == ret && errno == EINTR);
    return 0 == ret;
}

void EventLoop::handleConnectClose(int fdClosed)
{
    _conns[fdClosed]->doOnCloseCallBack();
// 这里一定要注意取消epoll和删除conn的先后顺序，删除其conn的时候，会调用Socket对象中的析构函数
// 里的close（fdClosed）方法；在这之后进行epoll_ctl(EPOLL_CTL_DEL) 会返回-1并将errno置位
    //cout << ">> fd : " << fdClosed << endl;
    deleteEventRead(fdClosed); // 取消对fdClosed的监控

    auto retIter = _conns.find(fdClosed);
    _conns.erase(retIter); // 从map中删除这个TcpConnection的对象
}

void EventLoop::deleteEventRead(int fd)
{
    struct epoll_event tmp;
    memset(&tmp, 0, sizeof(struct epoll_event));
    tmp.data.fd = fd;

    int ret = epoll_ctl(_epFd, EPOLL_CTL_DEL, fd, &tmp);
    if (ret == -1) { ERROR_EXIT("epoll_ctl"); }
}

void EventLoop::setOnConnect(CallBackFunc&& cb) 
{ _onConnect = move(cb); }

void EventLoop::setOnMessage(CallBackFunc&& cb)
{ _onMessage = move(cb); }

void EventLoop::setOnClose(CallBackFunc&& cb)
{ _onClose = move(cb); }

int EventLoop::createEventFd()
{
    int evFd = eventfd(0, 0); // 如果为0的话，则第一次不会去执行
    if (-1 == evFd) { ERROR_EXIT("eventfd"); }
    return evFd;
}

void EventLoop::doInLoop(SendMsgCallBack&& sndCb)
{
    {
        MutexLockGuard lockGuard(_mutexLock);
        _sendCbList.push_back(move(sndCb));
    }
    weak();
}

void EventLoop::doPendingFunc()
{
    //{
    //    MutexLockGuard lockGuard(_mutexLock);
    //    for (auto & SendCb : _sendCbList) {
    //        SendCb();
    //    }
    //    vector<SendMsgCallBack>().swap(_sendCbList); // 采用这种方式清空容器的元素，效率极高
    //    不单只是capcity和size均变为0
    //    利用了临时对象的清空机制
    //}
    vector<SendMsgCallBack> tmp;
    {
        MutexLockGuard lockGuard(_mutexLock);
        tmp.swap(_sendCbList);
    }
    for (auto & sendCbFunc : tmp) {
        sendCbFunc();
    }
}

void EventLoop::weak()
{
    uint64_t number = 666;
    int ret = write(_evFd, &number, sizeof(number));
    if (sizeof(uint64_t) != ret) { ERROR_EXIT("write"); }
}

}
