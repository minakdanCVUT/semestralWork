#ifndef SEMESTRALKA_CONSOLE_H
#define SEMESTRALKA_CONSOLE_H


#include <string>

class Console {
public:
    std::string input();
    std::string output();
protected:
    std::string currentLine;
};


#endif //SEMESTRALKA_CONSOLE_H

