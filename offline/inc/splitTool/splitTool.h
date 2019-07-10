#pragma once

#include <vector>
#include <string>

using namespace std;

namespace wd
{

class SplitTool
{
public:
    using Word_t = string;
    virtual
        vector<Word_t> cut(const string& sentence) = 0;
    virtual
        ~SplitTool() {}
};

}
