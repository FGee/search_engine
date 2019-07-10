#include "../../inc/splitTool/cppJiebaSplitTool.h"

namespace wd
{

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

}
