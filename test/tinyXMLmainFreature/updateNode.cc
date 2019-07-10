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

bool updateUser(const char* xmlPath, User* user)
{
    XMLDocument doc;
    if (doc.LoadFile(xmlPath) != 0) {
        cout << "load xml file failed" << endl;
    }
    XMLElement* root = doc.RootElement();
    XMLElement* userNode = queryUserNodeByName(root, user->userName);

    if (userNode != NULL) {
        if (user->password != userNode->Attribute("Password")) {
            userNode->SetAttribute("Password", user->password.c_str());
        }
        XMLElement* genderNode = userNode->FirstChildElement("Gender");
        if (user->gender != atoi(genderNode->GetText())) {
            char buf[20] = { 0 };
            sprintf(buf, "%d", user->gender);
            genderNode->SetText(buf);
        }
        XMLElement* mobileNode = userNode->FirstChildElement("Mobile");
        if (user->mobile != mobileNode->GetText()) {
            mobileNode->SetText(user->mobile.c_str());
        }
        XMLElement* emailNode = userNode->FirstChildElement("Email");
        if (user->email != emailNode->GetText()) {
            emailNode->SetText(user->email.c_str());
        }
        if (0 == doc.SaveFile(xmlPath)) {
            return true;
        }
    }
    return false;
}

int main()
{
    User newUser("xiaoming", "123123456456", 0, "123456789", "123456@qq.com");
    if (updateUser("user.xml", &newUser)) {
        cout << "update success" << endl;
    } else {
        cout << "update fail" << endl;
    }

    return 0;
}

