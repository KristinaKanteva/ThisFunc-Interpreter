#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

#include "Parser.h"
#include "Interpreter.h"

void processLine(const std::string& line, Interpreter& interpreter) {
    Parser parser(line);
    Expr* parsedExpr = parser.parseLine();

    if (parsedExpr == nullptr) return;

    Value result(0.0);
    bool hasResult = interpreter.execute(parsedExpr, result);

    if (hasResult) {
        std::cout << "> " << result.toString() << std::endl;
    }
}

void runFromFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'" << std::endl;
        return;
    }

    Interpreter interpreter;
    std::string line;
    int lineNumber = 0;

    while (std::getline(inputFile, line)) {
        lineNumber++;
        try {
            processLine(line, interpreter);
        } catch (const std::runtime_error& error) {
            std::cerr << "Line " << lineNumber << ": " << error.what() << std::endl;
        }
    }
}

void runInteractiveMode() {
    std::cout << "ThisFunc Interactive Mode (type 'exit' to quit)" << std::endl;

    Interpreter interpreter;
    std::string line;

    while (true) {
        std::cout << "> ";

        if (!std::getline(std::cin, line)) {
            break;
        }

        if (line == "exit") {
            break;
        }

        try {
            processLine(line, interpreter);
        } catch (const std::runtime_error& error) {
            std::cerr << error.what() << std::endl;
        }
    }
}

int main() {
    std::cout << "ThisFunc Interpreter" << std::endl;
    std::cout << "Select mode:" << std::endl;
    std::cout << "  1) Interactive mode" << std::endl;
    std::cout << "  2) Run from file" << std::endl;
    std::cout << "Choice: ";

    std::string modeChoice;
    if (!std::getline(std::cin, modeChoice)) {
        return 0;
    }

    if (modeChoice == "1") {
        runInteractiveMode();
    } else if (modeChoice == "2") {
        std::cout << "Enter file path: ";
        std::string filePath;
        if (!std::getline(std::cin, filePath)) {
            return 0;
        }
        runFromFile(filePath);
    } else {
        std::cerr << "Invalid choice. Please run the program again and select 1 or 2." << std::endl;
    }

    return 0;
}
