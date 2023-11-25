#include <memory>
#include <string>
#include <tokenizer.h>
#include <error.h>
#include <object.h>
#include <scheme.h>
#include <basic_functions.h>

class Scheme;
// Predicates
std::shared_ptr<Object> BooleanPredicate::operator()(std::shared_ptr<Object>& args, Scheme& scheme) {
    std::string ans = "#f";
    if (args == nullptr) { // args.size() == 0;
        throw RuntimeError();
    }
    std::shared_ptr<Object> current = scheme.Evaluate(As<Cell>(args)->first_);
    std::shared_ptr<Object> next = As<Cell>(args)->second_;
    if (next == nullptr) {
        if (Is<Symbol>(current)) {
            std::string name = As<Symbol>(current)->GetName();
            ans = (name == "#t" || name == "#f") ? "#t" : "#f";
        }
    } else {
        throw RuntimeError();
    }
    return std::make_shared<Symbol>(ans);
};

std::shared_ptr<Object> NumberPredicate::operator()(std::shared_ptr<Object>& args, Scheme& scheme) {
    std::string ans = "#f";
    if (args == nullptr) { // args.size() == 0;
        throw RuntimeError();
    }
    std::shared_ptr<Object> current = scheme.Evaluate(As<Cell>(args)->first_);
    std::shared_ptr<Object> next = As<Cell>(args)->second_;
    if (next == nullptr) {
        ans = (Is<Number>(current)) ? "#t" : "#f";
    } else {
        throw RuntimeError();
    }
    return std::make_shared<Symbol>(ans);
};

std::shared_ptr<Object> SymbolPredicate::operator()(std::shared_ptr<Object>& args, Scheme& scheme) {
    std::string ans = "#f";
    if (args == nullptr) { // args.size() == 0;
        throw RuntimeError();
    }
    std::shared_ptr<Object> current = scheme.Evaluate(As<Cell>(args)->first_);
    std::shared_ptr<Object> next = As<Cell>(args)->second_;
    if (next == nullptr) {
        if (Is<Quote>(current)) {

            ans = (As<Quote>(current)->GetName()[0] != '\'') ? "#t" : "#f";;
        } else if (Is<Symbol>(current)) {
            std::string symb = As<Symbol>(current)->GetName();
            
            if (symb != "*" && symb != "/" && symb != "+" && symb != "-" && symb != "#t" && symb != "#f") {
                ans = "#t";
            }
        }
    } else {
        throw RuntimeError();
    }
    return std::make_shared<Symbol>(ans);
};

