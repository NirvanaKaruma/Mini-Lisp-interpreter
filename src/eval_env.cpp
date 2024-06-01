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
    //特殊内置过程
    this->defineBinding(
        "eval",
        std::make_shared<BuiltinProcValue>([this](const std::vector<ValuePtr>& params) {
                                                return this->eval(params[0]);})
    );
    this->defineBinding(
        "apply",
        std::make_shared<BuiltinProcValue>([this](const std::vector<ValuePtr>& params) {
                                                auto a=params[1];
                                                return this->apply(params[0],a->toVector());})
    );    
    this->defineBinding(
        "map",
        std::make_shared<BuiltinProcValue>([this](const std::vector<ValuePtr>& params) {
                                                if(params.size() != 2) throw LispError("map takes 2 arguments");
                                                std::vector<ValuePtr> result;
                                                auto proc = params[0];
                                                auto values = params[1]->toVector();
                                                if(proc->isPair()){
                                                    throw LispError("Unimplemented, waiting for complement");
                                                } else {
                                                    for(int i = 0; i < values.size(); i++){
                                                        std::vector<ValuePtr> arg{values[i]};
                                                        result.emplace_back(this->apply(proc,arg));
                                                    }
                                                    return list(result);
                                                }
        })
    );
    this->defineBinding(
        "filter",
        std::make_shared<BuiltinProcValue>([this](const std::vector<ValuePtr>& params) {
                                                if(params.size() != 2) throw LispError("filter takes 2 arguments");
                                                std::vector<ValuePtr> result;
                                                auto proc = params[0];
                                                auto values = params[1]->toVector();
                                                if(proc->isPair()){
                                                    throw LispError("Unimplemented, waiting for complement");
                                                } else {
                                                    for(int i = 0; i < values.size(); i++){
                                                        if(this->apply(proc,{values[i]})->asBool()){
                                                            std::vector<ValuePtr> arg{values[i]};
                                                            result.emplace_back(values[i]);
                                                        }
                                                    }
                                                    return list(result);
                                                }
        })
    );
    this->defineBinding(
        "reduce",
        std::make_shared<BuiltinProcValue>([this](const std::vector<ValuePtr>& params){
                                                if(params.size() != 2) throw LispError("reduce takes 2 arguments");
                                                if(params[1]->isNil()) throw LispError("Cannot reduce nilvalue!");
                                                
                                                if(params[1]->toVector().size() == 1) return params[1]->toVector()[0];
                                                else {
                                                    auto proc = params[0];
                                                    auto p = params[1]->toVector();
                                                    auto v = *p.rbegin();
                                                    for (int i = p.size() - 2; i >= 0; i--) {
                                                        std::vector<ValuePtr> args{p[i], v};
                                                        v = this->apply(proc, args);
                                                    }        
                                                    return v;
                                                }
        })
    );
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