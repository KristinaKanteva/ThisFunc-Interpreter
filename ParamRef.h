#pragma once

#include "Expr.h"

class ParamRef : public Expr {
public:
    int index;
    ParamRef(int parameterIndex) : index(parameterIndex) {}
    Type type() const override { return Type::ParamRef; }
};
