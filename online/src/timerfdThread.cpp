#include "../include/timerfdThread.h"

namespace wd
{

TimerfdThread::TimerfdThread(int initTime, int internalTime, CallBackFunc_t&& cb)
: _timer(initTime, internalTime, move(cb)), 
    _thread(bind(&Timer::start, &_timer), "Timerfd Thread") {}

void TimerfdThread::start()
{
    _thread.start();
}

void TimerfdThread::stop()
{
    _timer.stop();
    _thread.join();
}

}
