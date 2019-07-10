#include "../include/inetAddress.h"

#include <string.h>

namespace wd
{

InetAddress::InetAddress(const unsigned short& port)
{
    ::memset(&_addr, 0, sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = INADDR_ANY;
    _addr.sin_port = htons(port);
}

InetAddress::InetAddress(const string& ip, const unsigned short& port)
: _addr()
{
    ::memset(&_addr, 0, sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
    _addr.sin_port = htons(port);
}

InetAddress::InetAddress(const struct sockaddr_in& addr)
: _addr(addr) {}

string InetAddress::ip() const
{
    return ::inet_ntoa(_addr.sin_addr);
}

unsigned short InetAddress::port() const
{
    return ::ntohs(_addr.sin_port);
}

struct sockaddr_in* InetAddress::sockaddrInPtrGet()
{
    return &_addr;
}

}
