#pragma once

namespace wd
{

class SocketIO
{
public:
    SocketIO(int);
    int readN(char* buf, int len);
    int writeN(const char* buf, int len);
    int readLine(char* buf, int maxLen);
private:
    int recvPeek(char* buf, int Len);
    int _acceptFd;
};

}
