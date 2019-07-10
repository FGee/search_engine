#pragma once
#include <error.h>

namespace wd
{

#define ERROR_EXIT(msg) do{\
    perror(msg);\
    exit(0);\
}while(0);

}
