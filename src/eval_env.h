#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include <unordered_map>
#include <vector>

#include "./value.h"
#include "./builtins.h"


using namespace std::literals;

class EvalEnv {
public:
    EvalEnv();
    ValuePtr eval(ValuePtr expr);
    void defineBinding(std::string& name, ValuePtr value);
    ValuePtr apply(ValuePtr proc, std::vector<ValuePtr> args);
    std::vector<ValuePtr> evalList(ValuePtr expr);
private:
    std::unordered_map<std::string, ValuePtr> symbolTable;

    ValuePtr evalSymbol(ValuePtr expr);
    ValuePtr evalPair(ValuePtr expr);
};

using SpecialFormType = std::shared_ptr<Value>(const std::vector<ValuePtr>&, EvalEnv&);

#endif
