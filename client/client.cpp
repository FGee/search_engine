#include "client.h"
#include "macro.h"
#include "jsonFormat.h"

#include <vector>
#include <iostream>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/epoll.h>

#include <unistd.h>

namespace wd
{

typedef struct {
    size_t ctl_code;
    size_t len;
    char data[4096];
} Train_t, *pTrain_t;

Client::Client(size_t port, const string ip = string())
    : _sockFd(createSockFd()), _ip(ip), _port(port), _isStart(false),
    _epFd(createEpollFd())
{
    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(struct sockaddr_in));
    serAddr.sin_family = AF_INET;
    if (ip == string()) {
        serAddr.sin_addr.s_addr = INADDR_ANY;
    } else {
        serAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    }
    serAddr.sin_port = htons(port);

    int ret = ::connect(_sockFd, (struct sockaddr*)&serAddr, sizeof(struct sockaddr_in));
    if (-1 == ret) { ERROR_EXIT("connect"); }
}

int Client::createSockFd()
{
    int sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd == -1) { ERROR_EXIT("socket"); }
    return sockFd;
}

void Client::startQueryLoop(JsonFormat& refJson)
{
    _isStart = true;

    addEventRead(_sockFd);
    addEventRead(STDIN_FILENO);

    vector<struct epoll_event> eventsVec(5);

    while (_isStart) {
        int readyNum = epoll_wait(_epFd, &*eventsVec.begin(), 5, 0);
        if (-1 == readyNum && errno == EINTR) {
            continue;
        } else if (-1 == readyNum) {
            ERROR_EXIT("epoll_wait");
        } else {
            for (int i = 0; i < readyNum; ++i) {
                if (eventsVec[i].data.fd == _sockFd && (eventsVec[i].events & EPOLLIN)) {
                    Train_t train;
                    memset(&train, 0, sizeof(Train_t));
                    recvN((char*)&train, sizeof(Train_t));
                    cout << train.data << endl;
                }
                if (eventsVec[i].data.fd == STDIN_FILENO && eventsVec[i].events & EPOLLIN) {
                    char buf[1024] = { 0 };
                    int ret = read(STDIN_FILENO, buf, sizeof(buf) - 1);
                    if (ret == -1) { ERROR_EXIT("read"); }
                    buf[strlen(buf) - 1] = '\0'; //消除换行符
                    string query = refJson.dataToJson(buf);
                    Train_t train;
                    memset(&train, 0, sizeof(Train_t));
                    train.ctl_code = 1; // 查询操作控制码设置为1
                    train.len = query.size();
                    strcpy(train.data, query.c_str());

                    sendN((char*)&train, sizeof(Train_t));
                }
            }
        }
    }
    
}

int Client::createEpollFd()
{
    int tmp = epoll_create(1);
    if (-1 == tmp) { ERROR_EXIT("epoll_create"); }
    return tmp;
}

void Client::addEventRead(int newfd)
{
    struct epoll_event tmp;
    memset(&tmp, 0, sizeof(struct epoll_event));
    tmp.events = EPOLLIN;
    tmp.data.fd = newfd;

    int ret = epoll_ctl(_epFd, EPOLL_CTL_ADD, newfd, &tmp);
    if (ret == -1) { ERROR_EXIT("epoll_ctl"); }
}

void Client::deleteEventRead(int fd)
{
    struct epoll_event tmp;
    memset(&tmp, 0, sizeof(struct epoll_event));
    tmp.data.fd = fd;

    int ret = epoll_ctl(_epFd, EPOLL_CTL_DEL, fd, &tmp);
    if (ret == -1) { ERROR_EXIT("epoll_ctl"); }
}

void Client::recvN(char* buf, size_t len) 
{
    size_t total = 0;
    while (total < len) {
        int ret = read(_sockFd, buf + total, len - total);
        if (-1 == ret && errno == EINTR) { continue; }
        if (-1 == ret) { ERROR_EXIT("read"); }
        total += ret;
    }
}

void Client::sendN(const char* buf, size_t len) 
{
    size_t total = 0;
    while (total < len) {
        int ret = write(_sockFd, buf + total, len - total);
        if (-1 == ret && errno == EINTR) { continue; }
        if (-1 == ret) { ERROR_EXIT("read"); }
        total += ret;
    }
}

}
