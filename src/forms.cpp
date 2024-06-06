#include "./read.h"
#include "./forms.h"
#include "./builtins.h"
#include "./token.h"
#include "./tokenizer.h"
#include "./parser.h"
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
    if (args.size() != 3 && args.size() != 2) {
        throw LispError("Invalid number of arguments for if");
    }
    ValuePtr cond = env.eval(args[0]);
    if(args.size() == 3){
        if(cond->asBool() == true){
            return env.eval(args[1]);
        } else {
            return env.eval(args[2]);
        }
    } else if (args.size() == 2){
        if(cond->asBool() == true){
            return env.eval(args[1]);
        } else {
            return std::make_shared<NilValue>();
        }
    }
    throw BugError("You shouldn't come here, a terrible bug!");
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
        bool flag = true; // 判断是否所有字句都被跳过
        for (int i = 0; i < args.size(); i++) {
            auto relation = args[i]->toVector();
            if (relation[0]->toString() == "else"){
                if(i != args.size() - 1) throw LispError("Invalid else position");
                return env.eval(relation[1]);
            }
            if (relation.size() == 1) return env.eval(relation[0]);
            if (env.eval(relation[0])->asBool()) {
                flag = false;
                result = env.eval(relation[1]);
                break;
            }
        }
        if(flag == true) throw LispError("Invalid cond");
        return result;
    } else
        throw LispError("Invalid number of arguments for cond");
}

ValuePtr letForm(const std::vector<ValuePtr>& args, EvalEnv& env){
    if(args.size() < 2){
        throw LispError("Invalid number of arguments for let");
    }
    auto bindings = args[0]->toVector();
    std::vector<std::string> varNames;
    std::vector<ValuePtr> varValues;
    for (int i = 0; i < bindings.size(); i++){
        auto var = bindings[i]->toVector();
        varNames.emplace_back(var[0]->toString());
        varValues.emplace_back(env.eval(var[1]));
    }
    auto envChild = env.createChild(varNames, varValues);
    ValuePtr result;
    for (int i = 1; i < args.size(); i++){
        result = envChild->eval(args[i]);
    }
    return result;
}

ValuePtr beginForm(const std::vector<ValuePtr>& args, EvalEnv& env){
    if(args.empty()){
        throw LispError("Invalid number of arguments for begin");
    }
    ValuePtr result;
    for(const auto& arg : args){
        result = env.eval(arg);
    }
    return result;
}

ValuePtr quasiquoteForm(const std::vector<ValuePtr>& args, EvalEnv& env){
    if(args.size() != 1){
        throw LispError("Invalid number of arguments for quasiquote, should be only 1");
    }
    if(!args[0]->isPair()){
        return args[0];
    }
    auto value = args[0]->toVector();
    for(auto& i : value){
        if(i->isPair()){
            auto pair = i->toVector();
            if(pair[0]->asSymbol() == "unquote"){
                i = env.eval(pair[1]);
            }
        }
    }
    return list(value);
}

ValuePtr loadFileForm(const std::vector<ValuePtr>& args, EvalEnv& env){
    if(args.size() != 1){
        throw LispError("Invalid number of arguments for load-file");
    }
    std::string filename = args[0]->toString();
    filemode(filename);
    return std::make_shared<NilValue>();
}

ValuePtr readlineForm(const std::vector<ValuePtr>& args, EvalEnv& env){
    if(!args.empty()){
        throw LispError("Invalid number of arguments for read");
    }
    std::cout<<"IN: ";
    auto envChild = env.shared_from_this();
    std::string input = readInput();
    auto tokens = Tokenizer::tokenize(input);
    Parser parser(std::move(tokens));
    auto value = parser.parse();
    return envChild->eval(value);
}

const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS{
    {"lambda",lambdaForm},
    {"define", defineForm},
    {"quote", quoteForm},
    {"if", ifForm},
    {"and", andForm},
    {"or", orForm},
    {"cond", condForm},
    {"let", letForm},
    {"begin", beginForm},
    {"quasiquote", quasiquoteForm},
    {"load-file", loadFileForm},
    {"read-line", readlineForm}
};