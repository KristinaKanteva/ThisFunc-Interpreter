#include "Interpreter.h"
#include "NumberLiteral.h"
#include "ParamRef.h"
#include "FunctionCall.h"
#include "FunctionDecl.h"
#include <cmath>
#include <stdexcept>

Interpreter::Interpreter() {}

double Interpreter::requireNumber(const Value& value) const {
    if (value.type != Value::NUMBER) {
        throw std::runtime_error("Runtime error: Expected a number, got a list");
    }
    return value.number;
}

const std::vector<Value>& Interpreter::requireList(const Value& value) const {
    if (value.type != Value::LIST) {
        throw std::runtime_error("Runtime error: Expected a list, got a number");
    }
    return value.list;
}

bool Interpreter::evaluateBuiltin(const std::string& functionName, const std::vector<Value>& evaluatedArguments, Value& result) {
    if (functionName == "add") {
        if (evaluatedArguments.size() != 2)
            throw std::runtime_error("Runtime error: add expects 2 arguments, got " + std::to_string(evaluatedArguments.size()));

        double left = requireNumber(evaluatedArguments[0]);
        double right = requireNumber(evaluatedArguments[1]);
        result = Value(left + right);
        return true;
    }
    if (functionName == "sub") {
        if (evaluatedArguments.size() != 2)
            throw std::runtime_error("Runtime error: sub expects 2 arguments, got " + std::to_string(evaluatedArguments.size()));

        double left = requireNumber(evaluatedArguments[0]);
        double right = requireNumber(evaluatedArguments[1]);
        result = Value(left - right);
        return true;
    }
    if (functionName == "mul") {
        if (evaluatedArguments.size() != 2)
            throw std::runtime_error("Runtime error: mul expects 2 arguments, got " + std::to_string(evaluatedArguments.size()));

        double left = requireNumber(evaluatedArguments[0]);
        double right = requireNumber(evaluatedArguments[1]);
        result = Value(left * right);
        return true;
    }
    if (functionName == "div") {
        if (evaluatedArguments.size() != 2)
            throw std::runtime_error("Runtime error: div expects 2 arguments, got " + std::to_string(evaluatedArguments.size()));

        double dividend = requireNumber(evaluatedArguments[0]);
        double divisor = requireNumber(evaluatedArguments[1]);
        if (divisor == 0.0)
            throw std::runtime_error("Runtime error: Division by zero");

        result = Value(dividend / divisor);
        return true;
    }
    if (functionName == "pow") {
        if (evaluatedArguments.size() != 2)
            throw std::runtime_error("Runtime error: pow expects 2 arguments, got " + std::to_string(evaluatedArguments.size()));

        double base = requireNumber(evaluatedArguments[0]);
        double exponent = requireNumber(evaluatedArguments[1]);
        result = Value(std::pow(base, exponent));
        return true;
    }
    if (functionName == "sqrt") {
        if (evaluatedArguments.size() != 1)
            throw std::runtime_error("Runtime error: sqrt expects 1 argument, got " + std::to_string(evaluatedArguments.size()));

        double operand = requireNumber(evaluatedArguments[0]);
        if (operand < 0.0)
            throw std::runtime_error("Runtime error: sqrt of negative number");

        result = Value(std::sqrt(operand));
        return true;
    }
    if (functionName == "sin") {
        if (evaluatedArguments.size() != 1)
            throw std::runtime_error("Runtime error: sin expects 1 argument, got " + std::to_string(evaluatedArguments.size()));

        double operand = requireNumber(evaluatedArguments[0]);
        result = Value(std::sin(operand));
        return true;
    }
    if (functionName == "cos") {
        if (evaluatedArguments.size() != 1)
            throw std::runtime_error("Runtime error: cos expects 1 argument, got " + std::to_string(evaluatedArguments.size()));

        double operand = requireNumber(evaluatedArguments[0]);
        result = Value(std::cos(operand));
        return true;
    }
    if (functionName == "eq") {
        if (evaluatedArguments.size() != 2)
            throw std::runtime_error("Runtime error: eq expects 2 arguments, got " + std::to_string(evaluatedArguments.size()));

        double left = requireNumber(evaluatedArguments[0]);
        double right = requireNumber(evaluatedArguments[1]);
        result = Value(left == right ? 1.0 : 0.0);
        return true;
    }
    if (functionName == "le") {
        if (evaluatedArguments.size() != 2)
            throw std::runtime_error("Runtime error: le expects 2 arguments, got " + std::to_string(evaluatedArguments.size()));

        double left = requireNumber(evaluatedArguments[0]);
        double right = requireNumber(evaluatedArguments[1]);
        result = Value(left <= right ? 1.0 : 0.0);
        return true;
    }
    if (functionName == "nand") {
        if (evaluatedArguments.size() != 2)
            throw std::runtime_error("Runtime error: nand expects 2 arguments, got " + std::to_string(evaluatedArguments.size()));

        bool isLeftTrue = evaluatedArguments[0].isTrue();
        bool isRightTrue = evaluatedArguments[1].isTrue();
        result = Value(!(isLeftTrue && isRightTrue) ? 1.0 : 0.0);
        return true;
    }
    return false;
}

