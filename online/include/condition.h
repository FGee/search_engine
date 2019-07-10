#ifndef __CONDITON_H__
#define __CONDITON_H__
#include "noncopyable.h"

#include <stdio.h>
#include <errno.h>
#include <pthread.h>

namespace wd
{

class MutexLock;

class Condition
: protected Noncopyable
{
public:
    Condition(MutexLock & mutex);

    void wait();

    void notify();

    void notifyAll();

    ~Condition();
private:
    pthread_cond_t _cond;
    MutexLock& _mutex;
};
}
#endif
