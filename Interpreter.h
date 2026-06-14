#pragma once

#include "Expr.h"
#include "FunctionCall.h"
#include "FunctionDecl.h"
#include "Value.h"
#include "FunctionStore.h"
#include <vector>
#include <string>

class Interpreter {
    FunctionStore functionStore;

    Value evaluate(const Expr* curr, const std::vector<Value>& arguments);
    double requireNumber(const Value& value) const;
    const std::vector<Value>& requireList(const Value& value) const;
    bool evaluateBuiltin(const std::string& functionName, const std::vector<Value>& evaluatedArguments, Value& result);
    bool evaluateSpecial(const FunctionCall* functionCall, const std::vector<Value>& parentArguments, Value& result);
    Value callUserFunction(const std::string& functionName, const std::vector<Value>& evaluatedArguments);

public:
    Interpreter();
    bool execute(Expr* expression, Value& result);
};
