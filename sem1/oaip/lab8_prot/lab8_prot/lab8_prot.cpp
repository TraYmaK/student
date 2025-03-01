#include <iostream>

using namespace std;

int Search(int arr[], int n, int c) {
    int left = 0;
    int right = n - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == c) {
            return mid;
        }
        else if (arr[mid] < c) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return -1;
}

int SearchI(int arr[], int n, int c) {
    int left = 0;
    int right = n - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == c) {
            return mid;
        }
        else if (arr[mid] < c) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return -1;
}

int main() {
    setlocale(LC_ALL, "Russian");
    int n, c;
    cout << "Введите размер массива: ";
    cin >> n;

    int* arr = new int[n];
    cout << "Введите элементы массива (упорядоченные): ";
    for (int i = 0; i < n; ++i) {
        cin >> arr[i];
    }

    cout << "Введите элемент для поиска: ";
    cin >> c;

    int result = Search(arr, n, c);

    if (result != -1) {
        cout << "Элемент " << c << " найден на месте " << result << ".\n";
    }
    else {
        cout << "Элемент " << c << " не найден в массиве.\n";
    }

    delete[] arr;
    return 0;
}