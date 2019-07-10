#pragma once

namespace wd
{

class Socket
{
public:
    Socket();
    Socket(const int f);
    ~Socket();
    void shutDownWrite();
    int fdGet() const;
private:
    int _fd;
};

}
