#pragma once
#include "noncopyable.h"
#include "timer.h"
#include "thread.h"

namespace wd
{

class TimerfdThread
: Noncopyable
{
public:
    TimerfdThread(int, int, CallBackFunc_t&&);
    void start();
    void stop();
private:
    Timer _timer;
    Thread _thread;
};

}
