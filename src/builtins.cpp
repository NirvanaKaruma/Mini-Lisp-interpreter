#include "./builtins.h"
#include "./error.h"
#include "./forms.h"


using namespace std::literals;
ValuePtr display(const std::vector<ValuePtr>& args){
    if(args.size() != 1){
        throw LispError("display procedure takes 1 argument");
    }
    std::cout << args[0]->toString();

    return std::make_shared<NilValue>();
}

ValuePtr displayln(const std::vector<ValuePtr>& args){
    if(args.size() != 1){
        throw LispError("display procedure takes 1 argument");
    }
    std::cout << args[0]->toString() << '\n';

    return std::make_shared<NilValue>();
}

[[noreturn]] ValuePtr exitProcedure(const std::vector<ValuePtr>& args) {
    if(args.empty()){
        std::exit(0);
    } else if (args.size() != 1){
        throw LispError("exit procedure takes 0 or 1 argument");
    } else {
        if(!args[0]->isNumber()){
            throw LispError("exit procedure takes a number as argument");
        }
        int exitCode = static_cast<int>(args[0]->asNumber());
        double num = args[0]->asNumber();
        if(!(num != exitCode)){
            throw LispError("exit procedure takes a int number as argument");
        } else {
            if (exitCode < 0 || exitCode > 255) {
                throw LispError("Exit code must be in the range [0, 255].");
            }
            std::exit(exitCode);
        }
    }
    
}

[[noreturn]] ValuePtr error(const std::vector<ValuePtr>& args){
    if(args.empty()) throw LispError(0);
    if(args.size() == 1){
        throw LispError(args[0]->toString());
    }
    throw LispError("More arguments than needed");
}

ValuePtr newline(const std::vector<ValuePtr>& args){
    if(args.empty()){
        std::cout<<'\n';
    } else {
        throw LispError("newline procedure takes 0 argument");
    }
    return std::make_shared<NilValue>();
}

ValuePtr print(const std::vector<ValuePtr>& args) {
    for (const auto& arg : args) {
        std::cout << arg->toString() << '\n';
    }
    return std::make_shared<NilValue>();
}

