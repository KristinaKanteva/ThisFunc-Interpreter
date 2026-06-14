#pragma once

#include "Expr.h"

class NumberLiteral : public Expr {
public:
    double value;
    NumberLiteral(double literalValue) : value(literalValue) {}
    Type type() const override { return Type::NumberLiteral; }
};
