#include "../inc/invertIndexAndRmDup.h"
#include "../inc/macro.h"
#include "../inc/configuration.h"
#include "../inc/splitTool/cppJiebaSplitTool.h"

#include <cstdlib>
#include <cmath>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

namespace wd
{

InvertIndexAndRmDup::InvertIndexAndRmDup() { cout << "InvertIndexAndRmDup()" << endl; }

InvertIndexAndRmDup::~InvertIndexAndRmDup() { cout << "~InvertIndexAndRmDup()" << endl; }

void InvertIndexAndRmDup::loadLib(const string& offsetLib, const string& webpageLib)
{
    map<long, pair<long, long>>().swap(_offSetLib);
    map<long, pair<long, long>>().swap(_offSetLibNew);
    
    vector<WebPage>().swap(_webPageLibVec);
    vector<WebPage>().swap(_webPageLibVecNew);

    loadOffsetLib(offsetLib);
    loadWebpageLib(webpageLib);
}

void InvertIndexAndRmDup::productInvertIndex()
{
    auto configMap = Configuration::getInstanceConfig()->getConfigMap();
    string newOffsetLib = configMap["new_offset_lib"];
    string newWebpageLib = configMap["new_webpage_lib"];

    loadLib(newOffsetLib, newWebpageLib);

    for (auto & webPage : _webPageLibVec) { // 将文档所有文章的词汇作为索引，索引至docid
        long docID = webPage._docid;
        for (auto & wordItem : webPage._wordFreMap) {
            long termFrequency = wordItem.second;

            auto retPair = _invertIndex.find(wordItem.first);
            if (retPair == _invertIndex.end()) {
                vector<pair<long, double>> docidWeights;
                docidWeights.push_back(make_pair(docID, termFrequency));
                _invertIndex.insert(make_pair(wordItem.first, docidWeights));
                continue;
            }
            retPair->second.push_back(make_pair(docID, termFrequency));
        }
    }

    double webNum = _webPageLibVec.size();
    vector<double> totalWeightVec(webNum, 0); // 总的权重, 为了进行权重的归一化处理

    for (auto & wordItem : _invertIndex) { // 生成对应的逆文档频率
        double docFrequency = wordItem.second.size();

        for (auto & docWrdWght : wordItem.second) {
            double inverseDocFreq = log(webNum / (docFrequency + 1)) / log(2);
            double termFrequency = docWrdWght.second;
            double wght = termFrequency * inverseDocFreq;
            docWrdWght.second = wght;
            totalWeightVec[docWrdWght.first] += (wght * wght);
        }
    }

    for (auto & wordItem : _invertIndex) { // 权重归一化
        for (auto & docWrdWght : wordItem.second) {
            docWrdWght.second /= (sqrt(totalWeightVec[docWrdWght.first]));
        }
    }
    
    saveInvertIndex();
}

void InvertIndexAndRmDup::loadOffsetLib(const string& pathName)
{
    ifstream ifs;
    ifs.open(pathName, ios::in);
    if (ifs.fail()) { ERROR_EXIT("offset lib open error"); }

    string line;
    while (getline(ifs, line)) {
        istringstream iss(line);
        string docid;
        iss >> docid;
        string pos;
        iss >> pos;
        string len;
        iss >> len;

        _offSetLib.insert(make_pair(atol(docid.c_str()), 
                                    make_pair(atol(pos.c_str()), 
                                              atol(len.c_str()))));
    }
}

void InvertIndexAndRmDup::loadWebpageLib(const string& pathName)
{
    ifstream ifs;
    ifs.open(pathName, ios::in);
    if (ifs.fail()) { ERROR_EXIT("webpage lib open error"); }

    CppJiebaSplitTool splitTool;
    for (auto & elem : _offSetLib) {
        WebPage tmpWebpage;

        // 下面这种方式读到的位置，即为上回读取的结束位置
        // 所以不能直接通过文件自偏移的方式，只能seek
        //cout << "pos : " << ifs.tellg() << endl;
        ifs.seekg(elem.second.first);

        char buff[65535] = { 0 };
        ifs.read(buff, elem.second.second);

        tmpWebpage.webPageInit(buff, splitTool);

        _webPageLibVec.push_back(tmpWebpage);
    }
}

bool InvertIndexAndRmDup::webpageIfSim(unsigned long webSimhash)
{
    const uint16_t* pPartSimhash = reinterpret_cast<uint16_t*>(&webSimhash);

    for (int i = 0; i < 4; ++i, ++pPartSimhash) {
        auto retIter = _partSimHashMap.find(*pPartSimhash);

        if (retIter == _partSimHashMap.end()) {
            continue;
        } 
        
        for (auto& elem : retIter->second) {
            if (hammingDistance(elem, webSimhash) <= 3) {
                return true;
            }
        }
    }
    pushTheSimhash(webSimhash);

    return false;
}

int InvertIndexAndRmDup::hammingDistance(unsigned long based, unsigned long cmp)
{
    unsigned long resSimhash = based ^ cmp;
    
    //cout << "based : " << printHash(based) << endl;
    //cout << "cmp   : " << printHash(cmp) << endl;
    //cout << "res   : " << printHash(resSimhash) << endl;

    int dis = 0;
    for (int i = 0; i < 64; ++i) {
        if (resSimhash & (((unsigned long)1 << i))) {
            ++dis;
        } 
    }
    
    //cout << "dis : " << dis << endl;
    
    return dis;
}

void InvertIndexAndRmDup::pushTheSimhash(unsigned long webSimhash)
{
    const uint16_t* pPartSimhash = reinterpret_cast<uint16_t*>(&webSimhash);

    for (int i = 0; i < 4; ++i, ++pPartSimhash) {
        auto retIter = _partSimHashMap.find(*pPartSimhash);

        if (retIter == _partSimHashMap.end()) {
            set<unsigned long> tmpSet;
            tmpSet.insert(webSimhash);
            _partSimHashMap.insert(make_pair(*pPartSimhash, tmpSet));
            continue;
        } 
        
        retIter->second.insert(webSimhash);
    }
}

string InvertIndexAndRmDup::printHash(unsigned long hashVal) 
{
    ostringstream oss;
    for (int i = 0; i < 64; ++i) {
        unsigned long bit = (hashVal & ((unsigned long)1 << i)) ? 1 : 0;
        oss << bit;
    }
    return oss.str();
}

void InvertIndexAndRmDup::rmDupWebpage()
{
    auto configMap = Configuration::getInstanceConfig()->getConfigMap();
    string offsetLib = configMap["offset_lib"];
    string webpageLib = configMap["webpage_lib"];

    loadLib(offsetLib, webpageLib);


    int dupWebCnt = 0;

    for (auto & webpage : _webPageLibVec) {
        if (webpageIfSim(webpage._simHash)) {

            ++dupWebCnt;

            //if (cnt == 1) { exit(0); }
            
            continue;
        }
        _webPageLibVecNew.push_back(webpage);
    }
    cout << "dupWebCnt : " << dupWebCnt << endl;

    saveLib();
}

void InvertIndexAndRmDup::saveLib()
{
    saveNewWebpageLib();
    saveNewOffsetLib();
}

void InvertIndexAndRmDup::saveNewWebpageLib()
{
    auto configMap = Configuration::getInstanceConfig()->getConfigMap();
    string newWebpagePath = configMap["new_webpage_lib"];

    ofstream ofs;
    ofs.open(newWebpagePath, ios::out);
    if (ofs.fail()) { ERROR_EXIT("webpage lib open error"); }
    
    long newDocid = 0;
    for (auto& webpage : _webPageLibVecNew) {
        webpage._docid = newDocid;
        long pos = ofs.tellp();
        
        string xmlWeb = webpage.toDoc();

        long len = xmlWeb.size();
        _offSetLibNew.insert(make_pair(newDocid, make_pair(pos, len)));

        ofs << xmlWeb;
        ++newDocid;
    }

    ofs.close();
}

void InvertIndexAndRmDup::saveNewOffsetLib()
{
    auto configMap = Configuration::getInstanceConfig()->getConfigMap();
    string newOffsetPath = configMap["new_offset_lib"];
    
    ofstream ofs;
    ofs.open(newOffsetPath, ios::out);

    if (ofs.fail()) { ERROR_EXIT("new offset lib open error"); }

    for (auto & elem : _offSetLibNew) {
        ofs << elem.first << " "
            << elem.second.first << " "
            << elem.second.second
            << endl;
    }

    ofs.close();
}

void InvertIndexAndRmDup::saveInvertIndex()
{
    auto configMap = Configuration::getInstanceConfig()->getConfigMap();
    string pathName = configMap["invert_index_lib"];
    
    ofstream ofs;
    ofs.open(pathName, ios::out);

    if (ofs.fail()) { ERROR_EXIT("invert index lib open error"); }

    for (auto & wordItem : _invertIndex) {
        ofs << wordItem.first << " ";
        for (auto & docidFreq : wordItem.second) {
            ofs << docidFreq.first << " "
                << docidFreq.second << " ";
        }
        ofs << endl;
    }
    ofs.close();
}

}
