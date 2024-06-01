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
    while(true){
        if(value - (int)(value) == 0) break;
        value *= 10;
        newDenominator *= 10;
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
    int newNumerator = lhs.numerator * rhs.denominator + rhs.numerator * lhs.denominator;
    int newDenominator = lhs.denominator * rhs.denominator;
    return RationalValue(newNumerator, newDenominator);
}

RationalValue minusRational(const RationalValue& lhs, const RationalValue& rhs){
    int newNumerator = lhs.numerator * rhs.denominator - rhs.numerator * lhs.denominator;
    int newDenominator = lhs.denominator * rhs.denominator;
    return RationalValue(newNumerator, newDenominator);
}

RationalValue timesRational(const RationalValue& lhs, const RationalValue& rhs){
    int newNumerator = lhs.numerator * rhs.numerator;
    int newDenominator = lhs.denominator * rhs.denominator;
    return RationalValue(newNumerator, newDenominator);
}

RationalValue divideRational(const RationalValue& lhs, const RationalValue& rhs){
    if(rhs.numerator == 0){
        throw MathError("Division by zero");
    }
    int newNumerator = lhs.numerator * rhs.denominator;
    int newDenominator = lhs.denominator * rhs.numerator;
    return RationalValue(newNumerator, newDenominator);
}

RationalValue absRational(const RationalValue& lhs){
    return RationalValue(std::abs(lhs.numerator), lhs.denominator);
}

bool equalRational(const RationalValue& lhs, const RationalValue& rhs){
    return lhs.numerator == rhs.numerator && lhs.denominator == rhs.denominator;
}
