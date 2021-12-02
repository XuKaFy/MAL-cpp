#include "printer.h"

String Printer::print(ValuePointer obj, bool readably)
{
    switch(obj->type()) {
    case Type::TYPE_NULL:
        return "nil";
    case Type::TYPE_SYMBOL:
        return castSymbol(GETSYMBOL(obj), readably);
    case Type::TYPE_FLOAT:
        return castFloat(GETFLOAT(obj), readably);
    case Type::TYPE_INTEGER:
        return castInteger(GETINTEGER(obj), readably);
    case Type::TYPE_LIST:
        return castList(GETLIST(obj), readably);
    case Type::TYPE_BUILDIN:
        return castBuildinFunction(GETBUILDIN(obj), readably);
    case Type::TYPE_STRING:
        return castString(GETSTRING(obj), readably);
    case Type::TYPE_LAMBDA:
        return castLambda(GETLAMBDA(obj), readably);
    case Type::TYPE_MACRO:
        return castMacro(GETMACRO(obj), readably);
    case Type::TYPE_VECTOR:
        return castVector(GETVECTOR(obj), readably);
    case Type::TYPE_HASHMAP:
        return castMap(GETMAP(obj), readably);
    case Type::TYPE_KEYWORD:
        return castKeyword(GETKEYWORD(obj), readably);
    case Type::TYPE_ATOM:
        return castAtom(GETATOM(obj)->reference(), readably);
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

String Printer::castFloat(Float n, bool)
{
    return std::to_string(n);
}

String Printer::castInteger(Integer n, bool)
{
    return std::to_string(n);
}

String Printer::castSymbol(const Symbol &n, bool)
{
    return n;
}

String Printer::castList(ListPointer n, bool readably)
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
            ans += print(o, readably);
        });
        if(!ISEMPTY(remain))
            ans += " . " + print(remain, readably);
    }
    ans += ")";
    return ans;
}

String Printer::castString(const String &n, bool readably)
{
    if(readably) {
        return '"' + n + '"';
    }
    return printWithEscape(n);
}

String Printer::castBuildinFunction(Pointer<BuildinType> n, bool readably)
{
    if(readably) {
        return n->name();
    }
    return "#<procedure:" + n->name() + ">";
}

String Printer::castLambda(Pointer<LambdaType> n, bool)
{
    return "#<lambda>";
}

String Printer::castMacro(Pointer<MacroType>n, bool)
{
    return "#<macro>";
}

String Printer::castVector(const Vector& n, bool readably)
{
    String ans = "[";
    bool first = true;
    for(auto i : n) {
        if(first) {
            first = false;
        } else {
            ans += " ";
        }
        ans += print(i, readably);
    }
    ans += "]";
    return ans;
}

String Printer::castMap(const Map& n, bool readably)
{
    String ans = "{";
    bool first = true;
    for(auto i : n) {
        if(first) {
            first = false;
        } else {
            ans += " ";
        }
        ans += castKeyword(i.first, readably) + " " + print(i.second, readably);
    }
    ans += "}";
    return ans;
}

String Printer::castKeyword(const Keyword& key, bool)
{
    return ":" + castSymbol(key, true);
}

String Printer::castAtom(ValuePointer ref, bool)
{
    std::stringstream ss;
    ss << "#<atom: ";
    ss << ref.get();
    ss << ">";
    return ss.str();
}