std::shared_ptr<Object> EqualComparison::operator()(std::shared_ptr<Object>& args, Scheme& scheme) {
    std::string ans = "#t";
    int val = INT16_MIN;
    if (args == nullptr) {  // args.size() == 0;
        return std::make_shared<Symbol>(ans);
    } 
    std::shared_ptr<Object> next = args;
    if (As<Cell>(next)->second_ == nullptr) {
        throw RuntimeError();
    }

    while (next != nullptr) {
        std::shared_ptr<Object> current = scheme.Evaluate(As<Cell>(next)->first_);
        next = As<Cell>(next)->second_;
        if (!Is<Number>(current)) {
            throw RuntimeError();
        }
        std::shared_ptr<Number> number = As<Number>(current);

        if (val == INT16_MIN) {
            val = number->GetValue();
            continue;
        }
        ans = (val == number->GetValue()) ? "#t" : "#f";
        val = number->GetValue();
    }
    return std::make_shared<Symbol>(ans);

};
std::shared_ptr<Object> GreatComparison::operator()(std::shared_ptr<Object>& args, Scheme& scheme) {
    std::string ans = "#t";
    int val = INT16_MIN;
    if (args == nullptr) {  // args.size() == 0;
        return std::make_shared<Symbol>(ans);
    } 
    std::shared_ptr<Object> next = args;
    if (As<Cell>(next)->second_ == nullptr) {
        throw RuntimeError();
    }
    while (next != nullptr) {
        std::shared_ptr<Object> current = scheme.Evaluate(As<Cell>(next)->first_);
        next = As<Cell>(next)->second_;
        if (!Is<Number>(current)) {
            throw RuntimeError();
        }
        std::shared_ptr<Number> number = As<Number>(current);

        if (val == INT16_MIN) {
            val = number->GetValue();
            continue;
        }
        ans = (val > number->GetValue()) ? "#t" : "#f";
        val = number->GetValue();
    }
    return std::make_shared<Symbol>(ans);
};
std::shared_ptr<Object> GreatEqualComparison::operator()(std::shared_ptr<Object>& args, Scheme& scheme) {
    std::string ans = "#t";
    int val = INT16_MIN;
    if (args == nullptr) {  // args.size() == 0;
        return std::make_shared<Symbol>(ans);
    } 
    std::shared_ptr<Object> next = args;
    if (As<Cell>(next)->second_ == nullptr) {
        throw RuntimeError();
    }

    while (next != nullptr) {
        std::shared_ptr<Object> current = scheme.Evaluate(As<Cell>(next)->first_);
        next = As<Cell>(next)->second_;
        if (!Is<Number>(current)) {
            throw RuntimeError();
        }
        std::shared_ptr<Number> number = As<Number>(current);

        if (val == INT16_MIN) {
            val = number->GetValue();
            continue;
        }
        ans = (val >= number->GetValue()) ? "#t" : "#f";
        val = number->GetValue();
    }
    return std::make_shared<Symbol>(ans);
};
std::shared_ptr<Object> LessComparison::operator()(std::shared_ptr<Object>& args, Scheme& scheme) {
    std::string ans = "#t";
    int val = INT16_MIN;
    if (args == nullptr) {  // args.size() == 0;
        return std::make_shared<Symbol>(ans);
    } 
    std::shared_ptr<Object> next = args;
    if (As<Cell>(next)->second_ == nullptr) {
        throw RuntimeError();
    }

    while (next != nullptr) {
        std::shared_ptr<Object> current = scheme.Evaluate(As<Cell>(next)->first_);
        next = As<Cell>(next)->second_;
        if (!Is<Number>(current)) {
            throw RuntimeError();
        }
        std::shared_ptr<Number> number = As<Number>(current);

        if (val == INT16_MIN) {
            val = number->GetValue();
            continue;
        }
        ans = (val < number->GetValue()) ? "#t" : "#f";
        val = number->GetValue();
    }
    return std::make_shared<Symbol>(ans);
};
std::shared_ptr<Object> LessEqualComparison::operator()(std::shared_ptr<Object>& args, Scheme& scheme) {
    std::string ans = "#t";
    int val = INT16_MIN;
    if (args == nullptr) {  // args.size() == 0;
        return std::make_shared<Symbol>(ans);
    } 
    std::shared_ptr<Object> next = args;
    if (As<Cell>(next)->second_ == nullptr) {
        throw RuntimeError();
    }

    while (next != nullptr) {
        std::shared_ptr<Object> current = scheme.Evaluate(As<Cell>(next)->first_);
        next = As<Cell>(next)->second_;
        if (!Is<Number>(current)) {
            throw RuntimeError();
        }
        std::shared_ptr<Number> number = As<Number>(current);

        if (val == INT16_MIN) {
            val = number->GetValue();
            continue;
        }
        ans = (val <= number->GetValue()) ? "#t" : "#f";
        val = number->GetValue();
    }
    return std::make_shared<Symbol>(ans);
};

std::shared_ptr<Object> NotFunction::operator()(std::shared_ptr<Object>& args, Scheme& scheme) {
    std::string ans = "#f";
    if (args == nullptr) {  // args.size() == 0;
        throw RuntimeError();
    }
    std::shared_ptr<Object> next = args;
    if (As<Cell>(next)->second_ == nullptr) {
        std::shared_ptr<Object> current = scheme.Evaluate(As<Cell>(next)->first_);
        if (Is<Symbol>(current)) {
            std::string name = As<Symbol>(current)->GetName();
            ans = (name == "#f") ? "#t" : "#f";
            std::cout << "NotFunction: " << ans << std::endl;
        }
    } else {
        throw RuntimeError();
    }
    return std::make_shared<Symbol>(ans);
};

std::shared_ptr<Object> AndFunction::operator()(std::shared_ptr<Object>& args, Scheme& scheme) {
    std::string ans = "#t";
    if (args != nullptr) {
        std::shared_ptr<Object> current;
        std::shared_ptr<Object> next = args;
        while (next != nullptr) {
            current = scheme.Evaluate(As<Cell>(next)->first_);
            next = As<Cell>(next)->second_;
            if (Is<Symbol>(current)) {
                if (As<Symbol>(current)->GetName() == "#f"){
                    ans = "#f";
                break;
                }
            }
        }
        if (ans == "#t") {
            ans = ObjectValue(current);
        }
    }
    return std::make_shared<Symbol>(ans);
};

std::shared_ptr<Object> OrFunction::operator()(std::shared_ptr<Object>& args, Scheme& scheme) {
    std::string ans = "#f";
    if (args != nullptr) {
        std::shared_ptr<Object> current;
        std::shared_ptr<Object> next = args;
        while (next != nullptr) {
            current = scheme.Evaluate(As<Cell>(next)->first_);
            next = As<Cell>(next)->second_;
            if (Is<Symbol>(current)) {
                if (As<Symbol>(current)->GetName() == "#t"){
                    ans = "#t";
                break;
                }
            }
        }
        if (ans == "#f") {
            ans = ObjectValue(current);
        }
    }
    return std::make_shared<Symbol>(ans);
};

