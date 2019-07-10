#include "tinyxml2.h"

#include <iostream>

using std::cout;
using std::endl;
using std::cin;
using namespace tinyxml2;

void print(const char* xmlPath)
{
    XMLDocument doc;
    if (0 != doc.LoadFile(xmlPath)) {
        cout << "load xml file failed" << endl;
        return;
    }
    doc.Print();
}

int main()
{
    print("user.xml");

    return 0;
}

