#include "../include/workThreadTask.h"
#include "../include/tcpConnection.h"
#include "../include/dictoryary.h"
#include "../include/cacheManager.h"
#include "../include/LRUcache.h"
#include "../include/myLog.h"
#include "../include/splitTool/cppJiebaSplitTool.h"

#include <json/json.h>

#include <iostream>
#include <functional>
#include <sstream>
#include <cstdlib>

extern __thread const char* threadName;

namespace wd
{

WorkThreadTask::WorkThreadTask(shared_ptr<TcpConnection> sp, const string& m)
    : _sptrConn(sp), _msg(m) {}

void WorkThreadTask::process()
{
    // <<decode>>
    string queryText = jsonToQueryWrd(_msg);

    // <<compute>>
    string jsonStr;
    auto pCacheMan = CacheManager::getInstance();
    cout << "Thread " << threadName << " handle query" << endl;
    if (pCacheMan->isUpdating()) {
        matchWrdIntoResult(queryText);    
        jsonStr = obtainResultJson(RESULT_NUM);
    } else {
        LogDebug("Thread %d handle query task", atol(threadName));
        auto& cache = pCacheMan->getCache(atol(threadName));
        jsonStr = cache.searchInCache(queryText);
        if (jsonStr == string()) {
            matchWrdIntoResult(queryText);
            if (!_resultQue.empty()) {
                jsonStr = obtainResultJson(RESULT_NUM);
                cache.addIntoCache(queryText, jsonStr);
            }
        }
    }

    // <<encode>>
    _sptrConn->sendInLoop(jsonStr);
}

string WorkThreadTask::jsonToQueryWrd(const string& jsonData)
{
    Json::Reader readInfo;

    Json::Value root;
    if (readInfo.parse(jsonData, root)) {
        if (root["QueryWord"].isString()) {
            return root["QueryWord"].asString();
        } 
    }
    return string();
}

void WorkThreadTask::matchWrdIntoResult(const string& queryText)
{
    auto pCppJieba = CppJiebaSplitTool::getInstanceCppjieba();
    WebPage queryWeb;
    queryWeb.webPageInitNoXML(queryText, pCppJieba);

    map<long, Result> tmpRes;

    if (!ifExistMatch(queryWeb, tmpRes)) {
        return;
    }
    
    productResult(queryWeb, tmpRes);
}

bool WorkThreadTask::ifExistMatch(WebPage& queryWeb, map<long, Result>& tmpRes)
{
    auto& invertIndexMap = Dictionary::getInstance()->_invertIndexLib;
    double webNum = Dictionary::getInstance()->_webpageLib.size();
    
    double totalWeight = 0;

    for (auto& wordFreq : queryWeb._wordFreMap) {
        auto retIter = invertIndexMap.find(wordFreq.first);
        if (retIter == invertIndexMap.end()) {
            return false;
        }

        double docFreq = retIter->second.size();
        double IDF = log(webNum / (docFreq + 1)) / log(2);
        double TF = wordFreq.second;
        double w = TF * IDF;
        queryWeb._weightVector.insert(make_pair(wordFreq.first, w));
        totalWeight += (w * w);

        for (auto & docidWght : retIter->second) {
            long docid = docidWght.first;
            auto retIter = tmpRes.find(docid);

            if (retIter == tmpRes.end()) {
                Result res;
                res._docid = docid;
                res._weightVec.insert(make_pair(wordFreq.first, docidWght.second));

                tmpRes.insert(make_pair(docid, res));
                continue;
            }

            retIter->second._weightVec.insert(make_pair(wordFreq.first, docidWght.second));
        }
    }

    for (auto & wordWght : queryWeb._weightVector) {
        wordWght.second /= (sqrt(totalWeight));
    }

    unsigned long wrdNum = queryWeb._wordFreMap.size();

    for (auto iter = tmpRes.begin(); iter != tmpRes.end();) {
        if (iter->second._weightVec.size() != wrdNum) {
    cout << "5555555" << endl;
            iter = tmpRes.erase(iter);
        } else {
            ++iter;
        }
    }

    return tmpRes.size();
}

void WorkThreadTask::productResult(WebPage& queryWeb, map<long, Result>& resMap)
{
    auto& webpageVec = Dictionary::getInstance()->_webpageLib;
    
    for (auto & resPair : resMap) {
        WebPage tmpWeb = webpageVec[resPair.first];

        resPair.second._title = tmpWeb._title;
        resPair.second._url = tmpWeb._url;

        string wrd = queryWeb._weightVector.begin()->first;
        resPair.second._summary = tmpWeb.productSummary(wrd);

        resPair.second._cosSim = computeCosSimilarity(toWeightVector(queryWeb._weightVector), 
                                                      toWeightVector(resPair.second._weightVec));

        _resultQue.push(resPair.second);
    }
}

vector<double> WorkThreadTask::toWeightVector(const map<string, double>& refMap)
{
    vector<double> tmpVec;
    for (const auto& elem : refMap) {
        tmpVec.push_back(elem.second);
    }
    return tmpVec;
}

double WorkThreadTask::computeCosSimilarity(const vector<double>& v1, const vector<double>& v2)
{
    auto modularLenFunc = [](vector<double> vec) {
        double total = 0;
        for (auto dVal : vec) {
            total += dVal;
        }
        return sqrt(total);
    };
    
    double scalarProduct = 0;
    long dimension = v1.size();
    
    for (long i = 0; i < dimension; ++i) {
        scalarProduct += v1[i] * v2[i];
    }

    return scalarProduct / (modularLenFunc(v1) * modularLenFunc(v2));
}

string WorkThreadTask::obtainResultJson(int resultNum)
{
    if (_resultQue.empty()) {
        return string();
    }
    //Json::StyledWriter writerInfo;
    Json::FastWriter writerInfo;

    Json::Value root;

    for (int i = 0; i < resultNum; ++i) {
        if (_resultQue.empty()) { break; }
        Result tmp = _resultQue.top();
        _resultQue.pop();
        Json::Value resultValue;

        ostringstream oss;
        oss << i + 1;

        root["result:" + oss.str()] = tmp.toXML();
    }

    return writerInfo.write(root);
}

}
