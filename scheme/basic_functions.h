#pragma once

#include <memory>
#include <string>
#include <tokenizer.h>
#include <error.h>
#include <object.h>
#include <scheme.h>

class Scheme;
// Careful here; public std::enable_shared_from_this<Function>
class Function : public Object {
public:
    virtual ~Function() = default;
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Object>& args, Scheme& scheme) = 0;
};

// Predicates
class BooleanPredicate : public Function {
public:
    BooleanPredicate() {};
    std::shared_ptr<Object> operator()(std::shared_ptr<Object>& args, Scheme& scheme) override;
};

class NumberPredicate : public Function {
public:
    NumberPredicate() {};
    std::shared_ptr<Object> operator()(std::shared_ptr<Object>& args, Scheme& scheme) override;
};

class SymbolPredicate : public Function {
public:
    SymbolPredicate() {};
    std::shared_ptr<Object> operator()(std::shared_ptr<Object>& args, Scheme& scheme) override;
};

// Comparison assignment

class EqualComparison : public Function {
public:
    EqualComparison() {};
    std::shared_ptr<Object> operator()(std::shared_ptr<Object>& args, Scheme& scheme) override;
};
// Comparison >
class GreatComparison : public Function {
public:
    GreatComparison() {};
    std::shared_ptr<Object> operator()(std::shared_ptr<Object>& args, Scheme& scheme) override;
};
// Comparison >=
class GreatEqualComparison : public Function {
public:
    GreatEqualComparison() {};
    std::shared_ptr<Object> operator()(std::shared_ptr<Object>& args, Scheme& scheme) override;
};
// Comparison <
class LessComparison : public Function {
public:
    LessComparison() {};
    std::shared_ptr<Object> operator()(std::shared_ptr<Object>& args, Scheme& scheme) override;
};
// Comparison <=
class LessEqualComparison : public Function {
public:
    LessEqualComparison() {};
    std::shared_ptr<Object> operator()(std::shared_ptr<Object>& args, Scheme& scheme) override;
};


class NotFunction : public Function {
public:
    NotFunction() {};
    std::shared_ptr<Object> operator()(std::shared_ptr<Object>& args, Scheme& scheme) override;
};

class AndFunction : public Function {
public:
    AndFunction() {};
    std::shared_ptr<Object> operator()(std::shared_ptr<Object>& args, Scheme& scheme) override;
};

class OrFunction : public Function {
public:
    OrFunction() {};
    std::shared_ptr<Object> operator()(std::shared_ptr<Object>& args, Scheme& scheme) override;
};

// Operators
class AddFunction : public Function {
public:
    AddFunction(){};
    std::shared_ptr<Object> operator()(std::shared_ptr<Object>& args, Scheme& scheme) override;
};

class MinusFunction : public Function {
public:
    MinusFunction() {};
    std::shared_ptr<Object> operator()(std::shared_ptr<Object>& args, Scheme& scheme) override;

};

class MultiplicationFunction : public Function {
public:
    MultiplicationFunction() {};
    std::shared_ptr<Object> operator()(std::shared_ptr<Object>& args, Scheme& scheme) override;
};

class DivideFunction : public Function {
public:
    DivideFunction() {};
    std::shared_ptr<Object> operator()(std::shared_ptr<Object>& args, Scheme& scheme) override;
};

class MaxFunction : public Function {
public:
    MaxFunction() {};
    std::shared_ptr<Object> operator()(std::shared_ptr<Object>& args, Scheme& scheme) override;
};
class MinFunction : public Function {
public:
    MinFunction() {};
    std::shared_ptr<Object> operator()(std::shared_ptr<Object>& args, Scheme& scheme) override;
};

class AbsFunction : public Function {
public:
    AbsFunction() {};
    std::shared_ptr<Object> operator()(std::shared_ptr<Object>& args, Scheme& scheme) override;
};

// Define and Set and if

class IfFunction : public Function {
public:
    IfFunction() {};
    std::shared_ptr<Object> operator()(std::shared_ptr<Object>& args, Scheme& scheme) override;
};

// Lambda Function;
class LambdaFunctionCreater : public Function {
public:
    LambdaFunctionCreater() {};
    // For safety
    std::shared_ptr<Object> operator()(std::shared_ptr<Object>& args, Scheme& scheme) override;
};

class LambdaFunction : public Function {
public:
    LambdaFunction(std::vector<std::string> bind, std::shared_ptr<Object> body)
        : bind_(bind), body_(body) {};
    // For safety
    std::shared_ptr<Object> operator()(std::shared_ptr<Object>& args, Scheme& scheme) override;

private:
    std::vector<std::string> bind_;
    std::shared_ptr<Object> body_;
};