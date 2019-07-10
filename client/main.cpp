#include "client.h"
#include "jsonFormat.h"
#include "macro.h"

#include <cstdlib>

using namespace wd;
using namespace std;

int main(int argc, char** argv)
{
    if (argc != 3) { ERROR_EXIT("args error"); }
    JsonFormat jsonFormat;
    Client myClient(atoi(argv[2]), argv[1]);
    myClient.startQueryLoop(jsonFormat);
}
