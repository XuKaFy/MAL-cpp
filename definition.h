#ifndef DEFINITION_H
#define DEFINITION_H

#include <string>

enum class Type {
    TYPE_FLOAT,
    TYPE_INTEGER,
    TYPE_STRING,
    TYPE_SYMBOL,
    TYPE_LIST,
    TYPE_BUILDIN,
    TYPE_LAMBDA,
    TYPE_MACRO,
    TYPE_KEYWORD,
    TYPE_VECTOR,
    TYPE_HASHMAP,
    TYPE_NULL,
};

typedef     std::string     String;
typedef     double          Float;
typedef     long long       Integer;
typedef     String          Symbol;
typedef     String          Keyword;
typedef     String          Exception;

#define     SYM_QUOTE       "quote"
#define     SYM_COND        "cond"
#define     SYM_IF          "if"
#define     SYM_LAMBDA      "fn*"
#define     SYM_DEFINE      "def!"
#define     SYM_SET         "set!"
#define     SYM_DEMACRO     "defmacro!"
#define     SYM_LET         "let*"
#define     SYM_BEGIN       "do"
#define     SYM_APPLY       "apply"
#define     SYM_EVAL        "eval"
#define     SYM_QQ          "quasiquote"
#define     SYM_SUQ         "splice-unquote"
#define     SYM_UQ          "unquote"
#define     SYM_WITH_META   "with-meta"
#define     SYM_THROW       "throw"
#define     SYM_TRY         "try*"
#define     SYM_CATCH       "catch*"
#define     SYM_MACROEXP    "macroexpand"
#define     SYM_TIME_MS    "time-ms"
#define     SYM_EQ          "="
#define     SYM_VARIADIC    "&"
#define     SYM_SIM_QQ      "`"
#define     SYM_SIM_UQ      "~"
#define     SYM_SIM_SUQ     "~@"
#define     SYM_SIM_WM      "^"
#define     SYM_COMMENT     ";"
#define     SYM_SIM_QT      "'"

#endif // DEFINITION_H