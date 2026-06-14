#include "Utils.h"

bool Utils::isWhiteSpace(char character) {
    return character == ' ' || character == '\t' ||
           character == '\n' || character == '\r' ||
           character == '\f' || character == '\v';
}

bool Utils::isDigit(char character) {
    return character >= '0' && character <= '9';
}

bool Utils::isLetter(char character) {
    return (character >= 'a' && character <= 'z') ||
           (character >= 'A' && character <= 'Z');
}

bool Utils::isLetterOrDigit(char character) {
    return isLetter(character) || isDigit(character);
}
