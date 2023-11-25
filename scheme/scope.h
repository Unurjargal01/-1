#pragma once
// Default variables and operations;

#include <map>
#include <parser.h>
#include <tokenizer.h>
#include <object.h>
#include <basic_functions.h>

class Scheme;

struct Scope {
    std::map<std::string, std::shared_ptr<Object>> scope_;
    Scope() {};
    
    void DefaultVariables() {
        // scope_["+"] 
        scope_["#t"] = std::make_shared<Symbol>("#t");
        scope_["#f"] = std::make_shared<Symbol>("#f");
    }
    std::shared_ptr<Object>& operator[](std::string symb) {
        return scope_[symb];
    }
    bool Find(std::string name) {
        return scope_.find(name) != scope_.end();
    }

    void GetScope(Scheme* scheme) {
        scheme_ = scheme;
        DefaultVariables();
    }

private:
    Scheme* scheme_ = nullptr;
};

struct Functions {
    std::map<std::string, std::shared_ptr<Function>> func_;
    Functions() {};

    void DefaultFunctions() {
        func_["+"] = std::make_shared<AddFunction>();
        func_["-"] = std::make_shared<MinusFunction>();
        func_["*"] = std::make_shared<MultiplicationFunction>();
        func_["/"] = std::make_shared<DivideFunction>();
        func_["max"] = std::make_shared<MaxFunction>();
        func_["min"] = std::make_shared<MinFunction>();
        func_["abs"] = std::make_shared<AbsFunction>();
        
        func_["="] = std::make_shared<EqualComparison>();
        func_["<"] = std::make_shared<LessComparison>();
        func_["<="] = std::make_shared<LessEqualComparison>();
        func_[">"] = std::make_shared<GreatComparison>();
        func_[">="] = std::make_shared<GreatEqualComparison>();

        func_["boolean?"] = std::make_shared<BooleanPredicate>();
        func_["number?"] = std::make_shared<NumberPredicate>();
        func_["symbol?"] = std::make_shared<SymbolPredicate>();

        func_["not"] = std::make_shared<NotFunction>();
        func_["and"] = std::make_shared<AndFunction>();
        func_["or"] = std::make_shared<OrFunction>();

        func_["if"] = std::make_shared<IfFunction>();
        func_["lambda"] = std::make_shared<LambdaFunctionCreater>();
    }
    std::shared_ptr<Function> operator[](std::string symb) {
        return func_[symb];
    }
    bool Find(std::string name) {
        return func_.find(name) != func_.end();
    }

    void GetFuncs(Scheme* scheme) {
        scheme_ = scheme;
        DefaultFunctions();
    }

private:
    Scheme* scheme_ = nullptr;
};