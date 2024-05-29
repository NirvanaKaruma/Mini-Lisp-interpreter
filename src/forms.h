#ifndef FORMS_H
#define FORMS_H

#include "./value.h"
#include "./eval_env.h"

#include <unordered_map>
#include <vector>

using SpecialFormType = std::shared_ptr<Value>(const std::vector<ValuePtr>&, EvalEnv&);


extern const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS;



#endif