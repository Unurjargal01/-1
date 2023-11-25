#include <scheme.h>

#include <stdexcept>
#include <string>
#include <object.h>
#include <cassert>
#include <parser.h>
#include <error.h>

std::string Scheme::Evaluate(const std::string& expression) {
    std::cout << expression << std::endl;
    std::stringstream ss{std::move(expression)};
    Tokenizer tokenizer{&ss};
    std::shared_ptr<Object> obj = Read(&tokenizer);
    // One line;
    if (Is<Quote>(obj)) {
        // If List then we need to do it differently.
        return As<Quote>(obj)->GetName();
    } else if (Is<Number>(obj)) {
        return std::to_string(As<Number>(obj)->GetValue());
    } else if (Is<Symbol>(obj)) {
        // Symbol "#t" or "name" if name is defined before;
        std::string ans = As<Symbol>(obj)->GetName();
        if (var_.Find(ans)) {
            return ObjectValue(var_[ans]);
        } else if (funcs_.Find(ans)) {
            return "Function";
        }
        else {
            throw NameError(As<Symbol>(obj)->GetName());
        }

    } else if (Is<Cell>(obj)) { // Cell Like functions;
        std::cout << "Returned" << std::endl;
        std::shared_ptr<Object> ans = Evaluate(obj);
        std::cout << "Returned" << std::endl;
        return ObjectValue(ans);
    } else {
        throw SyntaxError(); 
    }
    throw std::runtime_error{"Not implemented"};
};

std::shared_ptr<Object> Scheme::Evaluate(std::shared_ptr<Object> expression) {
    std::shared_ptr<Object> result; 
    // Consider it as final answer; "'3" or "3"
    if (Is<Number>(expression) || Is<Quote>(expression) || Is<Symbol>(expression) || expression == nullptr) {
        return expression;
    }
    // If Our expression is Cell like (+ 3 4)
    std::shared_ptr<Object> first = Evaluate(As<Cell>(expression)->first_);

    // Change is here
    std::shared_ptr<Object> current = As<Cell>(expression)->second_;
    // std::cout << (Is<Cell>(expression)) << std::endl;

    if (Is<Symbol>(first)) {
        // Function operation
        std::string name_func = As<Symbol>(first)->GetName();
        std::cout << name_func << std::endl;
        if (funcs_.Find(name_func)) {
            std::shared_ptr<Function> func = funcs_[name_func];
            std::cout << "Processing" << std::endl;
            // Possible problem is (1 . 2);

            result = (*func)(current, *this);
            std::cout << "Succeeded" << std::endl;
        } else if (name_func == "set!") {
            std::vector<std::shared_ptr<Object>> args;
            while (current != nullptr) {
                args.push_back(Evaluate(As<Cell>(current)->first_));
                current = As<Cell>(current)->second_;
            }
            if (args.size() != 2 || !Is<Symbol>(args[0])) {
                throw SyntaxError();
            }
            std::string name_var = As<Symbol>(args[0])->GetName();
            if (!var_.Find(name_var)) {
                throw NameError(name_var);
            }
            var_[name_var] = args[1];
            result = args[1];
        } else if (name_func == "define") {

            // Define Lambda Sugar
            std::vector<std::shared_ptr<Object>> args;
            while (current != nullptr) {
                args.push_back(Evaluate(As<Cell>(current)->first_));
                current = As<Cell>(current)->second_;
            }
            std::cout << "Defined: " << std::endl;
            if(args.size() != 2) {
                throw SyntaxError();
            }
            std::string name_var = As<Symbol>(args[0])->GetName();
            std::cout << "name_var: " << name_var << std::endl;
            if (Is<Symbol>(args[1]) || Is<Number>(args[1]) || Is<Quote>(args[1])) {
                var_[name_var] = args[1];
                result = args[1];
            } else {
                funcs_[name_var] = As<Function>(args[1]);
                result = std::make_shared<Symbol>("Function");
            }
            std::cout << args.size() << std::endl;
            std::cout << "Succeeded" << std::endl;
        }
    } else if (Is<Function>(first)) {
        std::cout << "Function" << std::endl;
        return (*As<LambdaFunction>(first))(current, *this);
       // Possible error (1 2 3);
    } else {
        throw RuntimeError();
    }
    std::cout << "Returned Eval" << std::endl;
    std::cout << Is<Number>(result) << std::endl;
    
    return result;
}
std::shared_ptr<Object> Scheme::EvaluateTemp(std::shared_ptr<Object> expression, std::map<std::string, std::shared_ptr<Object>> binds) {
    std::cout << "Temp Reached" << std::endl;
    Scheme scheme_temp = *this;
    for (auto [k, v] : binds) {
        scheme_temp.var_[k] = v;
    }
    std::cout << "Temp Evaluating" << std::endl;
    auto temp_val = scheme_temp.Evaluate(expression);
    std::cout << "Temp Evaluated" << std::endl;
    return temp_val;
}
// String returner
std::string ObjectValue(std::shared_ptr<Object> obj) {
    if (Is<Quote>(obj)) {
        return As<Quote>(obj)->GetName();
    } else if (Is<Number>(obj)) {
        return std::to_string(As<Number>(obj)->GetValue());
    } else if (Is<Symbol>(obj)) {
        return As<Symbol>(obj)->GetName();
    } else {
        throw RuntimeError();
    }
    return "";
};

std::shared_ptr<Object> Scheme::GetVar(std::string name) {
    if (var_.Find(name)) {
        return var_[name];
    } else {
        throw NameError(name);
    }
}
bool Scheme::FindVar(std::string name) {
    return var_.Find(name);
}
std::shared_ptr<Object> Scheme::GetFunc(std::string name) {
    if (funcs_.Find(name)) {
        return funcs_[name];
    } else {
        throw NameError(name);
    }
}