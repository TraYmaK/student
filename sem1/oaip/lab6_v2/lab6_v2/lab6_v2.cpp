#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string FirstString;

    cout << "Введите предложение: ";
    getline(cin, FirstString);

    string SecondString;

    int FirstStringLength = FirstString.length();
    int WordCounter = 0;
    int i = 0;

    while (i < FirstStringLength) {

        while (i < FirstStringLength && FirstString[i] == ' ') {
            i++;
        }

        int start = i;

        while (i < FirstStringLength && FirstString[i] != ' ') {
            i++;
        }

        if (start < i) {
            WordCounter++;
            string word = FirstString.substr(start, i - start);

            if (WordCounter == 1 || WordCounter == 3 || WordCounter == 5) {
                if (!SecondString.empty()) {
                    SecondString += " ";
                }
                SecondString += word;
            }
        }
    }

    cout << "Итоговое предложение: " << SecondString << endl;

    return 0;
}