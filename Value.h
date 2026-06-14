#pragma once

#include <vector>
#include <string>
#include <cmath>
#include <sstream>

struct Value {
    enum Type { NUMBER, LIST };
    Type type;
    double number;
    std::vector<Value> list;

    Value(double numberValue) : type(NUMBER), number(numberValue) {}
    Value(const std::vector<Value>& listElements) : type(LIST), number(0), list(listElements) {}

    bool isTrue() const {
        if (type == NUMBER) return number != 0.0;
        return !list.empty();
    }

    std::string toString() const {
        if (type == NUMBER) {
            std::stringstream ss;
            ss << number;
            return ss.str();
        }

        std::string formatted = "[";
        for (size_t index = 0; index < list.size(); index++) {
            if (index > 0) formatted += ", ";
            formatted += list[index].toString();
        }
        formatted += "]";
        return formatted;
    }
};
