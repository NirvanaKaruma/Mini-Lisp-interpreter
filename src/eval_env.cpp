#include "./eval_env.h"
#include "./error.h"
#include "./forms.h"
#include <algorithm>
#include <iterator>

EvalEnv::EvalEnv() {
    // 循环遍历 builtinProcs 并将所有的内置过程添加到符号表中
    for (const auto& proc : builtinProcs) {
        symbolTable[proc.first] = std::make_shared<BuiltinProcValue>(proc.second);
    }
}

void EvalEnv::defineBinding(std::string& name, ValuePtr value) {
    symbolTable[name] = value;
}

ValuePtr EvalEnv::eval(ValuePtr expr){
    if (expr->isSelfEvaluating()){
        return expr;
    } else if (expr->isNil()){
        throw LispError("Evaluating nil is prohibited.");
    } else if (expr->asSymbol()){
        return evalSymbol(expr);
    } else if (expr->isPair()){
        return evalPair(expr);
    } else {
        throw LispError("Unimplemented");
    }
};

ValuePtr EvalEnv::evalSymbol(ValuePtr expr){
    auto name = expr->asSymbol();
    auto it = symbolTable.find(*name);
        if (it != symbolTable.end()){
            return it->second;
        } else {
             throw LispError("Variable " + *name + " not defined.");
        }
}

ValuePtr EvalEnv::evalPair(ValuePtr expr){
    if (expr->isNil()){
        return std::make_shared<NilValue>();
    }
    auto list = expr->toVector();
    if (auto pairExpr = std::dynamic_pointer_cast<PairValue>(expr); pairExpr != nullptr) {
            if (auto name = pairExpr->CAR()->asSymbol()){
                if(SPECIAL_FORMS.find(*name) != SPECIAL_FORMS.end()){
                    ValuePtr result = SPECIAL_FORMS.at(*name)(pairExpr->CDR()->toVector(),*this);
                    return result;
                } else {
                    auto proc = eval(list[0]);
                    if (pairExpr->CDR() != nullptr) {  // 添加空指针检查
                        auto cdrExpr = pairExpr->CDR();
                        std::vector<ValuePtr> args = evalList(cdrExpr);
                        return apply(proc, args);
                    } else {
                    throw LispError("Empty argument list in procedure call.");
                    }
                }  
            } else {
                throw LispError("Expected symbol but got something else.");
            }
        } else {
            throw LispError("Expected PairValue but got something else.");
        }
    throw LispError("Unimplemented");
}

ValuePtr EvalEnv::apply(ValuePtr proc, std::vector<ValuePtr> args){
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        // 调用内置过程
        return std::dynamic_pointer_cast<BuiltinProcValue>(proc)->getFunc()(args);
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