#pragma once

#include <string>
#include <map>

using namespace std;

namespace wd
{

struct Result
{
    string _title;
    string _summary;
    string _url;
    double _cosSim;
    long _docid;
    map<string, double> _weightVec;

    string toXML();
};

}
