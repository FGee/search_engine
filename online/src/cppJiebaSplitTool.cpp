#include "../include/splitTool/cppJiebaSplitTool.h"

#include <stdlib.h>

namespace wd
{

//CppJiebaSplitTool* CppJiebaSplitTool::_pCppJieba = nullptr;
CppJiebaSplitTool* CppJiebaSplitTool::_pCppJieba = CppJiebaSplitTool::getInstanceCppjieba();
pthread_once_t CppJiebaSplitTool::_once = PTHREAD_ONCE_INIT;

CppJiebaSplitTool::CppJiebaSplitTool()
: SplitTool(), _jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH,
         IDF_PATH, STOP_WORD_PATH)
    {
        loadStopWordDict(STOP_WORD_PATH);
        cout << "CppJiebaSplitTool()" << endl;
    }

void CppJiebaSplitTool::loadStopWordDict(const string& pathName)
{
    ifstream ifs;
    ifs.open(pathName, ios::in);
    if (ifs.fail()) { perror("stop word dict open error"); exit(0); }

    string line;
    while (getline(ifs, line)) {
        istringstream iss(line);
        string wrd;
        iss >> wrd;
    
        _stopWordDict.insert(wrd);
    }
    
    _stopWordDict.insert(" "); // 把分词结果中的空格加入停用词

    ifs.close();
}

vector<string> CppJiebaSplitTool::cut(const string& sentence) 
{
    vector<string> words;
    _jieba.Cut(sentence, words, true);

    vector<string> wordsRmStop;
    
    for (auto & wrd : words) {
        auto retIter = _stopWordDict.find(wrd);
        if (retIter == _stopWordDict.end()) {
            wordsRmStop.push_back(wrd);
        }
    }

    return wordsRmStop;
}

CppJiebaSplitTool::~CppJiebaSplitTool()
{
    cout << "~CppJiebaSplitTool()" << endl;
}

CppJiebaSplitTool* CppJiebaSplitTool::getInstanceCppjieba()
{
    pthread_once(&_once, init);
    return _pCppJieba;
}

void CppJiebaSplitTool::init()
{
    if (nullptr == _pCppJieba) {
        _pCppJieba = new CppJiebaSplitTool();
        atexit(destory);
    }
}

void CppJiebaSplitTool::destory()
{
    if (nullptr != _pCppJieba) {
        delete _pCppJieba;
        _pCppJieba = nullptr;
    }
}

}
