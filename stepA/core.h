#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <fstream>
#include <sstream>

#include <ctime>

#include "common.h"
#include "memory.h"
#include "helper.h"
#include "reader.h"
#include "printer.h"
#include "environment.h"

class Core
{
public:
    static void registerBasicFunction(Pointer<Environment> env);
    static void registerFunction(Pointer<Environment> env, String name, Function fun);
};

#endif // CORE_H