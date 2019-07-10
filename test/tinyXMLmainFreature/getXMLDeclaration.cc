#include "tinyxml2.h"

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::cin;
using std::string;
using namespace tinyxml2;

bool getXMLDeclaration(const char* xmlPath, string& strDecl)
{
    XMLDocument doc;
    if (0 != doc.LoadFile(xmlPath)) {
        cout << "load xml file failed" << endl;
    }
    XMLNode* decl = doc.FirstChild();
    if (decl != NULL) {
        XMLDeclaration* declaration = decl->ToDeclaration();
        if (NULL != declaration)
        {
            strDecl = declaration->Value();
            return true;
        }
    }
    return false;
}

int main()
{
    string strDecl;
    if (getXMLDeclaration("user.xml", strDecl)) {
        cout << "find it" << endl;
        cout << "XML declaration: " << endl;
        cout << strDecl << endl;
    } else {
        cout << "not find it" << endl;
        return -1;
    }

    return 0;
}

