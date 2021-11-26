#ifndef DEFINITION_H
#define DEFINITION_H

#include <string>
#include <map>
#include <vector>
#include <functional>

template<typename T>
class Pointer;

class AbstractType;
class AtomType;
class BuildinType;
class ListType;
class LambdaType;
class MacroType;
class NumberType;
class StringType;

class Environment;

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

typedef     Pointer<AbstractType>                        ValueType;

typedef     std::string                                  String;
typedef     double                                       Number;
typedef     String                                       Atom;
typedef     std::map<String, ValueType>                  Map;
typedef     std::function<ValueType(Pointer<ListType>)>  Function;
typedef     String                                       Exception;

#endif // DEFINITION_H