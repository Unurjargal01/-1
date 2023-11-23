#pragma once

#include <memory>
#include <string>
#include <tokenizer.h>

class Object : public std::enable_shared_from_this<Object> {
public:
    virtual ~Object() = default;
};

class Number : public Object {
public:
    Number(const int& value) : value_(value){};
    int GetValue() const {
        return value_;
    };

private:
    int value_;
};

class Symbol : public Object {
public:
    Symbol(const std::string& name) : name_(name){};
    const std::string& GetName() const {
        return name_;
    };

private:
    std::string name_;
};

class Cell : public Object {
public:
    std::shared_ptr<Object> first_;
    std::shared_ptr<Object> second_;
    Cell(std::shared_ptr<Object> first, std::shared_ptr<Object> second)
        : first_(first), second_(second){};
    std::shared_ptr<Object> GetFirst() const {
        // return pairs_.first;
        return first_;
    };
    std::shared_ptr<Object> GetSecond() const {
        // return pairs_.second;
        return second_;
    };
};

class Dot : public Object {
public:
    Dot() = default;
};

class Quote : public Object {
public:
    Quote() = default;
};

class BracketClosed : public Object {
public:
    BracketClosed() = default;
};
class BracketOpen : public Object {
public:
    BracketOpen() = default;
};
// Dynamic_Cast?
template <class T>
std::shared_ptr<T> As(const std::shared_ptr<Object>& obj) {
    return std::dynamic_pointer_cast<T>(obj);
};
// Static_cast?
template <class T>
bool Is(const std::shared_ptr<Object>& obj) {
    if (std::dynamic_pointer_cast<T>(obj)) {
        return true;
    }
    return false;
};
