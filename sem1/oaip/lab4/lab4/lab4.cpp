#include <iostream>
#include <cmath>

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");

    int ArrayLength, NewArrayLength = 0, value, s4et, index = 0;

    cout << "Введите мощность массива: ";
    cin >> ArrayLength;

    int* Array = new int[ArrayLength];

    for (int i = 0; i < ArrayLength; i++) {
        cout << "Введите элемент номер " << i + 1 << " массива: ";
        cin >> Array[i];
    }

    system("cls");

    cout << "Ваш массив: " << endl;

    cout << "[";

    for (int i = 0; i < ArrayLength; i++) {
        if (i == ArrayLength - 1) {
            cout << Array[i];
        }
        else {
            cout << Array[i] << ",";
        }
    }

    cout << "]\n\n" << "Введите значение, по которому нужно будет удалить значения из массива: ";
    cin >> value;

    system("cls");

    cout << "Ваш старый массив: " << endl;

    cout << "[";

    for (int i = 0; i < ArrayLength; i++) {
        if (i == ArrayLength - 1) {
            cout << Array[i];
        }
        else {
            cout << Array[i] << ",";
        }
    }

    cout << "]\n\n" << "Ваш новый массив: \n";

    for (int i = 0; i < ArrayLength; i++) {
        if (Array[i] != value) {
            NewArrayLength++;
        }
    }

    int* NewArray = new int[NewArrayLength];

    for (int i = 0; i < ArrayLength; i++) {
        if (Array[i] != value) {
            NewArray[index] = Array[i];
            index++;
        }
    }

    cout << "[";

    for (int i = 0; i < NewArrayLength; i++) {
        if (i == NewArrayLength - 1) {
            cout << NewArray[i];
        }
        else {
            cout << NewArray[i] << ",";
        }
    }

    cout << "]";

    delete[] Array;
    Array = nullptr;

    delete[] NewArray;
    NewArray = nullptr;

    return 0;
}