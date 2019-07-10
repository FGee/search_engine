#include "../inc/webPage.h"
#include "../inc/tinyxml2/tinyxml2.h"
#include "../inc/splitTool/cppJiebaSplitTool.h"

#include <string.h>

#include <cstdlib>
#include <sstream>
#include <functional>

using namespace tinyxml2;

namespace wd
{

string WebPage::toDoc() const
{
    stringstream ss;

    ss << _docid << " " << _simHash;
    string docidStr;
    ss >> docidStr;

    string simHashStr;
    ss >> simHashStr;

    return "<doc><docid>" + docidStr +
        "</docid><url>" + _url +
        "</url><title>" + _title +
        "</title><content>" + _content +
        "</content><simHash>" + simHashStr +
        "</simHash></doc>";
}

void WebPage::webPageInit(const string xmlStr, CppJiebaSplitTool& splitTool)
{
    docToWeb(xmlStr);
    cutWord(splitTool);
    produceSimHash();
}

void WebPage::docToWeb(const string& xmlStr)
{
    XMLDocument doc;
    doc.Parse(xmlStr.c_str());
    
    //XMLElement* pRoot = doc.RootElement();
    //cout << "pRoot : " << pRoot << endl;
    //cout << "GetText : " << pRoot->GetText() << endl;

    XMLElement* pDoc = doc.FirstChildElement("doc");
    //cout << "GetText : " << pDoc->GetText() << endl;

    XMLElement* pItem = pDoc->FirstChildElement();

    while (pItem) {
        if (!strcmp("docid", pItem->Name())) {
            _docid = atol(pItem->GetText());
        } else if (!strcmp("url", pItem->Name())) {
            _url = pItem->GetText();
        } else if (!strcmp("title", pItem->Name())) {
            _title = pItem->GetText();
        } else if (!strcmp("content", pItem->Name())) {
            _content = pItem->GetText();
        } else if (!strcmp("simHash", pItem->Name())) {
            if (pItem->GetText()) {
                _simHash = atol(pItem->GetText());
            } 
        }

        pItem = pItem->NextSiblingElement();
    }
}

void WebPage::cutWord(CppJiebaSplitTool& splitTool)
{
    istringstream iss(_content);

    string line;
    while (getline(iss, line)) {
        vector<string> wordsVec = splitTool.cut(line);

        for (auto & word : wordsVec) {
            auto retIter = _wordFreMap.find(word);
            if (retIter == _wordFreMap.end()) {
                _wordFreMap.insert(make_pair(word, 1));
            } else {
                ++_wordFreMap[word];
            }
        }
    }
}

void WebPage::produceSimHash()
{
    int arr[64] = { 0 };

    //if (_docid > 3) { return; }

    //cout << ">>>>>>>>>>>>> docid : " << _docid << endl;

    for (auto & elem : _wordFreMap) {
        unsigned long wrdSimHash = hash<string>{}(elem.first); // 单词 hash 降维

        //cout << "wrd : " << elem.first << "  freq : " << elem.second << endl;
        //for (int i= 0; i < 64; ++i) {
        //    int bit = (wrdSimHash & (1 << i)) ? 1 : 0;
        //    cout << bit;
        //}
        //cout << endl;

        int freq = elem.second; // 逐位 加权 单词频次 并且 进行合并
        for (int i = 0; i < 64; ++i) {
            int symbol = (wrdSimHash & ((unsigned long)1 << i)) ? 1 : -1; // bit为1，则正；否则为负
            arr[i] += (symbol * freq); 
        }
    }

    for (int i = 0; i < 64; ++i) { // 降维，生成最终结果
        unsigned long bit = (arr[i] >= 1) ? 1 : 0;
        _simHash |= (bit << i);
    }
    
    //for (int i = 0; i < 64; ++i) {
    //    unsigned long bit = (_simHash & ((unsigned long)1 << i)) ? 1 : 0;
    //    cout << bit;
    //}
    //cout << endl;
}

}
