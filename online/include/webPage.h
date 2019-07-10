#pragma once

#include <string>
#include <map>

using namespace std;

namespace wd
{

class CppJiebaSplitTool;

class WebPage
{
public:
    string toDoc() const;
    void webPageInit(const string, CppJiebaSplitTool*);
    void webPageInitNoXML(const string, CppJiebaSplitTool*);
    
    string productSummary(const string&);
    
    unsigned long _simHash;
    long _docid;
    string _url;
    string _title;
    string _content;
    
    map<string, int> _wordFreMap;
    map<string, double> _weightVector;
private:
    void docToWeb(const string&);
    void cutWord(CppJiebaSplitTool*);
    void produceSimHash();
};

}
