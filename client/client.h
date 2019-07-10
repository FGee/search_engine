#pragma once

#include <string>

using namespace std;

namespace wd
{

class JsonFormat;

class Client
{
public:
    Client(size_t port, const string ip);
    void startQueryLoop(JsonFormat& refJson);
private:
    int createSockFd();
    int createEpollFd();
    void addEventRead(int);
    void deleteEventRead(int);

    void recvN(char* buf, size_t len);
    void sendN(const char* buf, size_t len);

    int _sockFd;
    string _ip;
    size_t _port;
    bool _isStart;
    int _epFd;
};

}
