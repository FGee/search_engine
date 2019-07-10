#pragma once
#include "tcpServer.h"
#include "threadPool.h"
#include "timerfdThread.h"

using namespace wd;

namespace wd
{

class SpellCorrectServer
{
public:
    SpellCorrectServer(const string& ip, size_t port, size_t taskQueSz,
                       size_t threadNum, const string& threadName,
                       size_t initTime, size_t internalTime);
    void spellCorrectServerStart();
    void doOnConnect(const SPtrConn);
    void doOnMessage(const SPtrConn);
    void doOnClose(const SPtrConn);
    void stopSpellCorrServer();
    ~SpellCorrectServer();
private:
    TcpServer _tcpServer;
    ThreadPool _threadPool;
    TimerfdThread _timerfdThread;
};

}
