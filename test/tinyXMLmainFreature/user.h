#ifndef __USER_H__
#define __USER_H__

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

class User
{
public:
    User()
    {
        gender = 0;
    };

    User(const string& userName, const string& password, int gender, const string& mobile, const string& email)
    {
        this->userName = userName;
        this->password = password;
        this->gender = gender;
        this->mobile = mobile;
        this->email = email;
    }

    User(User& rhs)
    {
        rhs.userName = userName;
        rhs.password = password;
        rhs.gender = gender;
        rhs.mobile = mobile;
        rhs.email = email;
    }

    void print()
    {
        cout << "userName: " << userName << endl;
        cout << "password: " << password << endl;
        cout << "gender: " << gender << endl;
        cout << "mobile: " << mobile << endl;
        cout << "email" << email << endl;
    }

    string userName;
    string password;
    int gender;
    string mobile;
    string email;
};

#endif

