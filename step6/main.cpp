#include <iostream>
#include <string>

#include "environment.h"
#include "reader.h"
#include "evaluator.h"
#include "printer.h"
#include "core.h"
#include "common.h"

class Interface {
public:
    String read() {
        String str;
        if(!getline(std::cin, str))
            throw Exception::EXP_READ_END;
        return str;
    }
    AbstractType* eval(String exp) {
        AbstractType* root = Reader::read(exp);
        return evaluator.eval(root, &environment);
    }
    void print(AbstractType* obj) {
        std::cout << Printer::print(obj) << std::endl;
    }
    bool rep() {
        String str;
        std::cout << "user> ";
        try {
            str = read();
        } catch(Exception e) {
            return false;
        }
        print(eval(str));
        return true;
    }
    void loop() {
        bool flag = true;
        while(flag) {    
            try {
                flag = rep();
            } catch(Exception e) {
                switch(e) {
                case Exception::EXP_ANA_MATCHLESS:
                    std::cout << "Analyzer: matchless" << std::endl;
                    break;
                case Exception::EXP_ANA_NONE:
                    std::cout << "Analyzer: none" << std::endl;
                    break;
                case Exception::EXP_ANA_ESCAPE_ERROR:
                    std::cout << "Analyzer: escape error" << std::endl;
                    break;
                case Exception::EXP_READ_END:
                    std::cout << "Reader: reach the end" << std::endl;
                    break;
                case Exception::EXP_READ_MATCH_ERROR:
                    std::cout << "Reader: match error" << std::endl;
                    break;
                case Exception::EXP_READ_MATCH_MORE:
                    std::cout << "Reader: match more than max length" << std::endl;
                    break;
                case Exception::EXP_BUILDIN_FUNCTION_ARG_TYPE_ERROR:
                    std::cout << "Buildin Function: arg type error" << std::endl;
                    break;
                case Exception::EXP_BUILDIN_FUNCTION_LENGTH_ERROR:
                    std::cout << "Buildin Function: args length error" << std::endl;
                    break;
                case Exception::EXP_ENV_NOT_FOUND:
                    std::cout << "Environment: variable isn't found" << std::endl;
                    break;
                case Exception::EXP_HELPER_CONVERT_NONE:
                    std::cout << "Helper: cannot convert null to other type" << std::endl;
                    break;
                case Exception::EXP_HELPER_GET_ERROR:
                    std::cout << "Helper: get null" << std::endl;
                    break;
                case Exception::EXP_HELPER_APPEND_ERROR:
                    std::cout << "Helper: append a non-list-end" << std::endl;
                    break;
                case Exception::EXP_EVAL_CANNOT_EXECUTE:
                    std::cout << "Eval: cannot execute" << std::endl;
                    break;
                case Exception::EXP_EVAL_BUILDIN_LENGTH_ERROR:
                    std::cout << "Eval: buildin function args length error" << std::endl;
                    break;
                case Exception::EXP_EVAL_BUILDIN_LIST_ERROR:
                    std::cout << "Eval: buildin function takes a fake list" << std::endl;
                    break;
                case Exception::EXP_EVAL_BUILDIN_ARG_ERROR:
                    std::cout << "Eval: buildin function takes a non-flat arg list" << std::endl;
                    break;
                }
            }
        }
    }
    void generateMainEnvironment() {
        Core::registerBasicFunction(&environment);
        Core::registerFunction(&environment, "eval", QUOTEFUNCTION(o) {
            SINGLE(it, o);
            return evaluator.eval(it, &environment);
        });
    }

private:
    Evaluator evaluator;
    Environment environment;
} interface;

int main()
{
    interface.generateMainEnvironment();
    interface.loop();
    return 0;
}