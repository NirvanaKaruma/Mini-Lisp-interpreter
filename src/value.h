#ifndef VALUE_H
#define VALUE_H

#include "./error.h"
#include <string>
#include <memory>
#include <optional>
#include <vector>
#include <functional>


class Value{
public:

    virtual ~Value() = default;
    virtual std::string toString() const { throw BugError("Oops, it is a base Value!"); }

    using ValuePtr = std::shared_ptr<Value>;
    virtual bool isSelfEvaluating() const { return false; }
    virtual bool isBool() const { return false; }
    virtual bool asBool() const { return true; }
    virtual bool isNil() const { return false; }
    virtual bool isSymbol() const { return false; }
    virtual std::optional<std::string> asSymbol() const { return std::nullopt; }
    virtual bool isPair() const { return false; }
    virtual bool isNumber() const { return false; }
    virtual bool isString() const { return false; }
    virtual bool isProcedure() const { return false; }
    virtual double asNumber() const {
        throw BugError("Cannot convert value to number.");
    }
    virtual std::vector<ValuePtr> toVector() const {return {};}
    virtual std::shared_ptr<Value> CAR(){ throw BugError("Not a pair."); }
    virtual std::shared_ptr<Value> CDR(){ throw BugError("Not a pair."); }
    
};

using ValuePtr = std::shared_ptr<Value>;


class BooleanValue : public Value{
public:
    explicit BooleanValue(bool value) : value(value) {}
    std::string toString() const override;
    bool isSelfEvaluating() const override { return true; }
    bool isBool() const override { return true; }
    bool asBool() const override { return value; }
    
    
private:
    bool value;
};

class NumericValue : public Value{
public:
    explicit NumericValue(double value) : value(value) {}
    std::string toString() const override;
    bool isSelfEvaluating() const override { return true;}
    bool isNumber() const override { return true; }
    double asNumber() const override { return value; }
    
private:
    double value;
};

class StringValue : public Value{
public:
    explicit StringValue(const std::string& value) : value(value) {}
    std::string toString() const override;
    bool isSelfEvaluating() const override { return true;}
    bool isString() const override { return true; }
    
private:
    std::string value;
};

class NilValue : public Value{
public:
    std::string toString() const override;
    bool isNil() const override { return true; }
    
};

class SymbolValue : public Value{
public:
    explicit SymbolValue(const std::string& value) : value(value) {}
    std::string toString() const override;
    bool isSymbol() const override { return true; }
    std::optional<std::string> asSymbol() const override { return value; }

private:
    std::string value;
};

class PairValue : public Value{
public:
    explicit PairValue(const ValuePtr& car, const ValuePtr& cdr) : car(car), cdr(cdr) {}
    std::string toString() const override;
    bool isPair() const override { return true; }
    std::vector<ValuePtr> toVector() const override;
    ValuePtr CAR() override { return car; }
    ValuePtr CDR() override { return cdr; }

private:
    ValuePtr car;
    ValuePtr cdr;
};


class BuiltinProcValue : public Value{
public:
    using BuiltinFuncType = std::shared_ptr<Value>(const std::vector<ValuePtr>&);

    BuiltinProcValue(std::function<BuiltinFuncType> func) : func(func) {}
    std::string toString() const override;
    bool isProcedure() const override { return true; }
    std::function<BuiltinFuncType> getFunc() const { return func; }
    
private:
    std::function<BuiltinFuncType> func;
};



#endif