// Operators
std::shared_ptr<Object> AddFunction::operator()(std::shared_ptr<Object>& args, Scheme& scheme) {
    int ans = 0;
    std::shared_ptr<Object> current;
    std::shared_ptr<Object> next = args;
    while (next != nullptr) {
        current = scheme.Evaluate(As<Cell>(next)->first_);
        next = As<Cell>(next)->second_;
        if (Is<Symbol>(current)) {
            if (scheme.FindVar(As<Symbol>(current)->GetName())) {
                current = scheme.GetVar(As<Symbol>(current)->GetName());
            } else if (!Is<Number>(current)) {
                throw RuntimeError();
            }
        }
        ans += As<Number>(current)->GetValue();
    }
    std::cout << ans << std::endl;
    return std::make_shared<Number>(ans);
};

std::shared_ptr<Object> MinusFunction::operator()(std::shared_ptr<Object>& args, Scheme& scheme) {
    int ans = INT16_MIN;
    std::shared_ptr<Object> current;
    std::shared_ptr<Object> next = args;
    if (next == nullptr) {
        throw RuntimeError();
    }
    while (next != nullptr) {
        current = scheme.Evaluate(As<Cell>(next)->first_);
        next = As<Cell>(next)->second_;
        if (!Is<Number>(current)) {
            throw RuntimeError();
        }
        if (ans == INT16_MIN) {
            ans = As<Number>(current)->GetValue();
            continue;
        }
        ans -= As<Number>(current)->GetValue();
    }

    return std::make_shared<Number>(ans);
};

std::shared_ptr<Object> MultiplicationFunction::operator()(std::shared_ptr<Object>& args, Scheme& scheme) {
    int ans = 1;
    std::shared_ptr<Object> current;
    std::shared_ptr<Object> next = args;
    while (next != nullptr) {
        current = scheme.Evaluate(As<Cell>(next)->first_);
        next = As<Cell>(next)->second_;
        if (!Is<Number>(current)) {
            throw RuntimeError();
        }
        ans *= As<Number>(current)->GetValue();
    }

    return std::make_shared<Number>(ans);
};

std::shared_ptr<Object> DivideFunction::operator()(std::shared_ptr<Object>& args, Scheme& scheme) {
    int ans = INT16_MIN;
    std::shared_ptr<Object> current;
    std::shared_ptr<Object> next = args;
    if (next == nullptr) {
        throw RuntimeError();
    }
    while (next != nullptr) {
        current = scheme.Evaluate(As<Cell>(next)->first_);
        next = As<Cell>(next)->second_;
        if (!Is<Number>(current)) {
            throw RuntimeError();
        }
        if (ans == INT16_MIN) {
            ans = As<Number>(current)->GetValue();
            continue;
        }
        try {
            ans /= As<Number>(current)->GetValue();
        } catch (...) {
            throw SyntaxError();
        }   
    }
    return std::make_shared<Number>(ans);
};

std::shared_ptr<Object> MaxFunction::operator()(std::shared_ptr<Object>& args, Scheme& scheme) {
    int ans = INT16_MIN;
    std::shared_ptr<Object> current;
    std::shared_ptr<Object> next = args;
    if (next == nullptr) {
        throw RuntimeError();
    }
    while (next != nullptr) {
        current = scheme.Evaluate(As<Cell>(next)->first_);
        next = As<Cell>(next)->second_;
        if (!Is<Number>(current)) {
            throw RuntimeError();
        }
        if (ans == INT16_MIN) {
            ans = As<Number>(current)->GetValue();
            continue;
        }
        ans = std::max(ans, As<Number>(current)->GetValue());
    }

    return std::make_shared<Number>(ans);
};

