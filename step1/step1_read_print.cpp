#include <iostream>
#include <string>

#include "reader.h"
#include "printer.h"
#include "common.h"

Reader reader;
Printer printer;

class Interface {
public:
    String read() {
        String str;
        if(!getline(std::cin, str))
            throw Exception::EXP_READ_END;
        return str;
    }
    AbstractType* eval(String exp) {
        return reader.read_str(exp);
    }
    void print(AbstractType* obj) {
        std::cout << printer.print_str(obj) << std::endl;
    }
    bool rep() {
        String str;
        std::cout << "user> ";
        try {
            str = read();
        } catch(Exception e) {
            return false;
        }
        std::cout << "--- READ END ---" << std::endl;
        print(eval(str));
        return true;
    }
    void loop() {
        try {
            if(rep())
                loop();
        } catch(Exception e) {
            switch(e) {
            case Exception::EXP_ANA_MATCHLESS:
                std::cout << "Analyzer: matchless" << std::endl;
                break;
            case Exception::EXP_ANA_NONE:
                std::cout << "Analyzer: none" << std::endl;
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
            }
        }
    }
} interface;

int main()
{
    interface.loop();
    return 0;
}