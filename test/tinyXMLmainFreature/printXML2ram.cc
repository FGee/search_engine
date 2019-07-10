#include "tinyxml2.h"

#include <iostream>

using std::cout;
using std::endl;
using std::cin;
using namespace tinyxml2;

void print2ram(const char* xmlPath, XMLPrinter& xmlpriter)
{
    XMLDocument doc;
    if (0 != doc.LoadFile(xmlPath)) {
        cout << "load xml file failed" << endl;
        return;
    }
    doc.Print(&xmlpriter);
}

int main()
{
    XMLPrinter xmlpriter;
    print2ram("user.xml", xmlpriter);

    cout << xmlpriter.CStr() << endl;

    return 0;
}

