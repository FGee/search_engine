#pragma once
#include "taskQueue.h"
#include "thread.h"
#include "tcpConnection.h"

#include <vector>
#include <memory>
#include <functional>
#include <string>

using namespace std;

namespace wd
{
using TaskCallBackFunc = function<void()>;
class ThreadPool
{
    friend class Thread;
public:
    ThreadPool(size_t qs, size_t ts, const string& thName)
        : _taskQue(qs), _queSize(qs),
        _threadNum(ts), _isExit(false),
        _threadName(thName)
    { _threads.reserve(_threadNum); }
    void start();
    void stop();
    void addTask(TaskCallBackFunc&&);
private:
    TaskCallBackFunc getTask();
    void threadFunc();
private:
    TaskQueue _taskQue;
    size_t _queSize;
    vector<unique_ptr<Thread>> _threads;
    size_t _threadNum;
    bool _isExit;

    string _threadName;
};

}
