#ifndef RATIONAL_H
#define RATIONAL_H

#include "./value.h"

class RationalValue : public NumericValue {
public:
    RationalValue() = default;
    RationalValue(double value);
    RationalValue(const RationalValue& other);
    RationalValue(int numerator, int denominator);

    std::string toString() const override;
    bool isNumber() const override { return true; }
    bool isRational() const override { return true; }
    friend RationalValue addRational(const RationalValue& lhs, const RationalValue& rhs);
    friend RationalValue minusRational(const RationalValue& lhs, const RationalValue& rhs);
    friend RationalValue timesRational(const RationalValue& lhs, const RationalValue& rhs);
    friend RationalValue divideRational(const RationalValue& lhs, const RationalValue& rhs);

    friend RationalValue absRational(const RationalValue& lhs);
    friend bool equalRational(const RationalValue& lhs, const RationalValue& rhs);

private:
    int numerator;
    int denominator;

    //  实现约分操作
    void simplify();
};

#endif