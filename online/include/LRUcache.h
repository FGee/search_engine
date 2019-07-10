#pragma once

#include <list>
#include <unordered_map>

using namespace std;

namespace wd
{

class LRUCache
{
    friend class CacheManager;
public:
    struct CacheNode {
        CacheNode(const string& q, const string& j)
            : _queryWrd(q), _jsonResult(j) {}
        string _queryWrd;
        string _jsonResult;
    };
public:
    LRUCache();
    void loadFromCacheFile();
    void saveCache();
    
    string searchInCache(const string&);
    void addIntoCache(const string&, const string&);
private:
    size_t _cacheCapacity;
    list<CacheNode> _cacheList;
    unordered_map<string, list<CacheNode>::iterator> _hashMap;
};

}
