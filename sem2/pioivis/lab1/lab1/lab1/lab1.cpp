#include <iostream>
#include <fstream>
#include <algorithm>
#include "heap.hpp"

using namespace std;

void menu(int*& heap, int& n, int curr) {
    while (true) {
        wall();
        printHeap(heap, n);
        wall();
        cout << "Список операций:" << endl;
        cout << "1. Добавление" << endl;
        cout << "2. Извлечение" << endl;
        cout << "3. Выход" << endl;
        wall();
        cout << "Выберите операцию: ";
        int choise = 0;
        cin >> choise;
        wall();
        system("pause");
        system("cls");
        if (choise == 1) {
            addToHeap(heap, n, curr);
        }
        else if (choise == 2) {
            int searchedMaximum = searchMax(heap, n);
            cout << "Извлечённый элемент: " << searchedMaximum << endl;
        }
        else if (choise == 3) {
            break;
        }
    }
}

void process(int*& heap, int& n, int curr) {
    int lastSheetParentIndex;
    heap = creatingAnUnorderedHeap(n, lastSheetParentIndex);
    creatingAnOrderedHeap(n, heap, lastSheetParentIndex);
    menu(heap, n, curr);

    delete[] heap;
}

int main() {
    setlocale(LC_ALL, "RU");

    int n = 0;
    int curr = 0;
    int* heap = nullptr;

    process(heap, n, curr);

    return 0;
}