bool Interpreter::evaluateSpecial(const FunctionCall* functionCall, const std::vector<Value>& parentArguments, Value& result) {
    const std::string& functionName = functionCall->functionName;
    const std::vector<Expr*>& unevaluatedArguments = functionCall->arguments;

    if (functionName == "if") {
        if (unevaluatedArguments.size() != 3)
            throw std::runtime_error("Runtime error: if expects 3 arguments, got " + std::to_string(unevaluatedArguments.size()));

        Value condition = evaluate(unevaluatedArguments[0], parentArguments);
        if (condition.isTrue()) {
            result = evaluate(unevaluatedArguments[1], parentArguments);
        } else {
            result = evaluate(unevaluatedArguments[2], parentArguments);
        }
        return true;
    }

    if (functionName == "list") {
        std::vector<Value> elements;
        for (size_t index = 0; index < unevaluatedArguments.size(); index++) {
            elements.push_back(evaluate(unevaluatedArguments[index], parentArguments));
        }
        result = Value(elements);
        return true;
    }

    if (functionName == "head") {
        if (unevaluatedArguments.size() != 1)
            throw std::runtime_error("Runtime error: head expects 1 argument, got " + std::to_string(unevaluatedArguments.size()));

        Value evaluatedList = evaluate(unevaluatedArguments[0], parentArguments);
        const std::vector<Value>& listElements = requireList(evaluatedList);
        if (listElements.empty())
            throw std::runtime_error("Runtime error: head called on empty list");

        result = listElements[0];
        return true;
    }

    if (functionName == "tail") {
        if (unevaluatedArguments.size() != 1)
            throw std::runtime_error("Runtime error: tail expects 1 argument, got " + std::to_string(unevaluatedArguments.size()));

        Value evaluatedList = evaluate(unevaluatedArguments[0], parentArguments);
        const std::vector<Value>& listElements = requireList(evaluatedList);
        if (listElements.empty())
            throw std::runtime_error("Runtime error: tail called on empty list");

        std::vector<Value> tailElements(listElements.begin() + 1, listElements.end());
        result = Value(tailElements);
        return true;
    }

    if (functionName == "map") {
        if (unevaluatedArguments.size() != 2)
            throw std::runtime_error("Runtime error: map expects 2 arguments, got " + std::to_string(unevaluatedArguments.size()));

        if (unevaluatedArguments[0]->type() != Expr::Type::FunctionCall)
            throw std::runtime_error("Runtime error: First argument to map must be a function name");
        const FunctionCall* functionReference = static_cast<const FunctionCall*>(unevaluatedArguments[0]);
        std::string mappingFunctionName = functionReference->functionName;

        Value evaluatedList = evaluate(unevaluatedArguments[1], parentArguments);
        const std::vector<Value>& listElements = requireList(evaluatedList);

        std::vector<Value> mappedElements;
        for (size_t index = 0; index < listElements.size(); index++) {
            std::vector<Value> singleElementArgument;
            singleElementArgument.push_back(listElements[index]);
            mappedElements.push_back(callUserFunction(mappingFunctionName, singleElementArgument));
        }
        result = Value(mappedElements);
        return true;
    }

    if (functionName == "filter") {
        if (unevaluatedArguments.size() != 2)
            throw std::runtime_error("Runtime error: filter expects 2 arguments, got " + std::to_string(unevaluatedArguments.size()));

        if (unevaluatedArguments[0]->type() != Expr::Type::FunctionCall)
            throw std::runtime_error("Runtime error: First argument to filter must be a function name");

        const FunctionCall* functionReference = static_cast<const FunctionCall*>(unevaluatedArguments[0]);
        std::string predicateFunctionName = functionReference->functionName;

        Value evaluatedList = evaluate(unevaluatedArguments[1], parentArguments);
        const std::vector<Value>& listElements = requireList(evaluatedList);

        std::vector<Value> filteredElements;
        for (size_t index = 0; index < listElements.size(); index++) {
            std::vector<Value> singleElementArgument;
            singleElementArgument.push_back(listElements[index]);
            Value predicateResult = callUserFunction(predicateFunctionName, singleElementArgument);
            if (predicateResult.isTrue()) {
                filteredElements.push_back(listElements[index]);
            }
        }
        result = Value(filteredElements);
        return true;
    }

    if (functionName == "first") {
        if (unevaluatedArguments.size() != 2)
            throw std::runtime_error("Runtime error: first expects 2 arguments, got " + std::to_string(unevaluatedArguments.size()));

        if (unevaluatedArguments[0]->type() != Expr::Type::FunctionCall)
            throw std::runtime_error("Runtime error: First argument to first must be a function name");

        const FunctionCall* functionReference = static_cast<const FunctionCall*>(unevaluatedArguments[0]);
        std::string predicateFunctionName = functionReference->functionName;

        Value evaluatedList = evaluate(unevaluatedArguments[1], parentArguments);
        const std::vector<Value>& listElements = requireList(evaluatedList);

        for (size_t index = 0; index < listElements.size(); index++) {
            std::vector<Value> singleElementArgument;
            singleElementArgument.push_back(listElements[index]);
            Value predicateResult = callUserFunction(predicateFunctionName, singleElementArgument);
            if (predicateResult.isTrue()) {
                result = listElements[index];
                return true;
            }
        }
        result = Value(std::vector<Value>());
        return true;
    }

    return false;
}

