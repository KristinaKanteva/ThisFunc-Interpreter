#include "Parser.h"
#include "Utils.h"
#include "NumberLiteral.h"
#include "ParamRef.h"
#include "FunctionCall.h"
#include "FunctionDecl.h"
#include <stdexcept>

Parser::Parser(const std::string& source)
    : input(source), position(0) {}

char Parser::currentChar() const {
    if (position >= input.size()) return '\0';
    return input[position];
}

char Parser::peekChar() const {
    if (position + 1 >= input.size()) return '\0';
    return input[position + 1];
}

void Parser::skipWhitespace() {
    while (position < input.size() && Utils::isWhiteSpace(input[position])) {
        position++;
    }
}

double Parser::readNumber() {
    int startPosition = position;
    bool hasDecimalPoint = false;

    if (currentChar() == '-') {
        position++;
    }

    if (!Utils::isDigit(currentChar())) {
        throw std::runtime_error("Parse error: Expected digit at position " + std::to_string(position));
    }

    while (position < input.size() && (Utils::isDigit(currentChar()) || currentChar() == '.')) {
        if (currentChar() == '.') {
            if (hasDecimalPoint) {
                throw std::runtime_error("Parse error: Multiple decimal points in number at position " + std::to_string(position));
            }
            hasDecimalPoint = true;
        }
        position++;
    }

    return std::stod(input.substr(startPosition, position - startPosition));
}

std::string Parser::readIdentifier() {
    int startPosition = position;

    while (position < input.size() &&
           (Utils::isLetterOrDigit(currentChar()) || currentChar() == '_')) {
        position++;
    }

    return input.substr(startPosition, position - startPosition);
}

bool Parser::isAtEnd() {
    skipWhitespace();
    return position >= input.size();
}

bool Parser::isDeclaration() {
    int savedPosition = position;
    skipWhitespace();

    if (position >= input.size() || !(Utils::isLetter(currentChar()) || currentChar() == '_')) {
        position = savedPosition;
        return false;
    }

    while (position < input.size() && (Utils::isLetterOrDigit(currentChar()) || currentChar() == '_')) {
        position++;
    }

    skipWhitespace();

    bool foundArrow = (currentChar() == '<' && peekChar() == '-');
    position = savedPosition;
    return foundArrow;
}

Expr* Parser::parseExpr() {
    skipWhitespace();

    char character = currentChar();

    if (Utils::isDigit(character) || (character == '-' && Utils::isDigit(peekChar()))) {
        double value = readNumber();
        return new NumberLiteral(value);
    }

    if (character == '#') {
        position++;
        skipWhitespace();
        if (!Utils::isDigit(currentChar())) {
            throw std::runtime_error("Parse error: Expected parameter index after '#' at position " + std::to_string(position));
        }
        int parameterIndex = (int)readNumber();
        return new ParamRef(parameterIndex);
    }

    if (Utils::isLetter(character) || character == '_') {
        std::string functionName = readIdentifier();
        skipWhitespace();

        if (currentChar() == '(') {
            position++;

            std::vector<Expr*> arguments;
            skipWhitespace();
            if (currentChar() != ')') {
                arguments.push_back(parseExpr());
                skipWhitespace();
                while (currentChar() == ',') {
                    position++;
                    arguments.push_back(parseExpr());
                    skipWhitespace();
                }
            }

            if (currentChar() != ')') {
                throw std::runtime_error("Parse error: Expected ')' after function arguments at position " +
                    std::to_string(position) + ", got '" + std::string(1, currentChar()) + "'");
            }
            position++;
            return new FunctionCall(functionName, arguments);
        }

        return new FunctionCall(functionName, std::vector<Expr*>());
    }

    throw std::runtime_error("Parse error: Unexpected character '" + std::string(1, character) +
        "' at position " + std::to_string(position));
}

Expr* Parser::parseLine() {
    if (isAtEnd()) {
        return nullptr;
    }

    if (isDeclaration()) {
        skipWhitespace();
        std::string declaredName = readIdentifier();
        skipWhitespace();
        position += 2;

        Expr* declaredBody = parseExpr();

        if (!isAtEnd()) {
            throw std::runtime_error("Parse error: Unexpected content after declaration body at position " + std::to_string(position));
        }

        return new FunctionDecl(declaredName, declaredBody);
    }

    Expr* parsedExpression = parseExpr();

    if (!isAtEnd()) {
        throw std::runtime_error("Parse error: Unexpected content after expression at position " + std::to_string(position));
    }

    return parsedExpression;
}
