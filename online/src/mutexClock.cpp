#include "../include/mutexClock.h"

namespace wd
{

MutexLock::MutexLock() : _isLocking(false)
{
    if (pthread_mutex_init(&_mutex, NULL)) {
        perror("pthread_mutex_init");
    }
}

MutexLock::~MutexLock()
{
    if (pthread_mutex_destroy(&_mutex)) {
        perror("pthread_mutex_destroy");
    }
}

void MutexLock::lock()
{
    if (pthread_mutex_lock(&_mutex)) {
        perror("pthread_mutex_lock");
    }
    _isLocking = true;
}

void MutexLock::unlock()
{
    if (pthread_mutex_unlock(&_mutex)) {
        perror("pthread_mutex_unlock");
    }
    _isLocking = false;
}

inline
bool MutexLock::isLocking() const { return _isLocking; }

//inline //为何加上内联后，编译报错，未定义？？？？
pthread_mutex_t * MutexLock::getMutexLockPtr() { return &_mutex; }

MutexLockGuard::MutexLockGuard(MutexLock &mutex) : _mutex(mutex) 
{
    _mutex.lock();
}

MutexLockGuard::~MutexLockGuard() 
{
    _mutex.unlock();
}

}
