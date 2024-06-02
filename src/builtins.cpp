#include "./builtins.h"
#include "./error.h"
#include "./forms.h"


using namespace std::literals;

// 辅助函数
MatrixValue IdentityMatrix(int n){
    if (n <= 0) {
        throw MathError("Matrix dimensions must be positive.");
    }

    MatrixValue result(n, n);
    for (int i = 0; i < n; i++) {
        result.element[i][i] = 1;
    }
    return result;
}

// 内置过程
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
    if(params.empty()){
        return std::make_shared<NumericValue>(0);
    }
    if(params[0]->isNumber()){
        double result = 0;
        for (const auto& i : params) {
            if (!i->isNumber()) {
                throw LispError("Cannot add a non-numeric value.");
            }
            result += i->asNumber();
        }
        return std::make_shared<NumericValue>(result);
    } else if(params[0]->isMatrix()){
        int rows = params[0]->getrows();
        int cols = params[0]->getcols();
        MatrixValue result(rows,cols);
        for (const auto& i : params) {
            if(!i->isMatrix()){
                throw LispError("Cannot add a non-matrix value.");
            }
            MatrixValue temp = dynamic_cast<const MatrixValue&>(*i);
            result = result + temp;
        }
        return std::make_shared<MatrixValue>(result);
    }
    throw LispError("Cannot add a non-matrix and non-numeric value.");
}
ValuePtr minus(const std::vector<ValuePtr>& params){
    if(params.size() != 2 && params.size() != 1){
        throw LispError("Minus expects exactly one or two arguments.");
    }
    if(params[0]->isNumber()){
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
    } else if (params[0]->isMatrix()) {
        int rows = params[0]->getrows();
        int cols = params[0]->getcols();
        MatrixValue result(rows,cols);
        if(params.size() == 1){
            MatrixValue temp = dynamic_cast<const MatrixValue&>(*params[0]);
            result = result - temp;
        } else if(params.size() == 2){
            if(!params[1]->isMatrix()){
                throw LispError("Error: - expects a matrix as second argument");
            }
            MatrixValue temp1 = dynamic_cast<const MatrixValue&>(*params[0]);
            MatrixValue temp2 = dynamic_cast<const MatrixValue&>(*params[1]);
            result = temp1 - temp2;
        }
        return std::make_shared<MatrixValue>(result);
    }
    throw LispError("Unexpected values");
}

