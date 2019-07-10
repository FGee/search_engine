#include "tinyxml2.h"
#include "user.h"

#include <stdlib.h>
#include <stdio.h>

#include <iostream>

using std::cout;
using std::endl;
using std::cin;
using namespace tinyxml2;

//function:insert XML node 
//parm:xmlPath:xml file path; user:User object
//return:0 success; not 0: fail;

int insertXMLNode(const char* xmlPath, const User& user)
{
    XMLDocument doc;
    int res = doc.LoadFile(xmlPath);
    if (res != 0)
    {
        cout << "load xml file failed" << endl;
        return res;
    }
    XMLElement* root = doc.RootElement();
    
    XMLElement* userNode = doc.NewElement("User");
    userNode->SetAttribute("Name", user.userName.c_str());
    userNode->SetAttribute("Password", user.password.c_str());
    root->InsertEndChild(userNode);

    XMLElement* gender = doc.NewElement("Gender");
    char buf[20] = { 0 };
    sprintf(buf, "%d", user.gender);
    XMLText* genderText = doc.NewText(buf);
    gender->InsertEndChild(genderText);
    userNode->InsertEndChild(gender);

    XMLElement* mobile = doc.NewElement("Mobile");
    mobile->InsertEndChild(doc.NewText(user.mobile.c_str()));
    userNode->InsertEndChild(mobile);

    XMLElement* email = doc.NewElement("Email");
    email->InsertEndChild(doc.NewText(user.email.c_str()));
    userNode->InsertEndChild(email);

    return doc.SaveFile(xmlPath);
}

int main()
{
    User user("xiaoming", "123456", 1, "10086", "123456@qq.com");
    insertXMLNode("user.xml", user);

    return 0;
}

