#include "../inc/configuration.h"
#include "../inc/macro.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace wd
{

void Configuration::loadConfigFile(const char* fileName)
{
    ifstream ifs;
    ifs.open(fileName, ios::in);

    if (ifs.fail()) { ERROR_EXIT(">> load config error"); }

    string line;
    while (getline(ifs, line)) {
        pair<string, string> newPair;
        
        istringstream iss(line);
        iss >> newPair.first;
        iss >> newPair.second;

        _configMap.insert(newPair);
    }
    ifs.close();
}

const map<string, string>& Configuration::getConfigMap() const { return _configMap; }

pthread_once_t Configuration::_once = PTHREAD_ONCE_INIT;
Configuration* Configuration::_pConfig = Configuration::getInstanceConfig();

Configuration* Configuration::getInstanceConfig()
{
    pthread_once(&_once, init);
    return _pConfig;
}

void Configuration::init()
{
    if (nullptr == _pConfig) {
        _pConfig = new Configuration();
        atexit(destory);
    }
}

void Configuration::destory()
{
    if (nullptr != _pConfig) {
        delete _pConfig;
        _pConfig = nullptr;
    }
}

Configuration::Configuration()
{
    cout << "Configuration()" << endl;
}

Configuration::~Configuration()
{
    cout << "~Configuration()" << endl;
}

}
