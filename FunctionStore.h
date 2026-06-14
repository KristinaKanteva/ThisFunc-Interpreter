#pragma once

#include <string>
#include <unordered_map>
#include "Expr.h"

class FunctionStore {
    std::unordered_map<std::string, Expr*> userFunctions;
public:
    FunctionStore() {}

    ~FunctionStore() {
        for (std::unordered_map<std::string, Expr*>::iterator iterator = userFunctions.begin();
             iterator != userFunctions.end(); iterator++) {
            delete iterator->second;
        }
    }

    void saveFunc(const std::string& functionName, Expr* functionBody) {
        std::unordered_map<std::string, Expr*>::iterator existing = userFunctions.find(functionName);
        if (existing != userFunctions.end()) {
            delete existing->second;
            existing->second = functionBody;
        } else {
            userFunctions[functionName] = functionBody;
        }
    }

    Expr* findFunc(const std::string& functionName) const {
        std::unordered_map<std::string, Expr*>::const_iterator found = userFunctions.find(functionName);
        if (found != userFunctions.end()) {
            return found->second;
        }
        return nullptr;
    }
};
