#include "Operator.h"



Operator::Operator() {
    unary.insert({"ROUND", [](int x){return round(x);}});
    unary.insert({"FACT", [](int x){return factorial(x);}});
    binary.insert({"+", [](int x, int y){return x + y;}});
}

int Operator::factorial(int x) {
    return 1;
}

const std::map<std::string, std::function<int(int)>> &Operator::getUnary() const {
    return unary;
}
const std::map<std::string, std::function<int(int, int)>> &Operator::getBinary() const {
    return binary;
}

std::function<int(int)> Operator::getUnaryOperator(std::string operation) {
    return unary[operation];
}

std::function<int(int,int)> Operator::getBinaryOperator(std::string operation){
    return binary[operation];
}


