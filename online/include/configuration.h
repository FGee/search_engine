#pragma once
#include <string>
#include <map>

#define ENG_RAW_FILES_PATH "eng_raw_files_path"
#define CN_RAW_FILES_PATH "cn_raw_files_path"
#define INDEX_FILES_PATH "index_files_path"
#define DICT_STORE_PATH "dict_store_path"

using namespace std;

namespace wd
{

using Item = string;
using Value = string;

class Configuration
{
    class AutoRelease
    {
    public:
        AutoRelease();
        ~AutoRelease();
    };
public:
    friend class AutoRelease;
    map<Item, Value> _configMap;
    void loadConfigFile(const char* fileName);
    map<Item, Value>& getConfigMap();
private:
public:
    static Configuration* getInstance();
private:
    Configuration();
    ~Configuration();

    static Configuration* _pConfiguration;
    static AutoRelease _auto;
};

}
