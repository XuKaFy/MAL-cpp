#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <fstream>
#include <sstream>

#include <ctime>

#include "type.h"
#include "memory.h"
#include "helper.h"
#include "reader.h"
#include "printer.h"
#include "environment.h"

class Core
{
public:
    static void registerBasicFunction   (EnvironmentPointer env);
    static void registerFunction        (EnvironmentPointer env, String name, Function fun);
};

#endif // CORE_H