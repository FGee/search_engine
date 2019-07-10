#include "../include/macro.h"
#include "../include/configuration.h"
#include "../include/myLog.h"
#include "../include/spellCorrectServer.h"

#include "../include/dictoryary.h"
#include "../include/cacheManager.h"

#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <iostream>
#include <memory>
#include <functional>
#include <sstream>

using namespace wd;
using namespace std;

int exitPiFds[2];

void signalHandler(int sigNum)
{
    write(exitPiFds[1], &sigNum, sizeof(int));
}

int main()
{
    int ret = pipe(exitPiFds);
    if (ret == -1) { return 0; }
    if (fork()) {
        close(exitPiFds[0]);
        signal(SIGUSR1, signalHandler);
        
        int status;
        wait(&status);
        cout << endl;
        if (WIFEXITED(status)) {
            cout << ">> child process exited is normanl" << endl;
        } else {
            cout << ">> child process exited is error" << endl;
        }
        cout << endl;
            
        exit(0);
    }
    close(exitPiFds[1]);

    Configuration::getInstance()->loadConfigFile("../conf/myconf.conf");

    auto& configMap = Configuration::getInstance()->getConfigMap();

    stringstream ss;
    ss << exitPiFds[0];
    configMap["exitPiFds[0]"] = ss.str(); // 通过单例配置对象传递，同步退出机制的管道

    LogInfo("Spell correct server start");

    string ip = configMap["ip"];
    size_t port = atol(configMap["port"].c_str());
    size_t taskQueSz = atol(configMap["TaskQueueSize"].c_str());
    size_t threadNum = atol(configMap["ThreadNum"].c_str());
    string threadName = configMap["ThreadName"];
    
    size_t initTime = atol(configMap["timerfd_init_time"].c_str());
    size_t internalTime = atol(configMap["timerfd_internal_time"].c_str());

    Dictionary::getInstance();

    shared_ptr<SpellCorrectServer> _spSpellCorrectServer(new
                                            SpellCorrectServer(
                                            ip, port, taskQueSz, 
                                            threadNum, threadName,
                                            initTime, internalTime));

    _spSpellCorrectServer->spellCorrectServerStart();

    return 0;
}

