#pragma once

void wall();
int* heapBuildBySiftDown(int n, int* heap, int lastSheetParentIndex);
int* creatingAnUnorderedHeap(int& n, int& lastSheetParentIndex);
void printHeap(int* heap, int n);
void creatingAnOrderedHeap(int n, int* heap, int lastSheetParentIndex);
void addToHeap(int*& heap, int& n, int curr);
int searchMax(int*& heap, int& n);