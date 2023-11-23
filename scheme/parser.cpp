#include <object.h>
#include <parser.h>
#include <tokenizer.h>

#include <stdexcept>
#include <memory>
/*
std::shared_ptr<Object> Read(Tokenizer* tokenizer) {
    // throw std::runtime_error{"Not implemented"};
    Token first_token = tokenizer->GetToken();
    std::shared_ptr<Object> first;
    if (std::holds_alternative<ConstantToken>(first_token)) {
        first = std::make_shared<Number>(std::get<ConstantToken>(first_token).value);
    }
    if (!tokenizer->IsEnd()) {
        tokenizer->Next();
        auto second = Read(tokenizer);
        auto p = std::make_shared<Cell>(first, second);
        return p;
    }
    return first;
};
*/