#ifndef BUILTINS_H
#define BUILTINS_H

#include "./value.h"
#include <iostream>
#include <unordered_map>
#include <string>

typedef ValuePtr (*BuiltinProc)(const std::vector<ValuePtr>&);

extern std::unordered_map<std::string, BuiltinProc> builtinProcs;

ValuePtr list(const std::vector<ValuePtr>& args);

#endif