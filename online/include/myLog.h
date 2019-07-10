#ifndef __MY_LOG_H__
#define __MY_LOG_H__

#include <stdio.h>

#include <iostream>
#include <string>
#include <sstream>

#include <log4cpp/Category.hh>

using std::cout;
using std::endl;
using std::string;
using std::ostringstream;
using namespace log4cpp;

#define prefix(msg) (string("[").append(__FILE__)\
                     .append(": ").append(std::to_string(__LINE__))\
                     .append(": ").append(__FUNCTION__)\
                     .append("]").append(": ").append(msg))

#define LogDebug(msg, ...) Mylog::getInstance()->debug(prefix(msg).c_str(), ##__VA_ARGS__) 

#define LogInfo(msg, ...) Mylog::getInstance()->info(prefix(msg).c_str(), ##__VA_ARGS__) 

#define LogWarn(msg, ...) Mylog::getInstance()->warn(prefix(msg).c_str(), ##__VA_ARGS__) 

#define LogError(msg, ...) Mylog::getInstance()->error(prefix(msg).c_str(), ##__VA_ARGS__) 

namespace wd 
{
class Mylog
{
public:
    class AutoRelease
    {
    public:
        AutoRelease();
        ~AutoRelease();
    };
    template <typename...Args>
    void warn(Args...args)
    {
        _root.warn(args...);
    }

    template <typename...Args>
    void error(Args...args)
    {
        _root.error(args...);
    }

    template <typename...Args>
    void debug(Args...args)
    {
        cout << sizeof...(args) << endl;

        _root.debug(args...);
    }

    template <typename...Args>
    void info(Args...args)
    {
        _root.info(args...);
    }

    void warn(const char* msg);
    void error(const char* msg);
    void debug(const char* msg);
    void info(const char* msg);

    static Mylog* getInstance() 
    {
        if (nullptr == _pMylog) {
            _pMylog = new Mylog;
        }
        return _pMylog;
    }

private:
    const char* getInfo(const char* msg);
    Mylog();
    ~Mylog();
private:
    Category& _root;
    static Mylog* _pMylog;
    static AutoRelease _auto;
};
}

#endif
