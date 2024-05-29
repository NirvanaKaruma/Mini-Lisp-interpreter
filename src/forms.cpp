#include "./forms.h"

std::string LambdaValue::toString() const {
    return "#<procedure>";
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
    for(auto& param : params){
        if(param->isSymbol() == false){
            throw LispError("Lambda parameters must be symbols");
        }
        paramNames.push_back(param->toString());
    }
    auto body = std::vector<ValuePtr>(args.begin() + 1, args.end());
    return std::make_shared<LambdaValue>(paramNames, body);
}

ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 2) {
        throw LispError("Invalid number of arguments for define");
    }
    auto first = args[0];
    if(first->asSymbol()){
        auto name = first->asSymbol();
        env.addSymbol(*name, env.eval(args[1]));
        return std::make_shared<NilValue>();
    } else if (first->isPair()){
        auto name = first->CAR()->asSymbol();
        std::vector<ValuePtr> params;
        params.push_back(first->CDR());
        params.push_back(args[1]);
        ValuePtr lambda = lambdaForm(params, env);
        env.addSymbol(*name, lambda);
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

const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS{
    {"lambda",lambdaForm},
    {"define", defineForm},
    {"quote", quoteForm},
    {"if", ifForm},
    {"and", andForm},
    {"or", orForm}
};