#include "printer.h"

String Printer::print(ValuePointer obj)
{
    switch(obj->type()) {
    case Type::TYPE_NULL:
        return "nil";
    case Type::TYPE_SYMBOL:
        return castSymbol(GETSYMBOL(obj));
    case Type::TYPE_FLOAT:
        return castFloat(GETFLOAT(obj));
    case Type::TYPE_INTEGER:
        return castInteger(GETINTEGER(obj));
    case Type::TYPE_LIST:
        return castList(GETLIST(obj));
    case Type::TYPE_BUILDIN:
        return castBuildinFunction(GETBUILDIN(obj));
    case Type::TYPE_STRING:
        return castString(GETSTRING(obj));
    case Type::TYPE_LAMBDA:
        return castLambda(GETLAMBDA(obj));
    case Type::TYPE_MACRO:
        return castMacro(GETMACRO(obj));
    case Type::TYPE_VECTOR:
        return castVector(GETVECTOR(obj));
    case Type::TYPE_HASHMAP:
        return castMap(GETMAP(obj));
    case Type::TYPE_KEYWORD:
        return castKeyword(GETKEYWORD(obj));
    }
    return "#<error: print an unknown type>";
}

int Printer::hexFloat(String::value_type c)
{
    int ans = 0;
    if(isdigit(c)) {
        ans = c - '0';
    } else if(isalpha(c)) {
        ans = tolower(c) - 'a';
        if(ans >= 16)
            throw Exception("Printer::hexFloat: Wrong format");
    }
    return ans;
}

int Printer::octFloat(String::value_type c)
{
    if(!isdigit(c))
        throw Exception("Printer::octFloat: Wrong format");
    int ans = c - '0';
    if(ans >= 8)
            throw Exception("Printer::octFloat: Wrong format");
    return ans;
}

String Printer::printWithEscape(const String &s)
{
    String ans;
    for(size_t i=0; i<s.size(); ++i) {
        if(s[i] != '\\') {
            ans += s[i];
            continue;
        }
        ++i;
        switch(s[i]) {
        case 'a':
            ans += '\a';
            break;
        case 'b':
            ans += '\b';
            break;
        case 'f':
            ans += '\f';
            break;
        case 'n':
            ans += '\n';
            break;
        case 'r':
            ans += '\r';
            break;
        case 't':
            ans += '\t';
            break;
        case 'v':
            ans += '\v';
            break;
        case '\\':
            ans += '\\';
            break;
        case '?':
            ans += '\?';
            break;
        case '\'':
            ans += '\'';
            break;
        case '"':
            ans += '"';
            break;
        case '0':
            if(isdigit(s[i])) {
                int con = 0;
                for(int j=0; j<2; ++j)
                    con = con * 8 + octFloat(s[i]);
                ans += con;
            } else {
                ans += '\0';
            }
            break;
        default:
            if(!isalnum(s[i]))
                throw Exception("Printer::printWithEscape: Wrong format");
            int con = 0;
            if(s[i] == 'x') {
                ++i;
                for(int j=0; j<2; ++j, ++i)
                    con = con * 16 + hexFloat(s[i]);
                ans += String::value_type(con / 16);
                ans += String::value_type(con % 16);
            } else {
                for(int j=0; j<3; ++j, ++i)
                    con = con * 8 + octFloat(s[i]);
                ans += con;
            }
            break;
        }
    }
    return ans;
}

String Printer::castFloat(Float n)
{
    return std::to_string(n);
}

String Printer::castInteger(Integer n)
{
    return std::to_string(n);
}

String Printer::castSymbol(const Symbol &n)
{
    return n;
}

String Printer::castList(ListPointer n)
{
    String ans = "(";
    if(!ISEMPTY(n)) {
        bool first = true;
        ValuePointer remain = FOREACH(o, n, {
            if(first) {
                first = false;
            } else {
                ans += " ";
            }
            ans += print(o);
        });
        if(!ISEMPTY(remain))
            ans += " . " + print(remain);
    }
    ans += ")";
    return ans;
}

String Printer::castString(const String &n)
{
    return '"' + n + '"';
}

String Printer::castBuildinFunction(Pointer<BuildinType> n)
{
    return "#<procedure:" + n->name() + ">";
}

String Printer::castLambda(Pointer<LambdaType> n)
{
    String ans = "#<lambda:(";
    ans += castList(n->arg());
    String in = castList(n->body());
    in.erase(in.begin());
    in.pop_back();
    ans += " (begin " + in;
    ans += "))>";
    return ans;
}

String Printer::castMacro(Pointer<MacroType>n)
{
    String ans = "#<macro:(";
    ans += castList(n->arg());
    String in = castList(n->body());
    in.erase(in.begin());
    in.pop_back();
    ans += " (begin " + in;
    ans += "))>";
    return ans;
}

String Printer::castVector(const Vector& n)
{
    String ans = "[";
    bool first = true;
    for(auto i : n) {
        if(first) {
            first = false;
        } else {
            ans += " ";
        }
        ans += print(i);
    }
    ans += "]";
    return ans;
}

String Printer::castMap(const Map& n)
{
    String ans = "{";
    bool first = true;
    for(auto i : n) {
        if(first) {
            first = false;
        } else {
            ans += " ";
        }
        ans += castKeyword(i.first) + " " + print(i.second);
    }
    ans += "}";
    return ans;
}

String Printer::castKeyword(const Keyword& key)
{
    return ":" + castSymbol(key);
}