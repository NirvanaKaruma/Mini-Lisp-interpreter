#include "./rational.h"
#include "./error.h"
#include <numeric>

void RationalValue::simplify() {
    int gcd = std::gcd(numerator, denominator);
    if (gcd == 0) return;
    numerator /= gcd;
    denominator /= gcd;
    if(denominator < 0){
        numerator *= -1;
        denominator *= -1;
    }
}

RationalValue::RationalValue(double value) : NumericValue(value) {
    int newDenominator = 1;
    int flag = 0;
    while(flag < 100){
        if(value - (int)(value) == 0) break;
        value *= 10;
        newDenominator *= 10;
        flag++;
        // 如果newDenominator或者value再进行乘以十超出了整形的范围，则退出循环
        if(newDenominator  > INT_MAX / 10 || value > INT_MAX / 10){
            break;
        }
    }
    this->numerator = static_cast<int>(value);
    this->denominator = newDenominator;
    simplify();    
}

RationalValue::RationalValue(const RationalValue& other) : NumericValue(0), numerator(other.numerator), denominator(other.denominator) {
    simplify();
    this->value = static_cast<double>(numerator) / denominator;
}

RationalValue::RationalValue(int numerator, int denominator) :  NumericValue(0), numerator(numerator), denominator(denominator) {
    if(denominator == 0){
        throw MathError("Division by zero");
    }
    simplify();
    this->value = static_cast<double>(numerator) / denominator;
}

std::string RationalValue::toString() const {
    if(denominator == 1) return std::to_string(numerator);
    return std::to_string(numerator) + "/" + std::to_string(denominator);
}

RationalValue addRational(const RationalValue& lhs, const RationalValue& rhs){
    long long newNumerator = lhs.numerator * rhs.denominator + rhs.numerator * lhs.denominator;
    long long newDenominator = lhs.denominator * rhs.denominator;
    if(newNumerator == 0) return RationalValue(0, 1);
    long long gcd = std::gcd(newNumerator, newDenominator);
    if(gcd != 0){
        newNumerator /= gcd;
        newDenominator /= gcd;
    }
    return RationalValue(newNumerator, newDenominator);
}

RationalValue minusRational(const RationalValue& lhs, const RationalValue& rhs){
    long long newNumerator = lhs.numerator * rhs.denominator - rhs.numerator * lhs.denominator;
    long long newDenominator = lhs.denominator * rhs.denominator;
    if(newNumerator == 0) return RationalValue(0, 1);
    long long gcd = std::gcd(newNumerator, newDenominator);
    if(gcd != 0){
        newNumerator /= gcd;
        newDenominator /= gcd;
    }
    return RationalValue(newNumerator, newDenominator);
}

RationalValue timesRational(const RationalValue& lhs, const RationalValue& rhs){
    long long newNumerator = lhs.numerator * rhs.numerator;
    long long newDenominator = lhs.denominator * rhs.denominator;
    if(newNumerator == 0) return RationalValue(0, 1);
    long long gcd = std::gcd(newNumerator, newDenominator);
    if(gcd != 0){
        newNumerator /= gcd;
        newDenominator /= gcd;
    }
    return RationalValue(newNumerator, newDenominator);
}

RationalValue divideRational(const RationalValue& lhs, const RationalValue& rhs){
    if(rhs.numerator == 0){
        throw MathError("Division by zero");
    }
    long long newNumerator = lhs.numerator * rhs.denominator;
    long long newDenominator = lhs.denominator * rhs.numerator;
    if(newNumerator == 0) return RationalValue(0, 1);
    long long gcd = std::gcd(newNumerator, newDenominator);
    if(gcd != 0){
        newNumerator /= gcd;
        newDenominator /= gcd;
    }
    return RationalValue(newNumerator, newDenominator);
}

RationalValue absRational(const RationalValue& lhs){
    int newNumerator = lhs.numerator > 0 ? lhs.numerator : -lhs.numerator;
    int newDenominator = lhs.denominator;
    return RationalValue(newNumerator, newDenominator);
}

bool equalRational(const RationalValue& lhs, const RationalValue& rhs){
    return lhs.numerator == rhs.numerator && lhs.denominator == rhs.denominator;
}
