#pragma once

class Expr {
public:
    enum class Type { NumberLiteral, ParamRef, FunctionCall, FunctionDecl };
    virtual ~Expr() = default;
    virtual Type type() const = 0;
};
