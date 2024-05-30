#include "./forms.h"
#include <algorithm>
#include <iterator>


std::string LambdaValue::toString() const {
    return "#<procedure>";
}

ValuePtr LambdaValue::apply(const std::vector<ValuePtr>& args) const{
    auto kid = this->env->createChild(this->params, args);
    ValuePtr result;
    for (const auto& i : this->body) result = kid->eval(i);
    return result;
}

ValuePtr lambdaForm(const std::vector<ValuePtr>& args, EvalEnv& env){
    if (args.size() < 2) {
        throw LispError("Invalid number of arguments for lambda");
    }
    if (args[0]->isPair() == false || args[1]->isPair() == false){
        throw LispError("Lambda parameters must be lists");
    } 
    // 第一个参数是参数列表，第二个参数是过程体
    auto params = args[0]->toVector();
    std::vector<std::string> paramNames;
    std::transform(params.begin(), params.end(), std::back_inserter(paramNames), [](ValuePtr i){return i->toString();});
    std::vector<ValuePtr> body(args.begin() + 1, args.end());
    return std::make_shared<LambdaValue>(paramNames, body, env.shared_from_this());
}

ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() < 2) {
        throw LispError("Invalid number of arguments for define");
    }
    auto first = args[0];
    if(first->asSymbol()){
        auto name = first->asSymbol();
        env.defineBinding(*name, env.eval(args[1]));
        return std::make_shared<NilValue>();
    } else if (first->isPair()){
        auto name = first->CAR()->toString();
        std::vector<ValuePtr> values{first->CDR()};
        for (int i = 1; i < args.size(); i++) values.emplace_back(args[i]);
        env.defineBinding(name, lambdaForm(values, env));
        return std::make_shared<NilValue>();
    } else {
        throw LispError("Unimplemented");
    }

}

ValuePtr quoteForm(const std::vector<ValuePtr>& args, EvalEnv& env){
    if (args.size() != 1) {
        throw LispError("Invalid number of arguments for quote");
    }
    return args[0];
}

ValuePtr ifForm(const std::vector<ValuePtr>& args, EvalEnv& env){
    if (args.size() != 3) {
        throw LispError("Invalid number of arguments for if");
    }
    ValuePtr cond = env.eval(args[0]);
    if(cond->asBool() == true){
        return env.eval(args[1]);
    } else {
        return env.eval(args[2]);
    }
}

ValuePtr andForm(const std::vector<ValuePtr>& args, EvalEnv& env){
    for(auto& arg : args){
        ValuePtr cond = env.eval(arg);
        if(cond->asBool() == false){
            return cond;
        }
    }
    return args.empty() ? std::make_shared<BooleanValue>(true) : env.eval(args.back());
}

ValuePtr orForm(const std::vector<ValuePtr>& args, EvalEnv& env){
    for(auto& arg : args){
        ValuePtr cond = env.eval(arg);
        if(cond->asBool() == true){
            return cond;
        }
    }
    return args.empty() ? std::make_shared<BooleanValue>(false) : env.eval(args.back());
}

ValuePtr condForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() >= 2) {
        ValuePtr result;
        for (int i = 0; i < args.size(); i++) {
            auto relation = args[i]->toVector();
            if (relation[0]->toString() == "else"){
                if(i != args.size() - 1) throw LispError("Invalid else position");
                return env.eval(relation[1]);
            }
            if (relation.size() == 1) return env.eval(relation[0]);
            if (env.eval(relation[0])->asBool()) {
                result = env.eval(relation[1]);
                break;
            }
        }
        return result;
    } else
        throw LispError("Invalid number of arguments for cond");
}

const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS{
    {"lambda",lambdaForm},
    {"define", defineForm},
    {"quote", quoteForm},
    {"if", ifForm},
    {"and", andForm},
    {"or", orForm},
    {"cond", condForm}
};