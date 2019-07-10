#pragma once
#include "result.h"

#include <string.h>

#include <memory>
#include <queue>
#include <iostream>
#include <map>

#define RESULT_NUM 3

using namespace std;

namespace wd
{

class TcpConnection;

class WebPage;

class WorkThreadTask
{
    struct ResultComparator {
        bool operator()(const Result& lhs, const Result& rhs) {
            return lhs._cosSim < rhs._cosSim;
        }
    };

public:
    WorkThreadTask(shared_ptr<TcpConnection>, const string&);

    void process();
private:
    string jsonToQueryWrd(const string&);
    void matchWrdIntoResult(const string& wrd);
    bool ifExistMatch(WebPage&, map<long, Result>&);
    void productResult(WebPage&, map<long, Result>&);
    vector<double> toWeightVector(const map<string, double>&);
    double computeCosSimilarity(const vector<double>&, const vector<double>&);

    string obtainResultJson(int);

    shared_ptr<TcpConnection> _sptrConn;
    string _msg;
    priority_queue<Result, vector<Result>, WorkThreadTask::ResultComparator> _resultQue;
};

}
