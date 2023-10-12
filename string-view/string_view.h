#pragma once

#include <string>
#include <cstring>
#include <iterator>

class StringView {
public:
    StringView(const std::string& l, size_t start = 0, size_t sublen = std::string::npos)
        : s_(&(*(l.begin() + start))), len_(l.size() - start), sublen_(std::min(len_, sublen)){};
    // std::min(len, sublen_)
    StringView(const char* l) : s_(&l[0]), len_(std::strlen(l)), sublen_(std::strlen(l)) {
    }
    StringView(const char* l, size_t len) : s_(&l[0]), len_(std::strlen(l)), sublen_(len) {
    }

    // const char* why taking as std::string has a problem
    /*StringView& operator= (const char* l) {
        init_char(l);
        return *this;
    }*/
    char operator[](int n) const {
        return *(s_ + n);
    }
    std::string Data() const {
        return std::string(s_, s_ + len_ + 1).data();
    }

    int Size() const {
        return sublen_;
    }

private:
    // s is initiater pointer, sublen is the length of followin string
    // len is overall string len.
    const char* s_;
    size_t len_, sublen_;
};
