#pragma once
#include "mutexClock.h"

#include <vector>

using namespace std;

namespace wd
{

class LRUCache;

class CacheManager
{
    class AutoRelease
    {
    public:
        AutoRelease();
        ~AutoRelease();
    };
public:
    vector<LRUCache> _cacheVec;

    LRUCache& getCache(size_t thID);
    bool isUpdating();

    void updateAllCache();
public:
    static CacheManager* getInstance();
private:
    CacheManager();
    ~CacheManager();
    static CacheManager* _pCacheMana;
    static AutoRelease _auto;

    bool _isUpdating;
    MutexLock _mutexLock;
};

}
