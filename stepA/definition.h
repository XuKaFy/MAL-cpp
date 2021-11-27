#ifndef DEFINITION_H
#define DEFINITION_H

#include <string>

enum class Type {
    TYPE_NUMBER,
    TYPE_STRING,
    TYPE_ATOM,
    TYPE_LIST,
    TYPE_BUILDIN,
    TYPE_LAMBDA,
    TYPE_MACRO,
    TYPE_NULL,
};

typedef     std::string                                  String;
typedef     double                                       Number;
typedef     String                                       Atom;
typedef     String                                       Exception;

#endif // DEFINITION_H