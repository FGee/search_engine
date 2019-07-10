//function: create a xml file
//param: xmlPath: xml file path
//return: 0,success, not 0 fail
#include "tinyxml2.h"

#include <iostream>

using namespace tinyxml2;
using std::cout;
using std::endl;
using std::cin;

int createXML(const char* xmlPath)
{
    const char* declaration = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"?>";
    XMLDocument doc;
    doc.Parse(declaration);

    //add decharation 
    //XMLDeclaration* declaration = doc.NewDeclaration();
    //doc.InsertFirstChild(declaration);
    XMLElement* root = doc.NewElement("DBUSER");
    doc.InsertEndChild(root);

    return doc.SaveFile(xmlPath);
}

int main()
{
    createXML("user.xml");

    return 0;
}