Value Interpreter::callUserFunction(const std::string& functionName, const std::vector<Value>& evaluatedArguments) {
    Value result(0.0);
    if (evaluateBuiltin(functionName, evaluatedArguments, result)) {
        return result;
    }

    Expr* functionBody = functionStore.findFunc(functionName);
    if (functionBody == nullptr) {
        throw std::runtime_error("Runtime error: Undeclared function '" + functionName + "'");
    }

    return evaluate(functionBody, evaluatedArguments);
}

Value Interpreter::evaluate(const Expr* curr, const std::vector<Value>& arguments) {
    if (curr == nullptr) {
        throw std::runtime_error("Runtime error: Null expression encountered");
    }

    switch (curr->type()) {
        case Expr::Type::NumberLiteral: {
            const NumberLiteral* number = static_cast<const NumberLiteral*>(curr);
            return Value(number->value);
        }

        case Expr::Type::ParamRef: {
            const ParamRef* param = static_cast<const ParamRef*>(curr);
            if (param->index < 0 || param->index >= arguments.size()) {
                throw std::runtime_error("Runtime error: Parameter index #" + std::to_string(param->index) +
                    " out of range (function called with " + std::to_string(arguments.size()) + " arguments)");
            }

            return arguments[param->index];
        }

        case Expr::Type::FunctionCall: {
            const FunctionCall* functionCall = static_cast<const FunctionCall*>(curr);

            Value result(0.0);
            if (evaluateSpecial(functionCall, arguments, result)) {
                return result;
            }

            std::vector<Value> evaluatedArguments;
            for (size_t index = 0; index < functionCall->arguments.size(); index++) {
                evaluatedArguments.push_back(evaluate(functionCall->arguments[index], arguments));
            }

            
            if (evaluateBuiltin(functionCall->functionName, evaluatedArguments, result)) {
                return result;
            }

            Expr* functionBody = functionStore.findFunc(functionCall->functionName);
            if (functionBody == nullptr) {
                throw std::runtime_error("Runtime error: Undeclared function '" + functionCall->functionName + "'");
            }

            return evaluate(functionBody, evaluatedArguments);
        }

        case Expr::Type::FunctionDecl: {
            throw std::runtime_error("Runtime error: Cannot evaluate a function declaration as an expression");
        }
    }

    throw std::runtime_error("Runtime error: Unknown expression type");
}

bool Interpreter::execute(Expr* expression, Value& result) {
    if (expression == nullptr) {
        return false;
    }

    if (expression->type() == Expr::Type::FunctionDecl) {
        FunctionDecl* declaration = static_cast<FunctionDecl*>(expression);
        Expr* bodyToStore = declaration->body;
        declaration->body = nullptr;
        functionStore.saveFunc(declaration->functionName, bodyToStore);
        delete declaration;
        return false;
    }

    std::vector<Value> arguments;
    result = evaluate(expression, arguments);

    delete expression;
    return true;
}
