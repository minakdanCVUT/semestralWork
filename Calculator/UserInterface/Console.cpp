#include "Console.h"
#include <iostream>

std::string Console::input() {
    getline(std::cin, currentLine);
}

std::string Console::output() {
    return currentLine;
}

