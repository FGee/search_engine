#include "../include/timer.h"

#include <sys/timerfd.h>
#include <string.h>
#include <poll.h>
#include <unistd.h>

#include <iostream>

using namespace std;

namespace wd
{

Timer::Timer(int initTime, int internalTime, TimeCallBack&& cb)
    : _tFd(createTimerFd()), 
    _initTime(initTime),
    _internalTime(internalTime),
    _tCb(move(cb)), _isLooping(false) 
    {}

void Timer::setTimer(int initTime, int internalTime)
{
    _initTime = initTime;
    _internalTime = internalTime;

    struct itimerspec tmpItimerspec;
    memset(&tmpItimerspec, 0, sizeof(tmpItimerspec));
    tmpItimerspec.it_value.tv_sec = _initTime;
    tmpItimerspec.it_interval.tv_sec = _internalTime;

    int ret = ::timerfd_settime(_tFd, 0, &tmpItimerspec, nullptr);
    if (-1 == ret) { perror("timerfd_settime"); exit(EXIT_FAILURE); }
}

int Timer::createTimerFd()
{
    int tfd = ::timerfd_create(CLOCK_REALTIME, 0);
    if (-1 == tfd) {
        perror("timerfd_create");
        exit(EXIT_FAILURE);
    }
    return tfd;
}

void Timer::start()
{
    setTimer(_initTime, _internalTime); // 设置初始化时间，间隔时间，并开始

    struct pollfd struPoll;
    memset(&struPoll, 0, sizeof(struct pollfd));
    struPoll.fd = _tFd;
    struPoll.events = POLLIN;

    _isLooping = true;
    while (_isLooping) {
        int readyNum = ::poll(&struPoll, 1, 5000);

        if (-1 == readyNum && errno == EINTR) {
            continue;
        } else if (-1 == readyNum) {
            perror("poll"); exit(EXIT_FAILURE);
        } else if (0 == readyNum) {
            //cout << "** time out !" << endl;
        } else {
            if ((struPoll.fd == _tFd) 
                && (struPoll.events & POLLIN)) {
                handleTimefdRead();
                if (_tCb) {
                    _tCb();
                } 
            }
        }
    }
}

void Timer::handleTimefdRead()
{
    uint64_t timeoverCnt = 0;
    int ret = read(_tFd, &timeoverCnt, sizeof(uint64_t));

    if (sizeof(uint64_t) != ret) {
        perror("read"); exit(EXIT_FAILURE);
    }

    //cout << "** timeoverCnt: " << timeoverCnt << endl;
}

void Timer::stop()
{
    _isLooping = false;
}

}
