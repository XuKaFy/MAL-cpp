#ifndef CORE_H
#define CORE_H

#include <iostream>

#include "common.h"
#include "helper.h"
#include "reader.h"
#include "printer.h"
#include "environment.h"

class Core
{
public:
    static void registerBasicFunction(Environment* env);
    static void registerFunction(Environment* env, String name, Function fun);
};

#endif // CORE_H