std::shared_ptr<Object> MinFunction::operator()(std::shared_ptr<Object>& args, Scheme& scheme) {
    int ans = INT16_MIN;
    std::shared_ptr<Object> current;
    std::shared_ptr<Object> next = args;
    if (next == nullptr) {
        throw RuntimeError();
    }
    while (next != nullptr) {
        current = scheme.Evaluate(As<Cell>(next)->first_);
        next = As<Cell>(next)->second_;
        if (!Is<Number>(current)) {
            throw RuntimeError();
        }
        if (ans == INT16_MIN) {
            ans = As<Number>(current)->GetValue();
            continue;
        }
        ans = std::min(ans, As<Number>(current)->GetValue());
    }

    return std::make_shared<Number>(ans);
};
std::shared_ptr<Object> AbsFunction::operator()(std::shared_ptr<Object>& args, Scheme& scheme) {
    int ans = INT16_MIN;
    if (args == nullptr) { // args.size() == 0;
        throw RuntimeError();
    }
    std::shared_ptr<Object> current = scheme.Evaluate(As<Cell>(args)->first_);
    std::shared_ptr<Object> next = As<Cell>(args)->second_;
    if (next == nullptr) {
        if (!Is<Number>(current)) {
            throw RuntimeError();
        }
        ans = std::abs(As<Number>(current)->GetValue());
    } else {
        throw RuntimeError();
    }
    return std::make_shared<Number>(ans);
};

std::shared_ptr<Object> IfFunction::operator()(std::shared_ptr<Object>& args, Scheme& scheme) {
    std::shared_ptr<Object> ans;
    std::shared_ptr<Object> current;
    std::shared_ptr<Object> next = args;
    if (next == nullptr) {
        throw SyntaxError();
    }
    // Condition
    current = scheme.Evaluate(As<Cell>(next)->first_);
    next = As<Cell>(next)->second_; // next1
    if (!Is<Symbol>(current)) {
        throw SyntaxError();
    } else {
        std::string val = As<Symbol>(current)->GetName();
        if (val == "#t") {
            ans = scheme.Evaluate(As<Cell>(next)->first_); // val1
            // If it is variable
            if (Is<Symbol>(ans)) {
                std::string name = As<Symbol>(ans)->GetName();
                ans = scheme.GetVar(name);
            }
            if (As<Cell>(next)->second_ != nullptr) {
                next = As<Cell>(next)->second_; // val2
                if (As<Cell>(next)->second_ != nullptr) {
                    throw SyntaxError();
                }
            }
        } else if (val == "#f") {
            next = As<Cell>(next)->second_; // val2
            if (next == nullptr) {
                ans = std::make_shared<Symbol>("()");
            } else {
                ans = scheme.Evaluate(As<Cell>(next)->first_);
                if (Is<Symbol>(ans)) {
                    std::string name = As<Symbol>(ans)->GetName();
                    ans = scheme.GetVar(name);
                }
                if (As<Cell>(next)->second_ != nullptr) {
                    throw SyntaxError();
                }
            }
        } else {
            throw SyntaxError();
        }
    }
    return ans;
}
std::shared_ptr<Object> LambdaFunctionCreater::operator()(std::shared_ptr<Object>& args, Scheme& scheme) {
    std:: cout << "Cell: " << Is<Cell>(args) <<std::endl;
    if (args == nullptr) {
        
        throw SyntaxError();
    } 
    /*
    else if (As<Cell>(args)->second_ == nullptr) {
        std::cout << "FAILED: " << std::endl;
        throw SyntaxError();
    }
    */
    std::shared_ptr<Cell> lambdaExpression = As<Cell>(args);
    std::shared_ptr<Object> parameters = lambdaExpression->first_;
    std::shared_ptr<Object> body = lambdaExpression->second_;
    std::vector<std::string> bindings;
    std::shared_ptr<Object> par;
    
    while (parameters != nullptr) {
        std::cout << "FAILED: " << std::endl;
        
        par = As<Cell>(parameters)->first_;
        parameters = As<Cell>(parameters)->second_;
        if (!Is<Symbol>(par)) {
            throw RuntimeError();
        }
        std::cout << As<Symbol>(par)->GetName() << std::endl;
        bindings.push_back(As<Symbol>(par)->GetName());
    }

    auto func = std::make_shared<LambdaFunction>(bindings, body);
    std::cout << "Lambda Function Created" << std::endl;
    
    return func;
};

std::shared_ptr<Object> LambdaFunction::operator() (std::shared_ptr<Object>& args, Scheme& scheme) {
    std::shared_ptr<Object> current, next = args;
    std::map<std::string, std::shared_ptr<Object>> binds;
    std::vector<std::shared_ptr<Object>> pars;
    
    
    while (next != nullptr) {
        current = As<Cell>(next)->first_;
        next = As<Cell>(next)->second_;

        if (!Is<Number>(current)) {
            throw RuntimeError();
        }
        pars.push_back(current);
    }
    if (bind_.size() != pars.size()) {
        throw SyntaxError();
    }
    for (size_t i = 0; i < bind_.size(); ++i) {
        binds[bind_[i]] = pars[i];
    }
    std::cout << "Lambda Evaluating" << std::endl;
    std::shared_ptr<Object> eval = scheme.EvaluateTemp(body_, binds);
    std::cout << "Lambda Failed" << std::endl;
    return eval;
};
