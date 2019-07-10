#include "../include/configuration.h"
#include "../include/macro.h"

#include <sys/types.h>
#include <unistd.h>

#include <fstream>
#include <sstream>
#include <iostream>

namespace wd
{

Configuration::AutoRelease Configuration::_auto;

Configuration* Configuration::_pConfiguration = nullptr;
//Configuration* Configuration::_pConfiguration = Configuration::getInstance();

void Configuration::loadConfigFile(const char* fileName)
{
    ifstream ifs;
    ifs.open(fileName, ios::in);
    if (ifs.fail()) { ERROR_EXIT("open config"); }

    string line;
    while (getline(ifs, line)) {
        istringstream iss(line);
        string item;
        string value;
        iss >> item;
        iss >> value;

        pair<Item, Value> itemNew(item, value);
        _configMap.insert(itemNew);
    }
}

map<Item, Value>& Configuration::getConfigMap() 
{ return _configMap; }

inline
Configuration::AutoRelease::AutoRelease()
{
    cout << "AutoRelease()" << endl;
}

Configuration::AutoRelease::~AutoRelease()
{
    cout << "*****Configuration*****" << endl;
    cout << "process pid : " << getpid() << endl;
    cout << "_pConfiguration : " << _pConfiguration << endl;
    if (nullptr != _pConfiguration) {
        delete _pConfiguration;
    }
    cout << "Configuration: ~AutoRelease()" << endl;
}

Configuration* Configuration::getInstance()
{
    if (nullptr == _pConfiguration) {
        _pConfiguration = new Configuration();
        //(void)_auto;
    }
    return _pConfiguration;
}

inline
Configuration::Configuration()
{ cout << "Configuration()" << endl; }

Configuration::~Configuration() { cout << "~Configuration()" << endl; }

}
