#include "printer.h"

String Printer::print(AbstractType *obj)
{
    switch(obj->type()) {
    case Type::TYPE_NULL:
        return "";
    case Type::TYPE_ATOM:
        return castAtom(Helper::convert<AtomType*>(obj)->atom());
    case Type::TYPE_FLOAT:
        return castFloat(Helper::convert<FloatType*>(obj)->number());
    case Type::TYPE_LIST:
        return castList((Helper::convert<ListType*>(obj)));
    case Type::TYPE_BUILDIN_FUNCTION:
        return castBuildinFunction(Helper::convert<BuildinFunctionType*>(obj));
    case Type::TYPE_STRING:
        return castString(Helper::convert<StringType*>(obj)->string());
    case Type::TYPE_LAMBDA:
        return castLambda(Helper::convert<LambdaType*>(obj));
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

String Printer::printWithEscape(String s)
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
    std::ostringstream s;
    s << n;
    return s.str();
}

String Printer::castAtom(Atom n)
{
    return n;
}

String Printer::castList(ListType *n)
{
    String ans = "(";
    if(!Helper::isEmpty(n)) {
        bool first = true;
        AbstractType* remain = Helper::foreach(n, [&](AbstractType* o) {
            if(first) {
                first = false;
            } else {
                ans += " ";
            }
            ans += print(o);
        });
        if(!Helper::isEmpty(remain))
            ans += " . " + print(remain);
    }
    ans += ")";
    return ans;
}

String Printer::castString(String n)
{
    return '"' + n + '"';
}

String Printer::castBuildinFunction(BuildinFunctionType* n)
{
    return "#<procedure:" + n->name() + ">";
}

String Printer::castLambda(LambdaType* n)
{
    String ans = "#<lambda:(";
    ans += print(n->arg());
    ans += " (begin " + print(n->body());
    ans += "))>";
    return ans;
}