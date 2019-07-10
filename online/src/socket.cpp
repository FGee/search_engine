#include "../include/socket.h"
#include "../include/macro.h"

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>

namespace wd
{

Socket::Socket()
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == _fd) { ERROR_EXIT("socket"); }
}

Socket::Socket(const int f) : _fd(f) {}

Socket::~Socket()
{
    int ret = ::close(_fd);
    if (ret == -1) {
        ERROR_EXIT("close");
    }
}

void Socket::shutDownWrite()
{
    int ret = shutdown(_fd, SHUT_WR);
    if (ret == -1) {
        ERROR_EXIT("shutdown");
    }
}

int Socket::fdGet() const { return _fd; }

}
