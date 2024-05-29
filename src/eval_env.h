#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include <unordered_map>
#include <vector>

#include "./value.h"
#include "./builtins.h"


using namespace std::literals;

class EvalEnv : public std::enable_shared_from_this<EvalEnv>{
public:
    EvalEnv();
    ValuePtr eval(ValuePtr expr);
    void defineBinding(const std::string& name, ValuePtr value);
    ValuePtr apply(ValuePtr proc, std::vector<ValuePtr> args);
    std::vector<ValuePtr> evalList(ValuePtr expr);
    ValuePtr lookupBinding(const std::string& name);
    std::shared_ptr<EvalEnv> createChild(const std::vector<std::string>& params, const std::vector<ValuePtr>& args);
private:
    std::unordered_map<std::string, ValuePtr> symbolTable;
    std::shared_ptr<EvalEnv> parent;

    ValuePtr evalSymbol(ValuePtr expr);
    ValuePtr evalPair(ValuePtr expr);
};

using SpecialFormType = std::shared_ptr<Value>(const std::vector<ValuePtr>&, EvalEnv&);

#endif
