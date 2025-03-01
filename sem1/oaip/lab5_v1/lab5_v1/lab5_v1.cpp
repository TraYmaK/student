#include <iostream>
#include <cmath>

using namespace std;

int main() {
    setlocale(LC_ALL, "ru");

    int n = 0, method = 0;
    int const min = 10;
    int const max = 99;

    cout << "Введите нечётный порядок квадратной матрицы: ";
    cin >> n;

    while (n % 2 == 0) {
        cout << "\nВыберите нечетный порядок: ";
        cin >> n;
    }

    cout << endl;

    int** FirstMatrix = new int* [n];

    for (int i = 0; i < n; i++) {
        FirstMatrix[i] = new int[n];
    }

    cout << "Методы заполнения матрицы:\n";
    cout << "1. Вручную [10;99]\n";
    cout << "2. Рандомно [10;99]\n\n";
    cout << "Выберите метод: ";
    cin >> method;

    while (method != 1 && method != 2) {
        cout << "\nВыберите корректный метод: ";
        cin >> method;
    }

    system("cls");

    if (method == 1) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << "Введите элемент " << i + 1 << " строки " << j + 1 << " столбца:";
                cin >> FirstMatrix[i][j];
                while (FirstMatrix[i][j] < 10 || FirstMatrix[i][j] > 99) {
                    cout << "Элемент выходит за границы [10;99]. Повторите ввод: ";
                    cin >> FirstMatrix[i][j];
                }
            }
        }

        system("cls");

        cout << "Ваша матрица:\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << FirstMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }
    else if (method == 2) {
        srand(time(0));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                FirstMatrix[i][j] = min + rand() % (max - min + 1);
            }
        }

        system("cls");

        cout << "Ваша матрица:\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << FirstMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }

    int FirstMax = FirstMatrix[0][0];
    int FirstMin = FirstMatrix[0][0];

    for (int i = 0; i <= n / 2; i++) {
        for (int j = i; j < n - i; j++) {
            if (FirstMatrix[i][j] > FirstMax) {
                FirstMax = FirstMatrix[i][j];
            }
            if (FirstMatrix[i][j] < FirstMin) {
                FirstMin = FirstMatrix[i][j];
            }
        }
    }

    int** SecondMatrix = new int* [n];

    for (int i = 0; i < n; i++) {
        SecondMatrix[i] = new int[n];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            SecondMatrix[i][j] = FirstMatrix[n - 1 - i][n - 1 - j];
        }
    }

    int SecondMax = SecondMatrix[0][0];
    int SecondMin = SecondMatrix[0][0];

    for (int i = 0; i <= n / 2; i++) {
        for (int j = i; j < n - i; j++) {
            if (SecondMatrix[i][j] > SecondMax) {
                SecondMax = SecondMatrix[i][j];
            }
            if (SecondMatrix[i][j] < FirstMin) {
                SecondMin = SecondMatrix[i][j];
            }
        }
    }

    cout << endl;

    int Max = 0;
    int Min = 0;

    if (FirstMax > SecondMax || FirstMax == SecondMax) {
        Max = FirstMax;
    }
    else if (FirstMax < SecondMax) {
        Max = SecondMax;
    }

    if (FirstMin > SecondMin || FirstMin == SecondMin) {
        Min = SecondMin;
    }
    else if (FirstMin < SecondMin) {
        Min = FirstMin;
    }

    cout << "Максимальное число в области поиска: " << Max << endl;

    cout << "Минимальное число в области поиска: " << Min << endl;

    for (int i = 0; i < n; i++) {
        delete[] FirstMatrix[i];
    }
    delete[] FirstMatrix;

    for (int i = 0; i < n; i++) {
        delete[] SecondMatrix[i];
    }
    delete[] SecondMatrix;

    return 0;
}