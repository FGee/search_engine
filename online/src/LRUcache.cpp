#include "../include/LRUcache.h"
#include "../include/configuration.h"
#include "../include/macro.h"

#include <cstdlib>

#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

namespace wd
{

LRUCache::LRUCache()
{
    auto configMap = Configuration::getInstance()->getConfigMap();
    _cacheCapacity = atol(configMap["cache_capacity"].c_str());
}

void LRUCache::loadFromCacheFile()
{
    auto configMap = Configuration::getInstance()->getConfigMap();
    string cachePath = configMap["cache_file_path"];

    ifstream ifs;
    ifs.open(cachePath, ios::in);
    if (ifs.fail()) { ERROR_EXIT("open cache load"); }
    
    string line;
    while (getline(ifs, line)) {
        istringstream iss(line);
        string queryWrd;
        iss >> queryWrd;
        string jsonResult;
        iss >> jsonResult;
        addIntoCache(queryWrd, jsonResult);
    }
    ifs.close();
}

void LRUCache::saveCache()
{
    auto configMap = Configuration::getInstance()->getConfigMap();

    string cachePath = configMap["cache_file_path"];
    
    ofstream ofs;
    ofs.open(cachePath, ios::out | ios::trunc);
    if (ofs.fail()) { ERROR_EXIT("open cache store"); }
    
    for (auto & elem : _cacheList) {
        ofs << elem._queryWrd << " " << elem._jsonResult << endl;
        cout << elem._queryWrd << " " << elem._jsonResult << endl;
    }
    ofs.close();
}

void LRUCache::addIntoCache(const string& queryWrd, 
                            const string& jsonResult)
{
    auto retIter = _hashMap.find(queryWrd);
    if (retIter != _hashMap.end()) {
        retIter->second->_jsonResult = jsonResult;
        _cacheList.splice(_cacheList.begin(), _cacheList, retIter->second);
        retIter->second = _cacheList.begin();
        return;
    }
    if (_cacheList.size() == _cacheCapacity) {
        _hashMap.erase(_cacheList.end()->_queryWrd);
        _cacheList.pop_back();
    }
    _cacheList.push_front(CacheNode(queryWrd, jsonResult));
    
    pair<string, list<CacheNode>::iterator> newPair(queryWrd, _cacheList.begin());
    _hashMap.insert(newPair);
}

string LRUCache::searchInCache(const string& queryWrd)
{
    auto retIter = _hashMap.find(queryWrd);
    if (retIter == _hashMap.end()) {
        return string();
    }
    string tmp = retIter->second->_jsonResult;
    _cacheList.splice(_cacheList.begin(), _cacheList, retIter->second);
    _hashMap[retIter->first] = _cacheList.begin();

    return tmp;
}

}
