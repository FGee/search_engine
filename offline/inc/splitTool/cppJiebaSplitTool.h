#pragma once
#include "splitTool.h"
#include "cppjieba/Jieba.hpp"

#include <set>

#define DICT_PATH "../data/dict/jieba.dict.utf8"
#define HMM_PATH "../data/dict/hmm_model.utf8"
#define USER_DICT_PATH "../data/dict/user.dict.utf8"
#define IDF_PATH "../data/dict/idf.utf8"
#define STOP_WORD_PATH "../data/dict/stop_words.utf8"

namespace wd
{

class CppJiebaSplitTool
: public SplitTool
{
public:
    CppJiebaSplitTool();
    void loadStopWordDict(const string&);
    ~CppJiebaSplitTool();
    vector<Word_t> cut(const string& sentence) override;
private:
    cppjieba::Jieba _jieba;
    unordered_set<string> _stopWordDict;
};

}
