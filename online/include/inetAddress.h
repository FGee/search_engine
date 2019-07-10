#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>

using namespace std;

namespace wd
{

class InetAddress
{
public:
    InetAddress(const unsigned short&);
    InetAddress(const string&, const unsigned short&);
    InetAddress(const struct sockaddr_in&);
    string ip() const;
    unsigned short port() const;
    struct sockaddr_in* sockaddrInPtrGet();
private:
    struct sockaddr_in _addr;
};

}
