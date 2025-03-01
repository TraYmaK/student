#include <iostream>
#include <windows.h>
using namespace std;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    const int MAX_SIZE = 256;
    char input[MAX_SIZE];

    cout << "Введите строку (завершите ввод нажатием Enter): ";
    cin.getline(input, MAX_SIZE);

    cout << endl;

    int start = 0;
    bool isParsingNumber = false;
    bool dotFound = false;
    int parsedNumbers = 0;

    for (int i = 0; input[i] != '\0'; i++) {
        char c = input[i];

        if ((c >= '0' && c <= '9') || c == '.' || c == '+' || c == '-') {
            if (!isParsingNumber) {
                start = i;
                isParsingNumber = true;
            }
            if (c == '.') {
                if (dotFound) {
                    isParsingNumber = false;
                    dotFound = false;
                    continue;
                }
                dotFound = true;
            }
            if ((c == '+' || c == '-') && i != start) {
                isParsingNumber = false;
                dotFound = false;
                continue;
            }
        }
        else {
            if (isParsingNumber) {
                input[i] = '\0';
                cout << "Вещественное число: " << (input + start) << endl;
                isParsingNumber = false;
                dotFound = false;
                parsedNumbers++;
            }
        }
    }

    if (isParsingNumber) {
        cout << "Вещественное число: " << (input + start) << endl;
        parsedNumbers++;
    }

    if (parsedNumbers == 0) {
        cout << "В строке не найдено вещественных чисел." << endl;
    }

    return 0;
}