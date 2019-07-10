#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__

namespace wd
{

class Noncopyable
{
protected:
    Noncopyable() {}
    Noncopyable(const Noncopyable& rhs) = delete;
    Noncopyable& operator=(const Noncopyable& rhs) = delete;
};

}

#endif

