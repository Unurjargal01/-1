#pragma once

#include <variant>
#include <istream>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <iostream>
struct SymbolToken {
    std::string name;

    bool operator==(const SymbolToken& other) const {
        return name == other.name;
    };
};

struct QuoteToken {
    bool operator==(const QuoteToken&) const {
        return true;
    };
};

struct DotToken {
    bool operator==(const DotToken&) const {
        return true;
    };
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    int value;

    bool operator==(const ConstantToken& other) const {
        return value == other.value;
    };
};

using Token = std::variant<ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken>;

bool Symb(char& c) {
    std::string s = " <=>*#";
    return (isalpha(c) || (s.find(c) != std::string::npos));
}
bool SymbNext(char c) {
    std::string s = " <=>*#?!-";
    return (isalpha(c) || isdigit(c) || (s.find(c) != std::string::npos));
}

class Tokenizer {
public:
    Tokenizer(std::istream *in) : in_(in) {
        Read();
    };
    
    bool IsEnd() {
        // return  (in_->peek() == EOF) && (it_ == vect_.end()) && in_->eof();
        // std::cout << "peeking: " << in_->peek() << std::endl;
        // std::cout << "peeking: " << in_->peek() << std::endl;
        return (it_ == vect_.end()) && (in_->peek() == EOF) && (in_->eof());
    };

    void Next() {
        if (IsEnd()) {
            return;
        }
        ++it_;
        if (it_ == vect_.end()) {
            Read();
        }
    }

    Token GetToken() {
        return *it_;
    }

    void Read() {
        vect_.clear();
        std::string token;
        std::getline(*in_ >> std::ws, token, ' ');
        // std::cout << token << std::endl;
        if (token.empty()) {
            // vect_.clear();
            it_ = vect_.begin();
            return;
        }
        std::stringstream sub{token};
        
        char c;
        while (sub.get(c)) {
            if (c == '(') {
                vect_.push_back(BracketToken::OPEN);
            } else if (c == ')') {
                vect_.push_back(BracketToken::CLOSE);
            } else if (c == '.') {
                vect_.push_back(DotToken{});
            } else if (c == '\'') {
                vect_.push_back(QuoteToken{});
            } else if ((c == '+' || c == '-') && !isdigit(sub.peek())) {
                vect_.push_back(SymbolToken{std::string{c}});
            } else if (((c == '+' || c == '-') && isdigit(sub.peek())) || isdigit(c)) {
                std::string cur;
                cur += c;
                while (isdigit(sub.peek())) {
                    sub.get(c);
                    cur += c;
                }
                // std::cout << "BY READING DIGIT: " << cur << std::endl;
                vect_.push_back(ConstantToken{std::atoi(cur.c_str())});
            } else if (Symb(c)) {
                std::string cur;
                cur += c;
                while ((sub.peek() != EOF) && SymbNext(sub.peek())) {
                    sub.get(c);
                    cur += c;
                }
                // std::cout << "Reading Symb: " << cur << std::endl;
                vect_.push_back(SymbolToken{cur});
            }
        it_ = vect_.begin();
        }
        
    }
private:
    std::istream *in_;
    std::vector<Token> vect_;
    std::vector<Token>::iterator it_;
};
