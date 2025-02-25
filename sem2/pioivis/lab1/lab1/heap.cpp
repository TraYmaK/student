#include <iostream>
#include <fstream>
#include <algorithm>
#include "heap.hpp"

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