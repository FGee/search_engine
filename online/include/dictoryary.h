#pragma once
#include "webPage.h"

#include <vector>
#include <unordered_map>
#include <set>

using namespace std;

namespace wd
{

class Dictionary
{
    class AutoRelease
    {
    public:
        AutoRelease();
        ~AutoRelease();
    };
public:
    map<long, pair<long, long>> _offsetLib;
    vector<WebPage> _webpageLib;
    unordered_map<string, vector<pair<long, double>>> _invertIndexLib;
private:
    void loadOffsetLib(const string&);
    void loadWebpageLib(const string&);
    void loadInvertIndexLib(const string&);
public:
    static Dictionary* getInstance()
    {
        if (nullptr == _pDict) {
            _pDict = new Dictionary();
        }
        return _pDict;
    }
private:
    Dictionary();
    ~Dictionary();

    static Dictionary* _pDict;
    static AutoRelease _auto;
};

}
