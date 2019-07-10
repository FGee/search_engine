#pragma once
#include "mutexClock.h"
#include "condition.h"

#include "tcpConnection.h"

#include <vector>
#include <queue>
#include <functional>

using namespace std;

namespace wd
{

class Task;
using ElemType = function<void()>;

class TaskQueue
{
public:
    TaskQueue(size_t qs)
        : _queueSize(qs), _queue(),
        _mutex(), _notEmpty(_mutex),
        _notFull(_mutex), _exitFlag(false)
    {}
    bool empty() const;
    bool full() const;
    void push(const ElemType&);
    ElemType pop();
public:
    void weak();
private:
    size_t _queueSize;
    queue<ElemType> _queue;
    MutexLock _mutex;
    Condition _notEmpty;
    Condition _notFull;
private:
    bool _exitFlag;
};

}
