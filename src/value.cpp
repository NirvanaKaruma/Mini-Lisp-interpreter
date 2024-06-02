#include "./value.h"
#include <iostream>
#include <iomanip>
#include <sstream>


std::string BooleanValue::toString() const {
    return value ? "#t" : "#f";
}

std::string NumericValue::toString() const {
    if (value == static_cast<int>(value)) {
            return std::to_string(static_cast<int>(value));
        }
    return std::to_string(value);
}

std::string StringValue::toString() const {
    std::stringstream ss;
    ss << std::quoted(value);
    std::string&& result = ss.str();
    return result;
}

std::string NilValue::toString() const {
    return "()";
}

std::string SymbolValue::toString() const {
    return value;
}

std::string PairValue::toString() const {
    std::string result = "(" + car->toString();
    auto current = cdr;
    while (auto pair = dynamic_cast<const PairValue*>(current.get())) {
        result += " " + pair->car->toString();
        current = pair->cdr;
    }
    if (current != nullptr && !dynamic_cast<const NilValue*>(current.get())) {
        result += " . " + current->toString();
    }
    result += ")";
    return result;
}

std::string BuiltinProcValue::toString() const {
    return "#<procedure>";
}

std::vector<ValuePtr> PairValue::toVector() const {
    std::vector<ValuePtr> result;
    const PairValue* current = this;
    while (current) {
        result.push_back(current->car);
        if (auto next = std::dynamic_pointer_cast<PairValue>(current->cdr)) {
            current = next.get();
        } else {
            if (!current->cdr->isNil()) {
                result.push_back(current->cdr);
            }
            break;
        }
    }
    return result;
}
