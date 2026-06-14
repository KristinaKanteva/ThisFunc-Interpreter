#pragma once

#include "Expr.h"
#include <string>
#include <vector>

class FunctionCall : public Expr {
public:
    std::string functionName;
    std::vector<Expr*> arguments;

    FunctionCall(const std::string& calledFunctionName, const std::vector<Expr*>& callArguments)
        : functionName(calledFunctionName), arguments(callArguments) {}

    ~FunctionCall() override {
        for (size_t index = 0; index < arguments.size(); index++) {
            delete arguments[index];
        }
    }

    Type type() const override { return Type::FunctionCall; }
};
