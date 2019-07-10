#include "../include/cacheManager.h"
#include "../include/configuration.h"
#include "../include/LRUcache.h"

#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <cstdlib>

using namespace std;

namespace wd
{

CacheManager::CacheManager() 
: _isUpdating(false), _mutexLock()
{
    auto configMap = Configuration::getInstance()->getConfigMap();
    size_t cacheNum = atol(configMap["ThreadNum"].c_str());
    vector<LRUCache>(cacheNum).swap(_cacheVec);

    _cacheVec[0].loadFromCacheFile();
    
    updateAllCache();

    cout << "CacheManager()" << endl;
}

void CacheManager::updateAllCache()
{
    {
        MutexLockGuard mutexGuard(_mutexLock);
        _isUpdating = true;
    }
    
    auto configMap = Configuration::getInstance()->getConfigMap();
    int cacheNum = atol(configMap["ThreadNum"].c_str());
    
    for (int i = 1; i < cacheNum; ++i) {
        for (auto & pair : _cacheVec[i]._cacheList) {
            _cacheVec[0].addIntoCache(pair._queryWrd, 
                                      pair._jsonResult);
        }
    }

    for (int i = 0; i < cacheNum; ++i) {
        if (0 == i) { continue; }
        for (auto cacheNode : _cacheVec[0]._cacheList) {
            _cacheVec[i].addIntoCache(cacheNode._queryWrd, cacheNode._jsonResult);
        }
    }

    _cacheVec[0].saveCache();

    {
        MutexLockGuard mutexGuard(_mutexLock);
        _isUpdating = false;
    }
}

LRUCache& CacheManager::getCache(size_t thID)
{
    return _cacheVec[thID];
}

bool CacheManager::isUpdating() 
{
    {
        MutexLockGuard mutexGuard(_mutexLock);
        return _isUpdating; 
    }
}

CacheManager* CacheManager::_pCacheMana = nullptr;
//CacheManager* CacheManager::_pCacheMana = CacheManager::getInstance();
CacheManager::AutoRelease CacheManager::_auto;

CacheManager::AutoRelease::AutoRelease() { cout << "AutoRelease()" << endl; }

CacheManager::AutoRelease::~AutoRelease() 
{
    cout << "*****CacheManager*****" << endl;
    cout << "process pid : " << getpid() << endl;
    cout << "_pCacheMana : " << _pCacheMana << endl;
    if (nullptr != _pCacheMana) {
        delete _pCacheMana;
    }
    cout << "CacheManager: ~AutoRelease()" << endl; 
}

CacheManager* CacheManager::getInstance()
{
    if (nullptr == _pCacheMana) {
        _pCacheMana = new CacheManager();
    }
    return _pCacheMana;
}

CacheManager::~CacheManager()
{
    cout << "~CacheManager()" << endl;
}

}
