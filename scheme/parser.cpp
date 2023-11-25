#include <parser.h>
#include <object.h>
#include <tokenizer.h>
#include <error.h>
#include <memory>

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer);

std::shared_ptr<Object> ReadQuote(Tokenizer* tokenizer);

std::shared_ptr<Object> Read(Tokenizer* tokenizer) {
    std::shared_ptr<Object> first;
    if (tokenizer->IsEnd()) {
        return first;
    }
    Token first_token = tokenizer->GetToken();
    // Shift
    tokenizer->Next();
    if (std::holds_alternative<ConstantToken>(first_token)) {
        first = std::make_shared<Number>(std::get<ConstantToken>(first_token).value);
    } else if (std::holds_alternative<SymbolToken>(first_token)) {
        first = std::make_shared<Symbol>(std::get<SymbolToken>(first_token).name);
    } else if (std::holds_alternative<QuoteToken>(first_token)) {

        // QUOTE I think it is fine here.
        first = ReadQuote(tokenizer, 0);

    } else if (std::holds_alternative<BracketToken>(first_token)) {
        auto bracket = std::get<BracketToken>(first_token);
        if (bracket == BracketToken::CLOSE) {
            throw SyntaxError();
        }
        first = ReadList(tokenizer);
    } else {
        throw SyntaxError();
    }
    // Maybe here we should move;
    if (!tokenizer->IsEnd()) {
        throw SyntaxError();
    }
    return first;
};

// ReadAny;
std::shared_ptr<Object> ReadCell(Tokenizer* tokenizer) {
    // std::cout << "Read Cell Started: " << std::endl;
    std::shared_ptr<Object> current, next;
    // bool current_state = tokenizer->IsEnd();
    Token cur_token = tokenizer->GetToken();
    tokenizer->Next();
    Token next_token = tokenizer->GetToken();
    // Current, ?? if it is our guess is right, we shouldn't encounter the dot in current state.
    if (std::holds_alternative<ConstantToken>(cur_token)) {
        current = std::make_shared<Number>(std::get<ConstantToken>(cur_token).value);
        // std::cout << std::get<ConstantToken>(cur_token).value << std::endl;
    } else if (std::holds_alternative<SymbolToken>(cur_token)) {
        current = std::make_shared<Symbol>(std::get<SymbolToken>(cur_token).name);
        // std::cout << std::get<SymbolToken>(cur_token).name << std::endl;
    } else if (std::holds_alternative<QuoteToken>(cur_token)) {
        // why here ''#f finishes at '
        current = ReadQuote(tokenizer, 0);



    } else if (std::holds_alternative<BracketToken>(cur_token)) {
        auto bracket = std::get<BracketToken>(cur_token);
        if (bracket == BracketToken::OPEN) {
            current = ReadList(tokenizer);
            // what would happend to next_token; // Should I change next_token.
            // tokenizer->Next();
            next_token = tokenizer->GetToken();
        } else {
            current = std::make_shared<BracketClosed>();
            // std::cout << "Read: BrackedClosed" << std::endl;
            return current;
        }
    } else if (std::holds_alternative<DotToken>(cur_token)) {
        // std::cout << "FAILED: DotToken" << std::endl;
        throw SyntaxError();
    }
    // Next;
    if (std::holds_alternative<DotToken>(next_token)) {
        // std::cout << "Read: DOTTOKEN" << std::endl;
        tokenizer->Next();
        next_token = tokenizer->GetToken();
        if (std::holds_alternative<ConstantToken>(next_token)) {
            next = std::make_shared<Number>(std::get<ConstantToken>(next_token).value);
            tokenizer->Next();
        } else if (std::holds_alternative<SymbolToken>(next_token)) {
            next = std::make_shared<Symbol>(std::get<SymbolToken>(next_token).name);
            tokenizer->Next();
        } else if (std::holds_alternative<BracketToken>(next_token)) {
            auto bracket = std::get<BracketToken>(next_token);
            if (bracket == BracketToken::OPEN) {
                tokenizer->Next();
                next = ReadList(tokenizer);
            } else {
                // std::cout << "Failed: Next ClosedBracket" << std::endl;
                throw SyntaxError();
            }
        } else {
            // std::cout << "FAILED: Next" << std::endl;
            throw SyntaxError();
        }
        // Here Careful
    }
    if (next != nullptr) {
        // std::cout << "Dot Sucessed" << std::endl;
        current = std::make_shared<Cell>(current, next);
    } else if (Is<Cell>(current)) {  // Change is here.
        // std::cout << "FAILED CELL" << std::endl;
        current = std::make_shared<Cell>(current, nullptr);
    }
    // std::cout << "ReadCell finished: " << std::endl;
    return current;
}

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer) {
    // std::cout << "ReadList started: " << std::endl;
    std::shared_ptr<Cell> ans;
    std::shared_ptr<Cell> ptr_second;
    std::shared_ptr<Object> elem = ReadCell(tokenizer);
    // Maybe if tokenizer->IsEnd();
    while (!Is<BracketClosed>(elem)) {
        // Maybe the last element is bracket?
        if (tokenizer->IsEnd()) {
            // std::cout << "FAILED: FINISHED" << std::endl;
            throw SyntaxError();
        }
        if (ans == nullptr) {
            // Added here QUOTE
            if (Is<Symbol>(elem)) {
                if (As<Symbol>(elem)->GetName() == "quote") {
                    // Why the heck ah
                    return ReadQuote(tokenizer, 1);
                } else {
                    ans = std::make_shared<Cell>(elem, nullptr);
                }
            } else if (Is<Number>(elem)) {
                ans = std::make_shared<Cell>(elem, nullptr);
                // std::cout << "Assigned: " << std::endl;
            } else if (Is<Cell>(elem)) {
                ans = As<Cell>(elem);
                // We can break here if we want;
            }
            ptr_second = ans;
        } else if (ptr_second->second_ == nullptr) {
            if (Is<Number>(elem) || Is<Symbol>(elem) || Is<Quote>(elem)) {
                ptr_second->second_ = std::make_shared<Cell>(elem, nullptr);
                ptr_second = As<Cell>(ptr_second->second_);
            } else if (Is<Cell>(elem)) {
                // Our Guess;
                // std::cout << "FAILED GUESS" << std::endl;
                ptr_second->second_ = elem;
                ptr_second = As<Cell>(ptr_second->second_);
                // What if it is not Cell the second part
            }
        } else {
            // std::cout << "FAILED: Assignment while" << std::endl;
            throw SyntaxError();
        }
        elem = ReadCell(tokenizer);
    };
    // std::cout << "ReadList finished: " << std::endl;
    return ans;
}

