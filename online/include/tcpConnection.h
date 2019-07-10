#pragma once
#include "noncopyable.h"
#include "inetAddress.h"
#include "socket.h"
#include "socketIO.h"

#include <functional>
#include <memory>

using namespace std;

namespace wd
{

typedef struct {
    size_t ctl_code;
    size_t len;
    char data[4096];
} Train_t, *pTrain_t;

class EventLoop;
class TcpConnection;
class SpellCorrectServer;

using CallBackFunc = function<void(const shared_ptr<TcpConnection>)>;

class TcpConnection
: Noncopyable
, public enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(int, EventLoop*);
    string recv();
    string recvTrain();
    void send(const string&);
    void sendTrain(const string& data);
    string toString();
    ~TcpConnection();

    void setOnConnectCallBack(CallBackFunc);
    void setOnMessageCallBack(CallBackFunc);
    void setOnCloseCallBack(CallBackFunc);

    void doOnConnectCallBack();
    void doOnMessageCallBack();
    void doOnCloseCallBack();

    void sendInLoop(const string&);
private:
    InetAddress getLocalInetAddr(int);
    InetAddress getPeerInetAddr(int);
private:
    Socket _acceptFd;
    SocketIO _sockIO;
    InetAddress _localInetAddr;
    InetAddress _peerInetAddr;
    bool _isShutdownWrite;

    CallBackFunc _onConnect;
    CallBackFunc _onMessage;
    CallBackFunc _onClose;

    EventLoop* _pEventLoop;
};

}
