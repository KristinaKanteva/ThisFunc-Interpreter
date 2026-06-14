#pragma once

#include <string>
#include "Expr.h"

class Parser {
    std::string input;
    int position;

    char currentChar() const;
    char peekChar() const;
    void skipWhitespace();
    double readNumber();
    std::string readIdentifier();
    bool isAtEnd();
    bool isDeclaration();
    Expr* parseExpr();

public:
    Parser(const std::string& source);
    Expr* parseLine();
};
