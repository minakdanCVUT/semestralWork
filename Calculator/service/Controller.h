#ifndef SEMESTRALKA_CONTROLLER_H
#define SEMESTRALKA_CONTROLLER_H


#include "../UI/Console.h"
#include "ExpressionReader.h"
#include "SystemReader.h"

class Controller{
    Console console;
    ExpressionReader reader;
    SystemReader systemReader;
public:
    double calculate(std::string expression);
    void readInput(Console& console);
    std::string writeOutput(Console& console);
};


#endif //SEMESTRALKA_CONTROLLER_H

