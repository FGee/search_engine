#pragma once
#include "noncopyable.h"
#include "webPage.h"

#include <vector>
#include <unordered_map>
#include <set>

using namespace std;

namespace wd
{

class InvertIndexAndRmDup
: Noncopyable
{
public:
    InvertIndexAndRmDup();
    ~InvertIndexAndRmDup();

    void loadLib(const string&, const string&);
    
    void rmDupWebpage();

    void productInvertIndex();
   
    void saveLib();
private:
    void loadOffsetLib(const string&);
    void loadWebpageLib(const string&);
    
    bool webpageIfSim(unsigned long);
    int hammingDistance(unsigned long, unsigned long);
    void pushTheSimhash(unsigned long);
    string printHash(unsigned long);
    
    void saveNewWebpageLib();
    void saveNewOffsetLib();

    void saveInvertIndex();

    vector<WebPage> _webPageLibVec;
    map<long, pair<long, long>> _offSetLib;

    vector<WebPage> _webPageLibVecNew;
    map<long, pair<long, long>> _offSetLibNew;

    unordered_map<uint16_t, set<unsigned long>> _partSimHashMap;

    unordered_map<string, vector<pair<long, double>>> _invertIndex;
};

}
