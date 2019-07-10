#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__
#include "../inc/noncopyable.h"

#include <stdlib.h>

#include <map>

using namespace std;

namespace wd
{

class Configuration
: Noncopyable
{
public:
    void loadConfigFile(const char*);
    const map<string, string>& getConfigMap() const;
private:
    map<string, string> _configMap;

public:
    static Configuration* getInstanceConfig();
private:
    static void init();
    static void destory();
    
    Configuration();
    ~Configuration();

    static Configuration* _pConfig;
    static pthread_once_t _once;
};

}

#endif
