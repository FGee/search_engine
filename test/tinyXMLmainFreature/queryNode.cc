#include "user.h"
#include "tinyxml2.h"

#include <iostream>

using std::cout;
using std::endl;
using std::cin;
using namespace tinyxml2;

XMLElement* queryUserNodeByName(XMLElement* root, const string& userName)
{
    XMLElement* userNode = root->FirstChildElement("User");
    while (userNode != NULL) {
        if (userNode->Attribute("Name") == userName)
            break;
        userNode = userNode->NextSiblingElement();
    }

    return userNode;
}

User* queryUserByName(const char* xmlPath, const string& userName)
{
    XMLDocument doc;
    if (doc.LoadFile(xmlPath)) {
        cout << "load xml file failed" << endl;
    }

    XMLElement* root = doc.RootElement();
    XMLElement* userNode = queryUserNodeByName(root, userName);

    if (userNode != NULL) {
        User* user = new User();
        user->userName = userName;
        user->password = userNode->Attribute("Password");
        XMLElement* genderNode = userNode->FirstChildElement("Gender");
        user->gender = atoi(genderNode->GetText());
        XMLElement* mobileNode = userNode->FirstChildElement("Mobile");
        user->mobile = mobileNode->GetText();
        XMLElement* emailNode = userNode->FirstChildElement("Email");
        user->email = emailNode->GetText();
        return user;
    }

    return NULL;
}

//void test()
//{
//    User* puser = queryUserByName("user.xml", "xiaoming");
//    puser->print();
//}

