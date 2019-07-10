#include "tinyxml2.h"
#include "user.h"

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

bool deleteUserByName(const char* xmlPath, const string& userName) 
{
    XMLDocument doc;
    if (doc.LoadFile(xmlPath)) {
        cout << "load xml file failed" << endl;
        return false;
    }
    XMLElement* root = doc.RootElement();
    //doc.DeleteNode(root);
    
    XMLElement* userNode = queryUserNodeByName(root, userName);
    if (NULL != userNode)
    {
        userNode->DeleteAttribute("Password");
        XMLElement* emailNode = userNode->FirstChildElement("Email");
        userNode->DeleteChild(emailNode);
        if (0 == doc.SaveFile(xmlPath)) {
            return true;
        }
    }
    return false;
}

int main()
{
    if (deleteUserByName("user1.xml", "xiaoming")) {
        cout << "delete success" << endl;
    } else {
        cout << "delete fail" << endl;
    }

    return 0;
}

