#ifndef __MUTEX_LOCK_H__
#define __MUTEX_LOCK_H__
#include "noncopyable.h"

#include <stdio.h>
#include <error.h>
#include <pthread.h>

#include <iostream>

using std::cout;
using std::endl;

namespace wd
{

class MutexLock
: protected Noncopyable
{
public:
    MutexLock();
    ~MutexLock();
    void lock();
    void unlock();
    bool isLocking() const;
    pthread_mutex_t * getMutexLockPtr();
private:
    pthread_mutex_t _mutex;
    bool _isLocking;
};

class MutexLockGuard
{
public:
    MutexLockGuard(MutexLock &mutex); 
    ~MutexLockGuard(); 
private:
    MutexLock& _mutex;
};

}

#endif

