#pragma once

#include <string>
#include <map>

#include <scope.h>

class Scope;
class Functions;

class Scheme {
public:
    Scheme() {
        var_.GetScope(this);
        funcs_.GetFuncs(this);
    }
    std::string Evaluate(const std::string& expression);
    std::shared_ptr<Object> Evaluate(std::shared_ptr<Object> expression);
    std::shared_ptr<Object> EvaluateTemp(std::shared_ptr<Object> expression, std::map<std::string, std::shared_ptr<Object>> binds);

    std::shared_ptr<Object> GetVar(std::string name);
    bool FindVar(std::string name);
    std::shared_ptr<Object> GetFunc(std::string name);
private:
    Scope var_;
    Functions funcs_;
};

std::string ObjectValue(std::shared_ptr<Object> obj);
