#pragma once

#include "Expr.h"
#include <string>

class FunctionDecl : public Expr {
public:
    std::string functionName;
    Expr* body;

    FunctionDecl(const std::string& declaredFunctionName, Expr* declaredBody)
        : functionName(declaredFunctionName), body(declaredBody) {}

    ~FunctionDecl() override {
        delete body;
    }

    Type type() const override { return Type::FunctionDecl; }
};
