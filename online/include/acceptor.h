#pragma once
#include "noncopyable.h"
#include "inetAddress.h"
#include "socket.h"

namespace wd
{

class Acceptor
: protected Noncopyable
{
public:
    Acceptor(const unsigned short&);
    Acceptor(const string&, const unsigned short&);
    void ready();
    int accept();
    int listenFdGet() const;
private:
    void setReuseAddr(bool);
    void setReusePort(bool);
    void bind();
    void listen();
private:
    InetAddress _inetAddr;
    Socket _listenFd;
};

}
