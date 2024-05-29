#ifndef BUILTINS_H
#define BUILTINS_H

#include "./value.h"
#include <iostream>
#include <unordered_map>
#include <string>

typedef ValuePtr (*BuiltinProc)(const std::vector<ValuePtr>&);

extern std::unordered_map<std::string, BuiltinProc> builtinProcs;

ValuePtr display(const std::vector<ValuePtr>& args);
ValuePtr exitProcedure(const std::vector<ValuePtr>& args);
ValuePtr newline(const std::vector<ValuePtr>& args);
ValuePtr print(const std::vector<ValuePtr>& args);

ValuePtr isAtom(const std::vector<ValuePtr>& args);
ValuePtr isBoolean(const std::vector<ValuePtr>& args);
ValuePtr isInteger(const std::vector<ValuePtr>& args);
ValuePtr isList(const std::vector<ValuePtr>& args);
ValuePtr isNumber(const std::vector<ValuePtr>& args);
ValuePtr isNull(const std::vector<ValuePtr>& args);
ValuePtr isPair(const std::vector<ValuePtr>& args);
ValuePtr isProcedure(const std::vector<ValuePtr>& args);
ValuePtr isString(const std::vector<ValuePtr>& args);
ValuePtr isSymbol(const std::vector<ValuePtr>& args);

ValuePtr car(const std::vector<ValuePtr>& args);
ValuePtr cdr(const std::vector<ValuePtr>& args);
ValuePtr cons(const std::vector<ValuePtr>& args);
ValuePtr length(const std::vector<ValuePtr>& args);
ValuePtr list(const std::vector<ValuePtr>& args);

ValuePtr add(const std::vector<ValuePtr>& params);
ValuePtr minus(const std::vector<ValuePtr>& params);
ValuePtr times(const std::vector<ValuePtr>& params);
ValuePtr divide(const std::vector<ValuePtr>& params);
ValuePtr absolute(const std::vector<ValuePtr>& params);
ValuePtr expt(const std::vector<ValuePtr>& params);
ValuePtr round(const std::vector<ValuePtr>& params);
ValuePtr quotient(const std::vector<ValuePtr>& params);
ValuePtr modulo(const std::vector<ValuePtr>& params);
ValuePtr remainder(const std::vector<ValuePtr>& params);

ValuePtr equal(const std::vector<ValuePtr>& params);
ValuePtr less(const std::vector<ValuePtr>& params);
ValuePtr greater(const std::vector<ValuePtr>& params);
ValuePtr notmore(const std::vector<ValuePtr>& params);
ValuePtr notless(const std::vector<ValuePtr>& params);
ValuePtr even(const std::vector<ValuePtr>& params);
ValuePtr odd(const std::vector<ValuePtr>& params);
ValuePtr zero(const std::vector<ValuePtr>& params);



#endif