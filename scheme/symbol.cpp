#include "symbol.h"

#include <string>

bool Symb(char& c) {
    std::string s = " <=>*/#";
    return (isalpha(c) || (s.find(c) != std::string::npos));
}
bool SymbNext(char c) {
    std::string s = " <=>*#?!-";
    return (isalpha(c) || isdigit(c) || (s.find(c) != std::string::npos));
}
