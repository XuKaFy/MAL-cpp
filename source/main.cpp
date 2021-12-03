#include <iostream>
#include <string>

#include "mal/interface.h"

Interface interface;

int main(int argc, char* argv[])
{
    interface.generateMainEnvironment(argc, argv);
    interface.loop();
    return 0;
}