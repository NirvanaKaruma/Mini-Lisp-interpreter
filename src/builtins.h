#ifndef BUILTINS_H
#define BUILTINS_H

#include "./rational.h"
#include "./matrix.h"
#include "./value.h"
#include <iostream>
#include <unordered_map>
#include <string>

typedef ValuePtr (*BuiltinProc)(const std::vector<ValuePtr>&);

extern std::unordered_map<std::string, BuiltinProc> builtinProcs;

// 辅助函数
MatrixValue IdentityMatrix(int n);

//  以下为课程要求内置过程
ValuePtr display(const std::vector<ValuePtr>& args);
ValuePtr displayln(const std::vector<ValuePtr>& args);
ValuePtr exitProcedure(const std::vector<ValuePtr>& args);
ValuePtr error(const std::vector<ValuePtr>& args);
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
ValuePtr append(const std::vector<ValuePtr>& args);
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
ValuePtr eq(const std::vector<ValuePtr>& params);
ValuePtr equal(const std::vector<ValuePtr>& params);
ValuePtr equal_num(const std::vector<ValuePtr>& params);
ValuePtr not_(const std::vector<ValuePtr>& params);
ValuePtr less(const std::vector<ValuePtr>& params);
ValuePtr greater(const std::vector<ValuePtr>& params);
ValuePtr notmore(const std::vector<ValuePtr>& params);
ValuePtr notless(const std::vector<ValuePtr>& params);
ValuePtr even(const std::vector<ValuePtr>& params);
ValuePtr odd(const std::vector<ValuePtr>& params);
ValuePtr zero(const std::vector<ValuePtr>& params);

// 更多的算术运算库
ValuePtr max(const std::vector<ValuePtr>& params);
ValuePtr min(const std::vector<ValuePtr>& params);

// 字符串运算库
ValuePtr number2String(const std::vector<ValuePtr>& params);
ValuePtr string2Number(const std::vector<ValuePtr>& params);
ValuePtr makingStr(const std::vector<ValuePtr>& params);
ValuePtr strRef(const std::vector<ValuePtr>& params);
ValuePtr strLength(const std::vector<ValuePtr>& params);
ValuePtr strCopy(const std::vector<ValuePtr>& params);
ValuePtr subStr(const std::vector<ValuePtr>& params);
ValuePtr strAppend(const std::vector<ValuePtr>& params);

// 有理数类库
ValuePtr rationalSet(const std::vector<ValuePtr>& params);
ValuePtr rationalAdd(const std::vector<ValuePtr>& params);
ValuePtr rationalMinus(const std::vector<ValuePtr>& params);
ValuePtr rationalTimes(const std::vector<ValuePtr>& params);
ValuePtr rationalDivide(const std::vector<ValuePtr>& params);
ValuePtr rationalAbsolute(const std::vector<ValuePtr>& params);
ValuePtr rationalEqual(const std::vector<ValuePtr>& params);

// 矩阵类库
ValuePtr matrixSet(const std::vector<ValuePtr>& params);
ValuePtr matrixTranspose(const std::vector<ValuePtr>& params);
ValuePtr matrixIdentity(const std::vector<ValuePtr>& params);
ValuePtr matrixTimes(const std::vector<ValuePtr>& params);
ValuePtr matrixMultiply(const std::vector<ValuePtr>& params);
ValuePtr matrixTrace(const std::vector<ValuePtr>& params);
ValuePtr matrixDet(const std::vector<ValuePtr>& params);
ValuePtr matrixRank(const std::vector<ValuePtr>& params);
ValuePtr matrixUpperTriangle(const std::vector<ValuePtr>& params);
ValuePtr matrixInverse(const std::vector<ValuePtr>& params);

#endif