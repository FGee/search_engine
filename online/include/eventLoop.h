#pragma once
#include "tcpConnection.h"
#include "acceptor.h"
#include "mutexClock.h"

#include <sys/epoll.h>

#include <map>
#include <vector>
#include <memory>

using namespace std;

namespace wd
{
    using SendMsgCallBack = function<void()>;
    using SPtrConn = shared_ptr<TcpConnection>;
    using StopServerCb = function<void()>;
class EventLoop
{
public:
    EventLoop(Acceptor&, StopServerCb&& ssCb);
    void loop();
    void unloop();

    void setOnConnect(CallBackFunc&&);
    void setOnMessage(CallBackFunc&&);
    void setOnClose(CallBackFunc&&);

    void doInLoop(SendMsgCallBack&&);
private:
    int epollCreate();
    void epollWait();
    
    void handleNewConnect();
    void addEventRead(int);
   
    void handleMessage(int);
    bool isConnectionClosed(int);
    
    void handleConnectClose(int);
    void deleteEventRead(int);

    int createEventFd();
    void weak();
    void doPendingFunc();
private:
    int _epFd;
    Acceptor& _acceptor;
    map<int, SPtrConn> _conns;
    vector<struct epoll_event> _events;
    bool _isLooping;
    
    CallBackFunc _onConnect;
    CallBackFunc _onMessage;
    CallBackFunc _onClose;

    vector<SendMsgCallBack> _sendCbList;
    MutexLock _mutexLock;

    int _evFd;

    StopServerCb _ssCb;
    int _exitPiFd;
};

}
