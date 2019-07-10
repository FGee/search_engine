#include "../include/tcpConnection.h"
#include "../include/macro.h"
#include "../include/eventLoop.h"

#include <string.h>

#include <sstream>

namespace wd
{

TcpConnection::TcpConnection(int acceptFd, EventLoop* p)
    : _acceptFd(acceptFd), _sockIO(acceptFd),
    _localInetAddr(getLocalInetAddr(acceptFd)),
    _peerInetAddr(getPeerInetAddr(acceptFd)),
    _isShutdownWrite(false), _pEventLoop(p) {}

InetAddress TcpConnection::getLocalInetAddr(int acceptFd)
{
    struct sockaddr_in tmp;
    socklen_t len = sizeof(struct sockaddr_in);
    int ret = getsockname(acceptFd, (struct sockaddr*)&tmp, &len);
    if (-1 == ret) { ERROR_EXIT("getsockname"); }
    return InetAddress(tmp);
}

InetAddress TcpConnection::getPeerInetAddr(int acceptFd)
{
    struct sockaddr_in tmp;
    socklen_t len = sizeof(struct sockaddr_in);
    int ret = getpeername(acceptFd, (struct sockaddr*)&tmp, &len);
    if (-1 == ret) { ERROR_EXIT("getsockname"); }
    return InetAddress(tmp);
}

string TcpConnection::recv()
{
    char buf[65536] = { 0 };
    _sockIO.readLine(buf, sizeof(buf));
    return string(buf);
}

string TcpConnection::recvTrain() {
    Train_t train;
    memset(&train, 0, sizeof(Train_t));
    _sockIO.readN((char*)&train, sizeof(Train_t));

    return string(train.data);
}

void TcpConnection::send(const string& data)
{
    _sockIO.writeN(data.c_str(), data.size());
}

void TcpConnection::sendTrain(const string& data)
{
    Train_t train;
    memset(&train, 0, sizeof(Train_t));
    strcpy(train.data, data.c_str());
    _sockIO.writeN((char*)&train, sizeof(Train_t));
}

string TcpConnection::toString()
{
    ostringstream oss;
    oss << _localInetAddr.ip() << ":" << _localInetAddr.port()
        << "---->" 
        << _peerInetAddr.ip() << ":" << _peerInetAddr.port();
    return oss.str();
}

TcpConnection::~TcpConnection()
{
    if (!_isShutdownWrite) {
        _isShutdownWrite = true;
        _acceptFd.shutDownWrite();
    }
}

void TcpConnection::setOnConnectCallBack(CallBackFunc cb) 
{ _onConnect = move(cb); }

void TcpConnection::setOnMessageCallBack(CallBackFunc cb)
{ _onMessage = move(cb); }

void TcpConnection::setOnCloseCallBack(CallBackFunc cb)
{ _onClose = move(cb); }

void TcpConnection::doOnConnectCallBack() 
{
    if (_onConnect) {
        _onConnect(shared_from_this()); 
    }
}

void TcpConnection::doOnMessageCallBack()
{
    if (_onMessage) {
        _onMessage(shared_from_this());
    }
}

void TcpConnection::doOnCloseCallBack()
{
    if (_onClose) {
        _onClose(shared_from_this());
    }
}

void TcpConnection::sendInLoop(const string& msg)
{
    _pEventLoop->doInLoop(bind(&TcpConnection::sendTrain, this, msg));
}

}
