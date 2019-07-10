#pragma once

#include <functional>

using namespace std;

namespace wd
{

class Timer
{
    using TimeCallBack = function<void()>;
public:
    Timer(int initTime, int internalTime, TimeCallBack&& cb);
    void start();
    void stop();
    void setTimer(int, int);
private:
    int createTimerFd();
    void handleTimefdRead();
private:
    int _tFd;
    int _initTime;
    int _internalTime;
    TimeCallBack _tCb;
    bool _isLooping;
};

}
