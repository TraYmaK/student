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

    int FirstLength = 0;

    while (input[FirstLength] != '\0') {
        FirstLength++;
    }

    char* FirstString = new char[FirstLength + 1]; 

    for (int i = 0; i < FirstLength; i++) {
        FirstString[i] = input[i];
    }
    FirstString[FirstLength] = '\0';

    int WordCounter = 0;
    int WordStart = 0;
    int WordEnd = 0;

    int FirstWordStart = 0;
    int FirstWordEnd = 0;
    int ThirdWordStart = 0;
    int ThirdWordEnd = 0;
    int FifthWordStart = 0;
    int FifthWordEnd = 0;

    bool FirstWordFound = false;
    bool ThirdWordFound = false;
    bool FifthWordFound = false;

    for (int i = 0; i < FirstLength; i++) {
        if (FirstString[i] != ' ') {
            WordCounter++; 
            WordStart = i;
            for (int j = WordStart; j < FirstLength && FirstString[j] != ' '; j++) {
                WordEnd = j;
                i = j;
            }
            if (WordCounter == 1) {
                FirstWordStart = WordStart;
                FirstWordEnd = WordEnd;
                FirstWordFound = true;
            }
            else if (WordCounter == 3) {
                ThirdWordStart = WordStart;
                ThirdWordEnd = WordEnd;
                ThirdWordFound = true;
            }
            else if (WordCounter == 5) {
                FifthWordStart = WordStart;
                FifthWordEnd = WordEnd;
                FifthWordFound = true;
            }
        }
    }

    cout << "Исходная строка: " << FirstString << endl;

    int SecondLength = 0;

    if (FirstWordFound) {
        SecondLength += (FirstWordEnd - FirstWordStart + 1) + 1;
    }
    else if (ThirdWordFound) {
        SecondLength += (ThirdWordEnd - ThirdWordStart + 1) + 1;
    }
    else if (FifthWordFound) {
        SecondLength += (FifthWordEnd - FifthWordStart + 1);
    }

    char* SecondString = new char[SecondLength + 1];

    int pos = 0;

    if (FirstWordFound) {
        for (int i = FirstWordStart; i <= FirstWordEnd; i++) {
            SecondString[pos++] = FirstString[i];
        }
        SecondString[pos++] = ' ';
    }

    if (ThirdWordFound) {
        for (int i = ThirdWordStart; i <= ThirdWordEnd; i++) {
            SecondString[pos++] = FirstString[i];
        }
        SecondString[pos++] = ' ';
    }

    if (FifthWordFound) {
        for (int i = FifthWordStart; i <= FifthWordEnd; i++) {
            SecondString[pos++] = FirstString[i];
        }
    }
    else if (pos > 0 && SecondString[pos - 1] == ' ') {
        pos--;
    }

    SecondString[pos] = '\0';

    cout << "Новая строка: " << SecondString << endl;

    delete[] FirstString;
    delete[] SecondString;

    return 0;
}
