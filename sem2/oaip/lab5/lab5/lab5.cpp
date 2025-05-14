#include <iostream>
#include <unordered_map>
#include <sstream>
#include "func.hpp"
#define MAX_LEN 100

using namespace std;

int main() {
    setlocale(LC_ALL, "ru");

    char expression[MAX_LEN] = {};
    char RPNexpression[MAX_LEN] = {};
    char substitutedRPN[MAX_LEN] = {};


    method(expression, RPNexpression, substitutedRPN);

    return 0;
}