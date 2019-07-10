#include "../include/spellCorrectServer.h"
#include "../include/workThreadTask.h"
#include "../include/cacheManager.h"
#include "../include/myLog.h"

#include <functional>

using namespace std;

namespace wd
{

SpellCorrectServer::SpellCorrectServer(const string& ip, size_t port,
                                       size_t taskQueSz, size_t threadNum, 
                                       const string& threadName, size_t initTime, 
                                       size_t internalTime)
: _tcpServer(ip, port, 
             bind(&SpellCorrectServer::stopSpellCorrServer, this)), 
    _threadPool(taskQueSz, threadNum, threadName),
    _timerfdThread(initTime, internalTime, 
                 bind(&CacheManager::updateAllCache, 
                      CacheManager::getInstance())) {}

void SpellCorrectServer::spellCorrectServerStart()
{
    _timerfdThread.start();
    
    _threadPool.start();
    
    using namespace placeholders;
    _tcpServer.setTcpOnConnectCallBack(bind(&SpellCorrectServer::doOnConnect, this, _1));
    _tcpServer.setTcpOnMessageCallBack(bind(&SpellCorrectServer::doOnMessage, this, _1));
    _tcpServer.setTcpOnCloseCallBack(bind(&SpellCorrectServer::doOnClose, this, _1));

    _tcpServer.start();
}

void SpellCorrectServer::doOnConnect(const SPtrConn spConn) {
    cout << spConn->toString() << endl;
    LogInfo(spConn->toString().c_str());
    spConn->sendTrain(">> Welcome to server!");
}

void SpellCorrectServer::doOnMessage(const SPtrConn spConn) {
    string recvMsg = spConn->recvTrain();
    cout << threadName << " >> Msg recv: " << recvMsg;

    _threadPool.addTask(bind(&WorkThreadTask::process, WorkThreadTask(spConn, recvMsg)));
}

void SpellCorrectServer::doOnClose(const SPtrConn spConn) {
    cout << ">> " << spConn->toString() 
        << " had closed by peer edge!" << endl;
    LogInfo("%s had closed by peer edge", spConn->toString().c_str());
}

void SpellCorrectServer::stopSpellCorrServer()
{
    _tcpServer.stop();
    _timerfdThread.stop();
    _threadPool.stop();
}

SpellCorrectServer::~SpellCorrectServer()
{
    LogDebug("~SpellCorrectServer()");
}

}
