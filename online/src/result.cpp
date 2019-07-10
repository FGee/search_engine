#include "../include/result.h"

namespace wd
{

string Result::toXML()
{
    return "<doc><title>" + _title +
        "</title><summary>" + _summary + " "
        "</summary><url>" + _url +
        "</url></doc>";
}

}
