#include <iostream>
#include "func.hpp"
#define MAX_LEN 100

using namespace std;

int main() {
    setlocale(LC_ALL, "ru");

    char expression[MAX_LEN] = {};
    char RPNexpression[MAX_LEN] = {};

    method(expression, RPNexpression);

    return 0;
}