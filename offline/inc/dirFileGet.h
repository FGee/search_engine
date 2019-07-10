#pragma once
#include "noncopyable.h"

#include <vector>
#include <string>
#include <regex>

using namespace std;

namespace wd
{

class DirFileGet
: Noncopyable
{
public:
    DirFileGet(const char* dirName, const string&);
    const vector<string>& filePathVecGet() const;
private:
    void depthFirstTraverse(const char* dirName);

    vector<string> _filePathVec;
    regex _regExpr;
};

}