ValuePtr times(const std::vector<ValuePtr>& params){
    if(params.size() < 2){
        throw LispError("Multiply expects at least two arguments.");
    }
    bool matrixFlag = false;
    int rows = 0;
    int cols = 0;
    for(size_t i = 0; i < params.size(); i++){
        if(!params[i]->isNumber() && !params[i]->isMatrix()){
            throw LispError("Multiply expects number(s) or Matrix(es).");
        }
        if(params[i]->isMatrix()){
            matrixFlag = true;
            rows = params[i]->getrows();
            cols = params[i]->getcols();
            break;
        }
    }

    if(matrixFlag == false){
        double result = 1;
        for(const auto& i : params){
            result *= i->asNumber();
        }
        return std::make_shared<NumericValue>(result);
    } else {
        MatrixValue result = IdentityMatrix(rows);
        for(size_t i = 0; i < params.size(); i++){
            if(params[i]->isMatrix()){
                MatrixValue temp = dynamic_cast<const MatrixValue&>(*params[i]);
                result = result * temp;
            } else if (params[i]->isNumber()){
                result =result * params[i]->asNumber();
            }
        }
        return std::make_shared<MatrixValue>(result);
    }
    throw LispError("Unexpected values");
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
    if(params[0]->isNumber() && params[1]->isNumber()){
        ValuePtr left = params[0];
        ValuePtr right = params[1];
        bool result = (left->asNumber() == right->asNumber());
        return std::make_shared<BooleanValue>(result);
    } else if (params[0]->isMatrix() && params[1]->isMatrix()){
        MatrixValue left = dynamic_cast<const MatrixValue&>(*params[0]);
        MatrixValue right = dynamic_cast<const MatrixValue&>(*params[1]);
        bool result = (left == right);
        return std::make_shared<BooleanValue>(result);
    }
    throw LispError("Equal expects two numeric or Matrix arguments.");
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

ValuePtr max(const std::vector<ValuePtr>& params){
    if(params.empty()){
        throw LispError("Max expects at least one argument.");
    }
    double result;
    if(!params[0]->isNumber()){
        throw LispError("All arguments to max must be numbers.");
    }
    result = params[0]->asNumber();
    for(const auto& i : params){
        if(!i->isNumber()){
            throw LispError("All arguments to max must be numbers.");
        }
        if(i->asNumber() > result){
            result = i->asNumber();
        }
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr min(const std::vector<ValuePtr>& params){
    if(params.empty()){
        throw LispError("Min expects at least one argument.");
    }
    double result;
    if(!params[0]->isNumber()){
        throw LispError("All arguments to min must be numbers.");
    }
    result = params[0]->asNumber();
    for(const auto& i : params){
        if(!i->isNumber()){
            throw LispError("All arguments to min must be numbers.");
        }
        if(i->asNumber() < result){
            result = i->asNumber();
        }
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr number2String(const std::vector<ValuePtr>& params){
    if(params.size() != 1){
        throw LispError("number->string expects exactly one argument.");
    }
    double num = params[0]->asNumber();
    std::string result;
    if(num == static_cast<int>(num)){
        result = std::to_string(static_cast<int>(num));
    } else {
        result = std::to_string(num);
    }
    return std::make_shared<StringValue>(result);
}

ValuePtr string2Number(const std::vector<ValuePtr>& params){
    if(params.size() != 1){
        throw LispError("string->number expects exactly one argument.");
    }
    std::string str = params[0]->asString();
    try{
        double num = std::stod(str);
        return std::make_shared<NumericValue>(num);
    } catch (std::invalid_argument&) {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr makingStr(const std::vector<ValuePtr>& params){
    if(params.size() != 1 && params.size() != 2){
        throw LispError("make-string expects exactly one or two arguments.");
    }
    int num = static_cast<int>(params[0]->asNumber());
    char c = ' ';
    if(params.size() == 2){
        if(!params[1]->isString() || params[1]->asString().size() != 1){
            throw LispError("make-string expects a char as its second argument.");
        }
        c = params[1]->asString()[0];
    }
    return std::make_shared<StringValue>(std::string(num, c));
}

ValuePtr strRef(const std::vector<ValuePtr>& params){
    if(params.size() != 2){
        throw LispError("string-ref expects exactly two arguments.");
    }
    if(!params[0]->isString()){
        throw LispError("string-ref expects a string as its first argument.");
    }
    if(!params[1]->isNumber()){
        throw LispError("string-ref expects a number as its second argument.");
    }
    int pos = params[1]->asNumber();
    if(pos < 0 || (params[1]->asNumber() != static_cast<int>(params[1]->asNumber()))){
        throw LispError("string-ref expects a non-negative integer as its second argument.");
    }
    if(params[0]->asString().size() <= pos){
        throw LispError("string-ref expects a position that is less than the length of the string.");
    }
    return std::make_shared<StringValue>(std::string(1, params[0]->asString()[pos]));
}

ValuePtr strLength(const std::vector<ValuePtr>& params){
    if(params.size() != 1){
        throw LispError("string-length expects exactly one argument.");
    }
    if(!params[0]->isString()){
        throw LispError("string-length expects a string as its argument.");
    }
    return std::make_shared<NumericValue>(params[0]->asString().size());
}

ValuePtr strCopy(const std::vector<ValuePtr>& params){
    if(params.size() != 1){
        throw LispError("string-copy expects exactly one argument.");
    }
    if(!params[0]->isString()){
        throw LispError("string-copy expects a string as its argument.");
    }
    std::string str = params[0]->asString();
    return std::make_shared<StringValue>(str);
}

ValuePtr subStr(const std::vector<ValuePtr>& params){
    if(params.size() == 0 || params.size() > 3){
        throw LispError("subString expects at most three arguments, at least one argument");
    }
    if(!params[0]->isString()){
        throw LispError("subString expects a string as its first argument.");
    }
    // 只有一个参数，相当于复制字符串
    if(params.size() == 1){
        return strCopy(params);
    }
    std::string str = params[0]->asString();
    int pos = params[1]->asNumber();
    std::size_t num = std::string::npos;
    // 两个参数，相当于从某个pos开始截取字符串
    // 三个参数，相当于从某个pos开始截取num个字符
    if(params.size() == 3) num = static_cast<std::size_t>(params[2]->asNumber());
    if (num < 0 || pos < 0 || pos >= str.size()){
        throw LispError("illegal subString.");
    }
    return std::make_shared<StringValue>(str.substr(pos, num));
}

ValuePtr strAppend(const std::vector<ValuePtr>& params){
    if(params.size()<2){
        throw LispError("string-append expects at least two arguments.");
    }
    std::string str;
    for(const auto& i : params){
        if(!i->isString()){
            throw LispError("string-append expects a string as its arguments.");
        }
        str += i->asString();
    }
    return std::make_shared<StringValue>(str);
}

ValuePtr rationalSet(const std::vector<ValuePtr>& params){
    if(params.size() != 2 && params.size() != 1){
        throw LispError("rational-set expects exactly one or two arguments.");
    }
    if(params.size() == 1){
        if(!params[0]->isNumber()){
            throw LispError("rational-set expects a number as its first argument.");
        }
        return std::make_shared<RationalValue>(params[0]->asNumber());
    }
    if(!params[0]->isNumber() || !params[1]->isNumber()){
        throw LispError("rational-set expects numbers as its arguments.");
    }
    return std::make_shared<RationalValue>(static_cast<int>(params[0]->asNumber()), static_cast<int>(params[1]->asNumber()));
}

ValuePtr rationalAdd(const std::vector<ValuePtr>& params){
    RationalValue result(0);
    for(const auto& i : params){
        if(!i->isNumber()){
            throw LispError("rational-add expects numbers as its arguments.");
        }
        RationalValue temp(0);
        if(!i->isRational()) temp = RationalValue(i->asNumber());
        else temp = dynamic_cast<const RationalValue&>(*i);
        result = addRational(result, temp);
    }
    return std::make_shared<RationalValue>(result);
}

ValuePtr rationalMinus(const std::vector<ValuePtr>& params){
    if(params.size() != 1 && params.size() != 2){
        throw LispError("rational-minus expects exactly one or two arguments.");
    }
    RationalValue result(0);
    if(params.size() == 1){
        if(!params[0]->isNumber()){
            throw LispError("rational-minus expects a number as its first argument.");
        }
        RationalValue temp(0);
        params[0]->isRational() ? temp = dynamic_cast<const RationalValue&>(*params[0]) : temp = RationalValue(params[0]->asNumber());
        result = minusRational(result, temp);
    } else {
        if(!params[0]->isNumber() || !params[1]->isNumber()){
            throw LispError("rational-minus expects numbers as its arguments.");
        }
        RationalValue temp1(0);
        RationalValue temp2(0);
        params[0]->isRational() ? temp1 = dynamic_cast<const RationalValue&>(*params[0]) : temp1 = RationalValue(params[0]->asNumber());
        params[1]->isRational() ? temp2 = dynamic_cast<const RationalValue&>(*params[1]) : temp2 = RationalValue(params[1]->asNumber());
        result = minusRational(temp1, temp2);
    }
    return std::make_shared<RationalValue>(result);
}

ValuePtr rationalTimes(const std::vector<ValuePtr>& params){
    RationalValue result(0);
    for(const auto& i : params){
        if(!i->isNumber()){
            throw LispError("rational-times expects numbers as its arguments.");
        }
        RationalValue temp(0);
        if(!i->isRational()) temp = RationalValue(i->asNumber());
        else temp = dynamic_cast<const RationalValue&>(*i);
        result = timesRational(result, temp);
    }
    return std::make_shared<RationalValue>(result);
}

ValuePtr rationalDivide(const std::vector<ValuePtr>& params){
    if(params.size() != 2 && params.size() != 1){
        throw LispError("rational-divide expects exactly one or two arguments.");
    }
    RationalValue result(1);
    if(params.size() == 1){
        if(!params[0]->isNumber()){
            throw LispError("rational-divide expects a number as its first argument.");
        }
        RationalValue temp(1);
        params[0]->isRational() ? temp = dynamic_cast<const RationalValue&>(*params[0]) : temp = RationalValue(params[0]->asNumber());
        result = divideRational(result, temp);
    } else {
        if(!params[0]->isNumber() || !params[1]->isNumber()){
            throw LispError("rational-divide expects numbers as its arguments.");
        }
        if(params[1]->asNumber() == 0){
            throw LispError("rational-divide: division by zero.");
        }
        RationalValue temp1(1);
        RationalValue temp2(1);
        params[0]->isRational() ? temp1 = dynamic_cast<const RationalValue&>(*params[0]) : temp1 = RationalValue(params[0]->asNumber());
        params[1]->isRational() ? temp2 = dynamic_cast<const RationalValue&>(*params[1]) : temp2 = RationalValue(params[1]->asNumber());
        result = divideRational(temp1, temp2);
    }
    return std::make_shared<RationalValue>(result);
}

ValuePtr rationalAbsolute(const std::vector<ValuePtr>& params){
    if(params.size() != 1){
        throw LispError("rational-absolute expects exactly one argument.");
    }
    if(!params[0]->isNumber()){
        throw LispError("rational-absolute expects a number as its argument.");
    }
    RationalValue result(params[0]->asNumber());
    return std::make_shared<RationalValue>(absRational(result));
}

ValuePtr rationalEqual(const std::vector<ValuePtr>& params){
    if(params.size() != 2){
        throw LispError("rational-equal expects exactly two arguments.");
    }
    if(!params[0]->isNumber() || !params[1]->isNumber()){
        throw LispError("rational-equal expects numbers as its arguments.");
    }
    RationalValue temp1(1);
    RationalValue temp2(1);
    params[0]->isRational() ? temp1 = dynamic_cast<const RationalValue&>(*params[0]) : temp1 = RationalValue(params[0]->asNumber());
    params[1]->isRational() ? temp2 = dynamic_cast<const RationalValue&>(*params[1]) : temp2 = RationalValue(params[1]->asNumber());
    return std::make_shared<BooleanValue>(equalRational(temp1, temp2));
}

ValuePtr matrixSet(const std::vector<ValuePtr>& params){
    if(params.empty()){
        throw LispError("matrix-set expects at least one argument.");
    }
    int rows = params.size();
    if(!params[0]->isPair()){
        throw LispError("matrix-set expects a list of lists.");
    }
    int cols = params[0]->toVector().size();
    std::vector<std::vector<double>> element;
    for(const auto& param : params){
        if(!param->isPair()){
            throw LispError("matrix-set expects a list of lists.");
        }
        auto temp = param->toVector();
        int tempcols = param->toVector().size();
        if(tempcols != cols){
            throw LispError("matrix-set expects a list of lists with the same number of columns.");
        }
        std::vector<double> temprow;
        for(const auto& eles : temp){
            ValuePtr ele = eles;
            if(ele->isNumber()){
                temprow.emplace_back(ele->asNumber());
            } else {
                throw LispError("matrix-set expects a list of lists with numbers as elements.");
            }
        }
        element.emplace_back(temprow);
    }
    return std::make_shared<MatrixValue>(element);
}

ValuePtr matrixTranspose(const std::vector<ValuePtr>& params){
    if(params.size() != 1){
        throw LispError("matrix-transpose expects one parameter.");
    }
    if(!params[0]->isMatrix()){
        throw LispError("matrix-transpose expects a matrix as parameter.");
    }
    MatrixValue matrix = dynamic_cast<const MatrixValue&>(*params[0]);
    return std::make_shared<MatrixValue>(matrix.Transpose());
}
ValuePtr matrixIdentity(const std::vector<ValuePtr>& params){
    if(params.size() != 1){
        throw LispError("matrix-identity expects one parameter.");
    }
    if(!isInteger(params)){
        throw LispError("matrix-identity expects an integer as parameter.");
    }
    int n = params[0]->asNumber();
    MatrixValue result = IdentityMatrix(n);
    return std::make_shared<MatrixValue>(result);
}

ValuePtr matrixTimes(const std::vector<ValuePtr>& params){
    if(params.size() != 2){
        throw LispError("matrix-times expects two parameters.");
    }
    if(!params[0]->isMatrix() || !params[1]->isMatrix()){
        throw LispError("matrix-times expects two matrices as parameters.");
    }
    MatrixValue matrix1 = dynamic_cast<const MatrixValue&>(*params[0]);
    MatrixValue matrix2 = dynamic_cast<const MatrixValue&>(*params[1]);
    auto result = matrix1 * matrix2;
    return std::make_shared<MatrixValue>(result);
}

ValuePtr matrixMultiply(const std::vector<ValuePtr>& params){
    if(params.size() != 2){
        throw LispError("matrix-ele-wise-multiply expects two parameters.");
    }
    if(!params[0]->isMatrix() || !params[1]->isMatrix()){
        throw LispError("matrix-ele-wise-multiply expects two matrices as parameters.");
    }
    MatrixValue matrix1 = dynamic_cast<const MatrixValue&>(*params[0]);
    MatrixValue matrix2 = dynamic_cast<const MatrixValue&>(*params[1]);
    auto result = elementWiseMultiply(matrix1, matrix2);
    return std::make_shared<MatrixValue>(result);
}

ValuePtr matrixTrace(const std::vector<ValuePtr>& params){
    if(params.size() != 1){
        throw LispError("matrix-trace expects one parameter.");
    }
    if(!params[0]->isMatrix()){
        throw LispError("matrix-trace expects a matrix as parameter.");
    }
    MatrixValue matrix = dynamic_cast<const MatrixValue&>(*params[0]);
    return std::make_shared<NumericValue>(matrix.trace());
}

ValuePtr matrixDet(const std::vector<ValuePtr>& params){
    if(params.size() != 1){
        throw LispError("matrix-det expects one parameter.");
    }
    if(!params[0]->isMatrix()){
        throw LispError("matrix-det expects a matrix as parameter.");
    }
    MatrixValue matrix = dynamic_cast<const MatrixValue&>(*params[0]);
    return std::make_shared<NumericValue>(matrix.det());
}

ValuePtr matrixRank(const std::vector<ValuePtr>& params){
    if(params.size() != 1){
        throw LispError("matrix-rank expects one parameter.");
    }
    if(!params[0]->isMatrix()){
        throw LispError("matrix-rank expects a matrix as parameter.");
    }
    MatrixValue matrix = dynamic_cast<const MatrixValue&>(*params[0]);
    return std::make_shared<NumericValue>(matrix.rank());
}

ValuePtr matrixUpperTriangle(const std::vector<ValuePtr>& params){
    if(params.size() != 1){
        throw LispError("matrix-upper-triangle expects one parameter.");
    }
    if(!params[0]->isMatrix()){
        throw LispError("matrix-upper-triangle expects a matrix as parameter.");
    }
    MatrixValue matrix = dynamic_cast<const MatrixValue&>(*params[0]);
    return std::make_shared<MatrixValue>(matrix.toUpperTriangularForm());
}

ValuePtr matrixInverse(const std::vector<ValuePtr>& params){
    if(params.size() != 1){
        throw LispError("matrix-inverse expects one parameter.");
    }
    if(!params[0]->isMatrix()){
        throw LispError("matrix-inverse expects a matrix as parameter.");
    }
    MatrixValue matrix = dynamic_cast<const MatrixValue&>(*params[0]);
    return std::make_shared<MatrixValue>(matrix.inverse());
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
    {"zero?", &zero},
    // 添加更多的内置过程：以下为ex库
    // 更多的算术运算库
    {"max", &max},
    {"min", &min},
    // 字符串运算库
    {"number->string",&number2String},
    {"string->number",&string2Number},
    {"make-string",&makingStr},
    {"string-ref",&strRef},
    {"string-length",&strLength},
    {"string-copy",&strCopy},
    {"substring",&subStr},
    {"string-append",&strAppend},
    // 有理数类库
    {"rational-set",&rationalSet},
    {"rational+",&rationalAdd},
    {"rational-",&rationalMinus},
    {"rational*",&rationalTimes},
    {"rational/",&rationalDivide},
    {"rational-abs",&rationalAbsolute},
    {"rational-equal",&rationalEqual},
    // 矩阵类库
    {"matrix-set",&matrixSet},
    {"T",&matrixTranspose},
    {"I",&matrixIdentity},
    {"@",&matrixTimes},
    {"multiply",&matrixMultiply},
    {"trace",&matrixTrace},
    {"det",&matrixDet},
    {"rank",&matrixRank},
    {"upper-triangle",&matrixUpperTriangle},
    {"inverse",&matrixInverse},
};