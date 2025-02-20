#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

void wall() {
    cout << "------------------------------------------------------------------\n";
}

int* heapBuildBySiftDown(int n, int* heap, int lastSheetParentIndex) {
    for (int i = lastSheetParentIndex; i >= 0; i--) {
        int currentIndex = i;
        while (true) {
            int leftChildIndex = 2 * currentIndex + 1;
            int rightChildIndex = 2 * currentIndex + 2;
            int largestIndex = currentIndex;

            if (leftChildIndex < n && heap[leftChildIndex] > heap[largestIndex]) {
                largestIndex = leftChildIndex;
            }

            if (rightChildIndex < n && heap[rightChildIndex] > heap[largestIndex]) {
                largestIndex = rightChildIndex;
            }

            if (largestIndex != currentIndex) {
                swap(heap[currentIndex], heap[largestIndex]);
                currentIndex = largestIndex;
            }
            else {
                break;
            }
        }
    }
    return heap;
}

int* creatingAnUnorderedHeap(int& n, int& lastSheetParentIndex) {
    wall();
    cout << "Введите количество элементов кучи: ";
    while (true) {
        cin >> n;
        if (n > 0) {
            break;
        }
        wall();
        cout << "Количество элементов должно быть положительным. Повторите ввод: ";
    }
    wall();

    lastSheetParentIndex = n / 2 - 1;

    int* heap = new int[n];

    for (int i = 0; i < n; i++) {
        cout << "Введите " << i + 1 << "-ый элемент: ";
        cin >> heap[i];
        wall();
    }

    system("pause");
    system("cls");
    return heap;
}

void printHeap(int* heap, int n) {
    cout << "Куча: ";
    for (int i = 0; i < n; i++) {
        cout << heap[i] << " ";
    }
    cout << endl;
}

void creatingAnOrderedHeap(int n, int* heap, int lastSheetParentIndex) {
    wall();
    cout << "Исходный массив: ";
    for (int i = 0; i < n; i++) {
        cout << heap[i] << " ";
    }
    cout << endl;

    heapBuildBySiftDown(n, heap, lastSheetParentIndex);
}

void addToHeap(int*& heap, int& n, int curr) {
    system("cls");
    wall();
    cout << "Введите новый элемент: ";
    cin >> curr;
    wall();
    ofstream tempFile("temp_heap.txt");
    for (int i = 0; i < n; i++) {
        tempFile << heap[i] << " ";
    }
    tempFile << curr;
    tempFile.close();

    delete[] heap;

    ifstream inputFile("temp_heap.txt");
    n++;
    heap = new int[n];

    for (int i = 0; i < n; i++) {
        inputFile >> heap[i];
    }
    inputFile.close();

    int lastSheetParentIndex = n / 2 - 1;
    heapBuildBySiftDown(n, heap, lastSheetParentIndex);

    remove("temp_heap.txt");
    system("pause");
    system("cls");

}

int searchMax(int*& heap, int& n) {
    wall();
    int maximum = heap[0];
    heap[0] = heap[n - 1];
    n--;
    int lastSheetParentIndex = n / 2 - 1;
    heapBuildBySiftDown(n, heap, lastSheetParentIndex);
    return maximum;
}

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