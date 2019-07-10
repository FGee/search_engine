#include "../include/thread.h"
#include "../include/myLog.h"

#include <cstdlib>

__thread const char* threadName = "wd";

namespace wd
{

struct ThreadDate
{
    ThreadDate(CallBackFunc_t cb, const string& name)
        : _cb(cb), _name(name) {}
    void runInThreadDate()
    {
        threadName = (_name == string()) ? "wd thread" : _name.c_str();

        if (_cb) {
            _cb();
        }
    }
private:
    CallBackFunc_t _cb;
    string _name;
};

void Thread::start()
{
    if (!_isRunning) {
        ThreadDate* pThreadDate = new ThreadDate(_cb, _name);

        int ret = pthread_create(&_thId, nullptr, threadFunc, pThreadDate);
        // 不需要传this指针了，这时由于回调函数是位于Thread对象的外部，不是对象的成员
        if (ret) {
            perror("pthread_create");
        }
        _isRunning = true;
    }
}

void Thread::join()
{
    if (_isRunning) {
        pthread_join(_thId, nullptr);
    }
}

void* Thread::threadFunc(void* args)
{
    ThreadDate* pThreadDate = static_cast<ThreadDate*>(args);
    if (pThreadDate) {
        pThreadDate->runInThreadDate();
        delete pThreadDate;
    }
    return nullptr;
}

Thread::~Thread()
{
    if (_isRunning) {
        pthread_detach(_thId);
    }
    LogDebug("~Thread()");
}

size_t Thread::getThID() const 
{
    return atol(_name.c_str());
}

}
