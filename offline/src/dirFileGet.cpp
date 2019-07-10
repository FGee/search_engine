#include "../inc/dirFileGet.h"
#include "../inc/macro.h"

#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#include <iostream>

using namespace std;

namespace wd
{

DirFileGet::DirFileGet(const char* dirName, const string& pattern)
    : _regExpr(pattern)
{
    depthFirstTraverse(dirName);
}

void DirFileGet::depthFirstTraverse(const char* dirName)
{
    DIR* pDir = opendir(dirName);
    if (nullptr == pDir) { ERROR_EXIT("opendir error"); }

    string dirPath(dirName);

    struct dirent* pEntry;
    while (nullptr != (pEntry = readdir(pDir))) {
        if (!strcmp(".", pEntry->d_name) || !strcmp("..", pEntry->d_name)) {
            continue;
        }
        if (DT_DIR == pEntry->d_type) {
            dirPath = dirPath + "/" + pEntry->d_name;
            depthFirstTraverse(dirPath.c_str());
        }
        if (DT_REG == pEntry->d_type && regex_match(pEntry->d_name, _regExpr)) {
            _filePathVec.push_back(dirPath + "/" + pEntry->d_name);
        }
    }
}

const vector<string>& DirFileGet::filePathVecGet() const { return _filePathVec; }

}
