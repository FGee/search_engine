#pragma once
#include "noncopyable.h"

#include <map>
#include <fstream>

using namespace std;

namespace wd
{

class WebpageLibProducer
: Noncopyable
{
public:
    WebpageLibProducer();
    ~WebpageLibProducer();
    void producePageLib();
private:
    void processSingle(ofstream& ofs, const char* pathName);
    string deleteHtmlMark(string);
    void saveOffsetLib();

    map<long, pair<long, long>> _offsetMap;
    long _docID;
};

}
