#include "../inc/configuration.h"
#include "../inc/webpageLibProducer.h"
#include "../inc/invertIndexAndRmDup.h"

#include <iostream>
#include <memory>

using namespace std;
using namespace wd;

int main()
{
    Configuration::getInstanceConfig()->loadConfigFile("../conf/myconf.conf");
    
    //unique_ptr<WebpageLibProducer> upPageLibPro(new WebpageLibProducer());
    //upPageLibPro->producePageLib();

    unique_ptr<InvertIndexAndRmDup> upInvert(new InvertIndexAndRmDup());
    
    upInvert->rmDupWebpage();
    
    upInvert->productInvertIndex();

    return 0;
}

