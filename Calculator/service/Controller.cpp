#include "Controller.h"
#include "SystemReader.h"

void Controller::readInput(Console &console) { // читает вход с консоли и передает на expressionReader
    while(true) {
        std::string input = console.input();
        if (input[2] == '=') {
            std::string expression;
            reader.setString((std::string) expression.substr(input.size() - 3));// можно создать отдельный класс reader
        } else {
            systemReader.read(input);
        }
    }
}

