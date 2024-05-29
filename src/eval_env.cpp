#include "./eval_env.h"
#include "./error.h"
#include "./forms.h"
#include <algorithm>
#include <iterator>

EvalEnv::EvalEnv() : parent(nullptr) {
    // 循环遍历 builtinProcs 并将所有的内置过程添加到符号表中
    for (const auto& proc : builtinProcs) {
        symbolTable[proc.first] = std::make_shared<BuiltinProcValue>(proc.second);
    }
}

std::shared_ptr<EvalEnv> EvalEnv::createChild(const std::vector<std::string>& params, const std::vector<ValuePtr>& args){
    if (args.size() != params.size()) throw LispError("arguments not matched");
    std::shared_ptr<EvalEnv> child{new EvalEnv};
    child->symbolTable.clear();
    child->parent = this->shared_from_this();
    for(int i = 0; i < params.size(); i++){
        child->defineBinding(params[i], args[i]);
    }
    return child;
}

void EvalEnv::defineBinding(const std::string& name, ValuePtr value) {
    symbolTable[name] = value;
}


ValuePtr EvalEnv::eval(ValuePtr expr) {
    if (expr->isSelfEvaluating()) {
        return expr;
    } else if (expr->isNil()) {
        throw LispError("Evaluating nil is prohibited.");
    } else if (expr->asSymbol()) {
        return evalSymbol(expr);
    } else if (expr->isPair()) { 
        return evalPair(expr);
    } else {
        throw LispError("Unimplemented");
    }
}

ValuePtr EvalEnv::lookupBinding(const std::string& name) {
    auto it = symbolTable.find(name);
    if (it != symbolTable.end()) {
        return it->second;
    } else if (parent) {
        return parent->lookupBinding(name); // 递归查找
    } else {
        throw LispError("Variable " + name + " not defined.");
    }
}
ValuePtr EvalEnv::evalSymbol(ValuePtr expr){
    if(expr->asSymbol()){
        return lookupBinding(expr->toString());
    } else {
        throw LispError("Unimplemented");
    }
}

ValuePtr EvalEnv::evalPair(ValuePtr expr){
    auto list = expr->toVector();
    while (list[0]->isPair()) list[0] = eval(list[0]);
    if (auto name = list[0]->asSymbol()) {
        if (SPECIAL_FORMS.contains(*name)) {
            return SPECIAL_FORMS.at(*name)(expr->CDR()->toVector(), *this);
        } else {
            ValuePtr proc = this->eval(list[0]);
            std::vector<ValuePtr> args = evalList(expr->CDR());
            return this->apply(proc, args);  
        }
    } else {
        ValuePtr proc = list[0];
        std::vector<ValuePtr> args = evalList(expr->CDR());
        return apply(proc, args);  
    }
}

ValuePtr EvalEnv::apply(ValuePtr proc, std::vector<ValuePtr> args){
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        // 调用内置过程
        return std::dynamic_pointer_cast<BuiltinProcValue>(proc)->getFunc()(args);
    } else if (typeid(*proc) == typeid(LambdaValue)) {
        return std::dynamic_pointer_cast<LambdaValue>(proc)->apply(args);
    } else {
        throw LispError("Unimplemented");
    }
}    

std::vector<ValuePtr> EvalEnv::evalList(ValuePtr expr) {
    std::vector<ValuePtr> result;
    std::ranges::transform(expr->toVector(),
                           std::back_inserter(result),
                           [this](ValuePtr v) { return this->eval(v); });
    return result;
}