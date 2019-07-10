#include "../include/acceptor.h"
#include "../include/macro.h"

#include <sys/types.h>
#include <sys/socket.h>

namespace wd
{

Acceptor::Acceptor(const unsigned short& port) : _inetAddr(port) {}

Acceptor::Acceptor(const string& ip, const unsigned short& port)
    : _inetAddr(ip, port) {}

void Acceptor::setReuseAddr(bool on)
{
    int one = on;
    int ret = setsockopt(_listenFd.fdGet(), SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    if (-1 == ret) { ERROR_EXIT("setsockopt"); }
}

void Acceptor::setReusePort(bool on)
{
    int one = on;
    int ret = setsockopt(_listenFd.fdGet(), SOL_SOCKET, SO_REUSEPORT, &one, sizeof(one));
    if (-1 == ret) { ERROR_EXIT("setsockopt"); }
}

void Acceptor::ready()
{
    setReuseAddr(true);
    setReusePort(true);
    bind();
    listen();
}

void Acceptor::bind()
{
    int ret = ::bind(_listenFd.fdGet(), (struct sockaddr*)_inetAddr.sockaddrInPtrGet(), sizeof(struct sockaddr_in));
    if (-1 == ret) { ERROR_EXIT("bind"); }
}

void Acceptor::listen()
{
    int ret = ::listen(_listenFd.fdGet(), LISTEN_SZ);
    if (-1 == ret) { ERROR_EXIT("listen"); }
}

int Acceptor::accept()
{
    int acceptFd = ::accept(_listenFd.fdGet(), nullptr, 0);
    if (-1 == acceptFd) { ERROR_EXIT("accept"); }
    return acceptFd;
}

int Acceptor::listenFdGet() const { return _listenFd.fdGet(); }

}