ValuePtr isAtom(const std::vector<ValuePtr>& args){
    if(args.size()!=1){
        throw LispError("integer procedure takes 1 argument");
    }
    ValuePtr arg = args[0];
    if(arg->isBool()||arg->isNumber()||arg->isString()||arg->isNil()||arg->isSymbol()){
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr isBoolean(const std::vector<ValuePtr>& args){
    ValuePtr arg = args[0];
    if(arg->isBool()){
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr isInteger(const std::vector<ValuePtr>& args){
    if(args.size()!=1){
        throw LispError("integer procedure takes 1 argument");
    }
    ValuePtr arg = args[0];
    if(!arg->isNumber()){
        return std::make_shared<BooleanValue>(false);
    }
    int int_num = static_cast<int>(arg->asNumber());
    if(int_num == arg->asNumber()){
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr isList(const std::vector<ValuePtr>& args){
    if(args.size()!=1){
        throw LispError("list procedure takes 1 argument");
    }
    ValuePtr arg = args[0];
    if(arg->isNil()||arg->isPair()&&arg->toString().find(".") == std::string::npos){
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr isNumber(const std::vector<ValuePtr>& args){
    if(args.size()!=1){
        throw LispError("number procedure takes 1 argument");
    }
    ValuePtr arg = args[0];
    if(arg->isNumber()){
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr isNull(const std::vector<ValuePtr>& args){
    if(args.size()!=1){
        throw LispError("null procedure takes 1 argument");
    }
    ValuePtr arg = args[0];
    if(arg->isNil()){
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr isPair(const std::vector<ValuePtr>& args){
    if(args.size()!=1){
        throw LispError("pair procedure takes 1 argument");
    }
    ValuePtr arg = args[0];
    if(arg->isPair()){
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr isProcedure(const std::vector<ValuePtr>& args){
    if(args.size()!=1){
        throw LispError("procedure procedure takes 1 argument");
    }
    ValuePtr arg = args[0];
    if(arg->isProcedure() || typeid(*arg) == typeid(BuiltinProcValue) || typeid(*arg) == typeid(LambdaValue)){
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr isString(const std::vector<ValuePtr>& args){
    if(args.size()!=1){
        throw LispError("string procedure takes 1 argument");
    }
    ValuePtr arg = args[0];
    if(arg->isString()){
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr isSymbol(const std::vector<ValuePtr>& args){
    if(args.size()!=1){
        throw LispError("symbol procedure takes 1 argument");
    }
    ValuePtr arg = args[0];
    if(arg->isSymbol()){
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr append(const std::vector<ValuePtr>& args){
    if(args.empty()){
        return std::make_shared<NilValue>();
    }
    std::vector<ValuePtr> result;
    for(const auto& arg : args){
        std::vector<ValuePtr> temp;
        if(arg->isPair()) temp = arg->toVector();
        else if (arg->isNil()) continue;
        else temp = {arg};
        std::copy(temp.begin(), temp.end(), std::back_inserter(result));
    }
    return list(result);
}
ValuePtr car(const std::vector<ValuePtr>& args){
    if(args.size()!=1){
        throw LispError("car procedure takes 1 argument");
    }
    ValuePtr arg = args[0];
    if(arg->isPair()){
        //强制类型转换为PairValue的指针
        auto pairExpr = std::dynamic_pointer_cast<PairValue>(arg);
        return pairExpr->CAR();
    }
    throw LispError("car procedure takes a pair as argument");
}

ValuePtr cdr(const std::vector<ValuePtr>& args){
    if(args.size()!=1){
        throw LispError("cdr procedure takes 1 argument");
    }
    ValuePtr arg = args[0];
    if(arg->isPair()){
        //强制类型转换为PairValue的指针
        auto pairExpr = std::dynamic_pointer_cast<PairValue>(arg);
        return pairExpr->CDR();
    }
    throw LispError("cdr procedure takes a pair as argument");
}

ValuePtr cons(const std::vector<ValuePtr>& args){
    if(args.size()!=2){
        throw LispError("cons procedure takes 2 argument");
    }
    ValuePtr car = args[0];
    ValuePtr cdr = args[1];
    return std::make_shared<PairValue>(car,cdr);
}

ValuePtr length(const std::vector<ValuePtr>& args){
    if(args.size()!=1){
        throw LispError("length procedure takes 1 argument");
    }
    auto result = args[0]->toVector().size();
    return std::make_shared<NumericValue>(result);
}

ValuePtr list(const std::vector<ValuePtr>& args){
    if(args.empty()){
        // 如果没有参数，返回一个空的PairValue表示空列表
        return std::make_shared<NilValue>();
    }
    auto car = args[0];
    std::vector<ValuePtr> cdr(args.begin() + 1, args.end());
    return std::make_shared<PairValue>(car, list(cdr));
}

ValuePtr add(const std::vector<ValuePtr>& params) {
    double result = 0;
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("Cannot add a non-numeric value.");
        }
        result += i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}
ValuePtr minus(const std::vector<ValuePtr>& params){
    if(params.size() != 2 && params.size() != 1){
        throw LispError("Minus expects exactly one or two arguments.");
    }
    double result = 0;
    if(params.size() == 1){
        if(!params[0]->isNumber()){
            throw LispError("Minus expects number(s).");
        }
        result = -params[0]->asNumber();
    }
    if(params.size() == 2){
        if(!params[0]->isNumber() || !params[1]->isNumber()){
            throw LispError("Minus expects number(s).");
        }
        result = params[0]->asNumber() - params[1]->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr times(const std::vector<ValuePtr>& params){
    double result = 1;
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("Cannot add a non-numeric value.");
        }
        result *= i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr divide(const std::vector<ValuePtr>& params){
    if(params.size() != 2 && params.size() != 1){
        throw LispError("Divide expects exactly one or two arguments.");
    }
    double result = 1;
    if(params.size() == 1){
        if(!params[0]->isNumber()){
            throw LispError("Divide expects number(s).");
        }
        if(params[0]->asNumber() == 0){
            throw LispError("Divide by zero is undefined.");
        }
        result /= params[0]->asNumber();
    }
    if(params.size() == 2){
        if(!params[0]->isNumber() || !params[1]->isNumber()){
            throw LispError("Divide expects number(s).");
        }
        if(params[1]->asNumber() == 0){
            throw LispError("Divide by zero is undefined.");
        }
        result = params[0]->asNumber() / params[1]->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr absolute(const std::vector<ValuePtr>& params){
    double result = 0;
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("Cannot add a non-numeric value.");
        }
        result = i->asNumber() > 0 ? i->asNumber() : -i->asNumber(); 
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr expt(const std::vector<ValuePtr>& params){
    if(params.size() != 2){
        throw LispError("Expt expects exactly two arguments.");
    }
    ValuePtr base = params[0];
    ValuePtr exponent = params[1];

    if (!base->isNumber() || !exponent->isNumber()) {
        throw LispError("Expt expects two numeric arguments.");
    }

    double b = base->asNumber();
    double e = exponent->asNumber();

    if(b == 0 && e == 0){
        throw LispError("Exponentiation with both base and exponent equal to zero is undefined.");
    }

    double result = pow(b, e);
    return std::make_shared<NumericValue>(result);
}

ValuePtr round(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("Round expects exactly one argument.");
    }

    ValuePtr num = params[0];

    if (!num->isNumber()) {
        throw LispError("Argument to round must be a number.");
    }

    double n = num->asNumber();

    // 向零取整函数
    int rounded = n >= 0 ? static_cast<int>(n) : static_cast<int>(n - 0.5);

    return std::make_shared<NumericValue>(rounded);
}

ValuePtr quotient(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("Quotient expects exactly two arguments.");
    }

    ValuePtr dividend = params[0];
    ValuePtr divisor = params[1];

    if (!dividend->isNumber() || !divisor->isNumber()) {
        throw LispError("Both arguments to quotient must be numbers.");
    }

    int d = dividend->asNumber();
    int s = divisor->asNumber();

    // 检查除数是否为零
    if (s == 0) {
        throw LispError("Cannot divide by zero.");
    }

    // 计算商并向下取整
    int result = d / s;

    return std::make_shared<NumericValue>(result);
}

ValuePtr modulo(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("Modulo expects exactly two arguments.");
    }

    ValuePtr dividend = params[0];
    ValuePtr divisor = params[1];

    if (!dividend->isNumber() || !divisor->isNumber()) {
        throw LispError("Both arguments to modulo must be numbers.");
    }

    double d = dividend->asNumber();
    double s = divisor->asNumber();

    // 检查除数是否为零
    if (s == 0) {
        throw LispError("Cannot divide by zero.");
    }

    // 计算余数
    double remainder = std::fmod(d, s);

    // 确保余数与除数有相同的正负性
    if (remainder * s < 0){
        remainder += s;
    }

    return std::make_shared<NumericValue>(remainder);
}

ValuePtr remainder(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("Remainder expects exactly two arguments.");
    }

    ValuePtr dividend = params[0];
    ValuePtr divisor = params[1];

    if (!dividend->isNumber() || !divisor->isNumber()) {
        throw LispError("Both arguments to remainder must be numbers.");
    }

    int d = dividend->asNumber();
    int s = divisor->asNumber();

    // 检查除数是否为零
    if (s == 0) {
        throw LispError("Cannot divide by zero.");
    }

    // 计算余数
    int remainder = d % s;

    // 确保余数与被除数有相同的正负性
    if (d < 0) {
        remainder = -(-d % s);
    }

    return std::make_shared<NumericValue>(remainder);
}

ValuePtr eq(const std::vector<ValuePtr>& params){
    if(params.size() != 2){
        throw LispError("Eq expects exactly two arguments.");
    }
    if(params[0]->isString() || params[0]->isPair()){
        bool result = (params[0] == params[1]);
        return std::make_shared<BooleanValue>(result);
    } else {
        bool result = (params[0]->toString() == params[1]->toString());
        return std::make_shared<BooleanValue>(result);
    }
}

ValuePtr equal(const std::vector<ValuePtr>& params){
    if(params.size() != 2){
        throw LispError("Equal expects exactly two arguments.");
    }
    bool result = (params[0]->toString() == params[1]->toString());
    return std::make_shared<BooleanValue>(result);
}

ValuePtr equal_num(const std::vector<ValuePtr>& params){
    if(params.size() != 2){
        throw LispError("Equal expects exactly two arguments.");
    }
    ValuePtr left = params[0];
    ValuePtr right = params[1];
    if (!left->isNumber() || !right->isNumber()) {
        throw LispError("Both arguments to equal_num must be numbers.");
    }
    bool result = (left->asNumber() == right->asNumber());
    return std::make_shared<BooleanValue>(result);
}

ValuePtr not_(const std::vector<ValuePtr>& params){
    if(params.size() != 1){
        throw LispError("Not expects exactly one argument.");
    }
    return std::make_shared<BooleanValue>(!params[0]->asBool());
}
ValuePtr less(const std::vector<ValuePtr>& params){
    if(params.size() != 2){
        throw LispError("Less expects exactly two arguments.");
    }
    ValuePtr left = params[0];
    ValuePtr right = params[1];
    if (!left->isNumber() || !right->isNumber()) {
        throw LispError("Both arguments to less must be numbers.");
    }
    bool result = (left->asNumber() < right->asNumber());
    return std::make_shared<BooleanValue>(result);
}

ValuePtr greater(const std::vector<ValuePtr>& params){
    if(params.size() != 2){
        throw LispError("Greater expects exactly two arguments.");
    }
    ValuePtr left = params[0];
    ValuePtr right = params[1];
    if (!left->isNumber() || !right->isNumber()) {
        throw LispError("Both arguments to greater must be numbers.");
    }
    bool result = (left->asNumber() > right->asNumber());
    return std::make_shared<BooleanValue>(result);
}

ValuePtr notmore(const std::vector<ValuePtr>& params){
    if(params.size() != 2){
        throw LispError("Notmore expects exactly two arguments.");
    }
    ValuePtr left = params[0];
    ValuePtr right = params[1];
    if (!left->isNumber() || !right->isNumber()) {
        throw LispError("Both arguments to notmore must be numbers.");
    }
    bool result = (left->asNumber() <= right->asNumber());
    return std::make_shared<BooleanValue>(result);
}

ValuePtr notless(const std::vector<ValuePtr>& params){
    if(params.size() != 2){
        throw LispError("Notless expects exactly two arguments.");
    }
    ValuePtr left = params[0];
    ValuePtr right = params[1];
    if (!left->isNumber() || !right->isNumber()) {
        throw LispError("Both arguments to notless must be numbers.");
    }
    bool result = (left->asNumber() >= right->asNumber());
    return std::make_shared<BooleanValue>(result);
}

ValuePtr even(const std::vector<ValuePtr>& params){
    if(params.size() != 1){
        throw LispError("Even expects exactly one argument.");
    }
    ValuePtr num = params[0];
    if (!num->isNumber()) {
        throw LispError("Argument to even must be a number.");
    }
    int realnum = num->asNumber();
    int integer = static_cast<int>(num->asNumber());
    if(realnum != integer){
        return std::make_shared<BooleanValue>(false);
    }
    bool result = (integer % 2 == 0);
    return std::make_shared<BooleanValue>(result);
}

ValuePtr odd(const std::vector<ValuePtr>& params){
    if(params.size() != 1){
        throw LispError("Odd expects exactly one argument.");
    }
    ValuePtr num = params[0];
    if (!num->isNumber()) {
        throw LispError("Argument to even must be a number.");
    }
    int realnum = num->asNumber();
    int integer = static_cast<int>(num->asNumber());
    if(realnum != integer){
        return std::make_shared<BooleanValue>(false);
    }
    bool result = (integer % 2 != 0);
    return std::make_shared<BooleanValue>(result);
}

ValuePtr zero(const std::vector<ValuePtr>& params){
    if(params.size() != 1){
        throw LispError("Zero expects exactly one argument.");
    }
    ValuePtr num = params[0];
    if (!num->isNumber()) {
        throw LispError("Argument to zero must be a number.");
    }
    bool result = (num->asNumber() == 0);
    return std::make_shared<BooleanValue>(result);
}

std::unordered_map<std::string, BuiltinProc> builtinProcs = {
    //核心库：
    {"display", &display},
    {"displayln",&displayln},
    {"exit", &exitProcedure},
    {"error",&error},
    {"newline", &newline},
    {"print", &print},
    //类型检查库：
    {"atom?",&isAtom},
    {"boolean?",&isBoolean},
    {"integer?",&isInteger},
    {"list?",&isList},
    {"number?",&isNumber},
    {"null?",&isNull},
    {"pair?",&isPair},
    {"procedure?",&isProcedure},
    {"string?",&isString},
    {"symbol?",&isSymbol},
    //对子与列表操作库:
    {"append", &append},
    {"car", &car},
    {"cdr", &cdr},
    {"cons", &cons},
    {"list", &list},
    {"length", &length},
    //算术运算库：
    {"+", &add},
    {"-", &minus},
    {"*", &times},
    {"/", &divide},
    {"abs", &absolute},
    {"expt", &expt},
    {"round", &round},
    {"quotient", &quotient},
    {"modulo", &modulo},
    {"remainder", &remainder},
    //比较库：
    {"eq?", &eq},
    {"equal?", &equal},
    {"not", &not_},
    {"=", &equal_num},
    {"<", &less},
    {">", &greater},
    {"<=", &notmore},
    {">=", &notless},
    {"even?", &even},
    {"odd?", &odd},
    {"zero?", &zero}
    // 添加更多的内置过程
};