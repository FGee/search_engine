#include "../include/splitTool/cppJiebaSplitTool.h"
#include "../include/dictoryary.h"
#include "../include/configuration.h"
#include "../include/macro.h"

#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <sstream>

namespace wd
{

Dictionary* Dictionary::_pDict = nullptr;
//Dictionary* Dictionary::_pDict = Dictionary::getInstance();

Dictionary::AutoRelease Dictionary::_auto;

Dictionary::AutoRelease::AutoRelease() { cout << "AutoRelease()" << endl; }

Dictionary::AutoRelease::~AutoRelease() 
{
    cout << "*****Dictionary*****" << endl;
    cout << "process pid : " << getpid() << endl;
    cout << "_pDict : " << _pDict << endl;
    if (nullptr != _pDict) {
        delete _pDict;
    }
    cout << "Dictory: ~AutoRelease()" << endl;
}

Dictionary::Dictionary()
{
    auto configMap = Configuration::getInstance()->_configMap;
    string offsetLibPath = configMap["offset_lib"];
    string webpageLibPath = configMap["webpage_lib"];
    string invertIndexLibPath = configMap["invert_index_lib"];

    loadOffsetLib(offsetLibPath);
    loadWebpageLib(webpageLibPath);
    loadInvertIndexLib(invertIndexLibPath);
    
    cout << "Dictionary()" << endl;
}

void Dictionary::loadOffsetLib(const string& pathName)
{
    ifstream ifs;
    ifs.open(pathName, ios::in);
    if (ifs.fail()) { ERROR_EXIT("offset lib open error"); }

    string line;
    while (getline(ifs, line)) {
        istringstream iss(line);

        string docidStr;
        iss >> docidStr;
        long docid = atol(docidStr.c_str());

        string posStr;
        iss >> posStr;
        long pos = atol(posStr.c_str());

        string lenStr;
        iss >> lenStr;
        long len = atol(lenStr.c_str());

        _offsetLib.insert(make_pair(docid, make_pair(pos, len)));
    }
    ifs.close();
}

void Dictionary::loadWebpageLib(const string& pathName)
{
    ifstream ifs;
    ifs.open(pathName, ios::in);
    if (ifs.fail()) { ERROR_EXIT("webpage lib open error"); }

    auto pCppjiba = CppJiebaSplitTool::getInstanceCppjieba();
    for (auto & elem : _offsetLib) {
        WebPage tmpWebpage;

        // 下面这种方式读到的位置，即为上回读取的结束位置
        // 所以不能直接通过文件自偏移的方式，只能seek
        //cout << "pos : " << ifs.tellg() << endl;
        ifs.seekg(elem.second.first);

        char buff[65535] = { 0 };
        ifs.read(buff, elem.second.second);

        tmpWebpage.webPageInit(buff, pCppjiba);

        _webpageLib.push_back(tmpWebpage);
    }
    ifs.close();
}

void Dictionary::loadInvertIndexLib(const string& pathName)
{
    ifstream ifs;
    ifs.open(pathName, ios::in);
    if (ifs.fail()) { ERROR_EXIT("invert index lib open error"); }
    
    string line;
    while (getline(ifs, line)) {
        istringstream iss(line);
        string word;
        iss >> word;

        vector<pair<long, double>> docidWght;
        long docid;
        double weight;
        while (iss >> docid >> weight) {
            docidWght.push_back(make_pair(docid, weight));
        }
        _invertIndexLib.insert(make_pair(word, docidWght));
    }
    ifs.close();
}

Dictionary::~Dictionary() { cout << "~Dictionary()" << endl; }

}
