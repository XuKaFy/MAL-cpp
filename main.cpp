#include <iostream>
#include <string>

#include "environment.h"
#include "definition.h"
#include "evaluator.h"
#include "printer.h"
#include "reader.h"
#include "type.h"
#include "core.h"

#define EOF_EXCEPTION Exception("Interface::read: EOF")

class Interface {
public:
    Interface() {
        environment = new EnvironmentType();
    }

    String read() {
        String str;
        if(!getline(std::cin, str))
            throw EOF_EXCEPTION;
        return str;
    }

    ValuePointer eval(String exp) {
        ValuePointer root = VALUE(Reader::read(exp));
        //print(root);
        return evaluator.eval(root, environment);
    }

    void print(ValuePointer obj) {
        std::cout << Printer::print(obj, true) << std::endl;
    }

    bool rep() {
        String str;
        std::cout << "user> ";
        try {
            str = read();
        } catch(Exception e) {
            return false;
        }
        try {
            print(eval(str));
        } catch(ValuePointer k) {
            std::cout << "rep: Caught error: " + Printer::print(k, true) << std::endl;
            return false;
        }
        return true;
    }

    void loop() {
        bool flag = true;
        while(flag) {    
            try {
                flag = rep();
            } catch(Exception e) {
                if(e == EOF_EXCEPTION)
                    break;
                std::cout << e << std::endl;
            }
        }
        environment->clear();
    }

    void generateMainEnvironment() {
        try {
            Core::registerBasicFunction(environment);
            environment->setValue("*host-language*", eval("\"C++\""));
            environment->setValue("true", eval("(quote t)"));
            environment->setValue("false", eval("(quote ())"));
            environment->setValue("not", eval("(fn* (x) (if x false true))"));
            environment->setValue("load-file", eval("(fn* (f) (eval (read-string (str \"(do \" (slurp f) \")\"))))"));
        } catch (Exception e) {
            std::cout << e << std::endl;
        }
    }

private:
    Evaluator           evaluator;
    EnvironmentPointer  environment;
} interface;

int main()
{
    interface.generateMainEnvironment();
    interface.loop();
    return 0;
}