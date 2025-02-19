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

    int** Matrix = new int* [n];

    for (int i = 0; i < n; i++) {
        Matrix[i] = new int[n];
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
                cin >> Matrix[i][j];
                while (Matrix[i][j] < 10 || Matrix[i][j] > 99) {
                    cout << "Элемент выходит за границы [10;99]. Повторите ввод: ";
                    cin >> Matrix[i][j];
                }
            }
        }

        system("cls");

        cout << "Ваша матрица:\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << Matrix[i][j] << " ";
            }
            cout << endl;
        }
    }
    else if (method == 2) {
        srand(time(0));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                Matrix[i][j] = min + rand() % (max - min + 1);
            }
        }

        system("cls");

        cout << "Ваша матрица:\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << Matrix[i][j] << " ";
            }
            cout << endl;
        }
    }

    int Max = Matrix[0][0];
    int Min = Matrix[0][0];

    for (int i = 0; i <= n / 2; i++) {
        for (int j = i; j < n - i; j++) {
            if (Matrix[i][j] > Max) {
                Max = Matrix[i][j];
            }
            if (Matrix[i][j] < Min) {
                Min = Matrix[i][j];
            }
        }
    }

    for (int i = n / 2 + 1; i < n; i++) {
        for (int j = n - i - 1; j <= i; j++) {
            if (Matrix[i][j] > Max) {
                Max = Matrix[i][j];
            }
            if (Matrix[i][j] < Min) {
                Min = Matrix[i][j];
            }
        }
    }

    cout << "\nМаксимальное число в области поиска: " << Max << endl;

    cout << "Минимальное число в области поиска: " << Min << endl;

    for (int i = 0; i < n; i++) {
        delete[] Matrix[i];
    }
    delete[] Matrix;

    return 0;
}