#include <iostream>
#include <string>

typedef std::string String;

enum class Exception {
    EXP_READ_END,
};

class Interface {
public:
    String read() {
        String str;
        if(!getline(std::cin, str))
            throw Exception::EXP_READ_END;
        return str;
    }
    String eval(String exp) {
        return exp;
    }
    void print(String obj) {
        std::cout << obj;
    }
    bool rep() {
        String str;
        print("user> ");
        try {
            str = read();
        } catch(Exception e) {
            return false;
        }
        print(eval(str) + "\n");
        return true;
    }
    void loop() {
        if(rep())
            loop();
    }
} interface;

int main()
{
    interface.loop();
    return 0;
}