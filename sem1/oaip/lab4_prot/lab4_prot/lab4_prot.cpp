#include <iostream>
#include <locale>

int main() {
    using namespace std;
    setlocale(LC_ALL, "ru");

    int size;

    cout << "Введите размер массивов: ";
    cin >> size;

    int* array1 = new int[size];
    int* array2 = new int[size];

    cout << "Введите элементы первого массива: ";

    for (int i = 0; i < size; i++) {
        cin >> array1[i];
    }

    cout << "Введите элементы второго массива: ";

    for (int i = 0; i < size; i++) {
        cin >> array2[i];
    }

    int minEl = array1[0];
    bool found = false;

    for (int i = 0; i < size; i++) {
        bool exists = false;
        for (int j = 0; j < size; j++) {
            if (array1[i] == array2[j]) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            if (!found || array1[i] < minEl) {
                minEl = array1[i];
                found = true;
            }
        }
    }

    if (found) {
        cout << "Наименьший элемент из первого массива, отсутствующий во втором: " << minEl << endl;
    }
    else {
        cout << "Все элементы первого массива присутствуют во втором." << endl;
    }

    delete[] array1;
    delete[] array2;

    return 0;
}