std::shared_ptr<Object> ReadQuote(Tokenizer* tokenizer, int number_open = 0) {
    // std::cout << "ReadQuote started: " << std::endl;
    std::string ans;
    Token current;
    bool last_bracket = (number_open == 0);
    do {
        // std::cout << number_open << std::endl;
        current = tokenizer->GetToken();
        tokenizer->Next();
        if (std::holds_alternative<ConstantToken>(current)) {
            ans += std::to_string(std::get<ConstantToken>(current).value) + " ";
        } else if (std::holds_alternative<SymbolToken>(current)) {
            ans += std::get<SymbolToken>(current).name + " ";
        } else if (std::holds_alternative<BracketToken>(current)) {
            // std::cout << "BRACKETS" << std::endl;
            auto bracket = std::get<BracketToken>(current);
            if (bracket == BracketToken::OPEN) {
                ans += "(";
                number_open += 1;
            } else {
                number_open -= 1;
                if (number_open != 0 || (number_open == 0 && last_bracket)) {
                    if (ans.back() == ' ') {
                        ans.pop_back();                        
                    }
                    ans += ") ";
                }
            }
        } else if (std::holds_alternative<DotToken>(current)) {
            ans += ". ";
        } else if (std::holds_alternative<QuoteToken>(current)) {
            ans += "'" + As<Quote>(ReadQuote(tokenizer, number_open))->GetName();
        } else {
            throw SyntaxError();
        }

    } while (number_open > 0 && !tokenizer->IsEnd());

    if (ans.back() == ' ') {
        ans.pop_back();                        
    }
    // std::cout << number_open << std::endl;
    // std::cout << "ReadQuote finished: " << ans << std::endl;   
    if (number_open != 0) {
        throw SyntaxError();
    }

    return std::make_shared<Quote>(ans);
}
