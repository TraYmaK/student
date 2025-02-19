#include <iostream>
#include <clocale>
using namespace std;

int main() {
    setlocale(LC_ALL, "ru");

    int n, m;
    cout << "Введите количество строк (n) и столбцов (m): ";
    cin >> n >> m;

    int** matrix = new int* [n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new int[m];
    }

    cout << "Введите элементы матрицы:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> matrix[i][j];
        }
    }

    int maxEl = matrix[0][0];
    int maxRow = 0, maxCol = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (matrix[i][j] > maxEl) {
                maxEl = matrix[i][j];
                maxRow = i;
                maxCol = j;
            }
        }
    }

    for (int j = 0; j < m; j++) {
        int per = matrix[0][j];
        matrix[0][j] = matrix[maxRow][j];
        matrix[maxRow][j] = per;
    }

    for (int i = 0; i < n; i++) {
        int per = matrix[i][0];
        matrix[i][0] = matrix[i][maxCol];
        matrix[i][maxCol] = per;
    }

    cout << "Результирующая матрица:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }

    for (int i = 0; i < n; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;

    return 0;
}
