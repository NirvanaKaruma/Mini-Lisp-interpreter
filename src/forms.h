#ifndef FORMS_H
#define FORMS_H

#include "./value.h"
#include "./eval_env.h"

#include <unordered_map>
#include <vector>

using SpecialFormType = std::shared_ptr<Value>(const std::vector<ValuePtr>&, EvalEnv&);

class LambdaValue : public Value{
public:
    LambdaValue(const std::vector<std::string>& params, const std::vector<ValuePtr>& body, std::shared_ptr<EvalEnv> env): params(params), body(body), env(env) {}
    std::string toString() const override;
    ValuePtr apply(const std::vector<ValuePtr>& args) const;

private:
    std::vector<std::string> params;
    std::vector<ValuePtr> body;    
    std::shared_ptr<EvalEnv> env;
};

extern const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS;



#endif