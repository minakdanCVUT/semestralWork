#ifndef SEMESTRALKA_OPERATOR_H
#define SEMESTRALKA_OPERATOR_H


#include <string>
#include <map>

class Operator {
    std::map<std::string, std::function<int(int)>> unary;
    std::map<std::string, std::function<int(int, int)>> binary;

public:
    static int factorial(int x);

    const std::map<std::string, std::function<int(int)>> &getUnary() const;

    const std::map<std::string, std::function<int(int, int)>> &getBinary() const;

    Operator();

    std::function<int(int)> getUnaryOperator(std::string operation);

    std::function<int(int,int)> getBinaryOperator(std::string operation);
};
#endif //SEMESTRALKA_OPERATOR_H
