#pragma once

struct Node;
void wall();
void clear();
void push(Node*& head, int value);
int pop(Node*& head);
void displayStack(Node* head);
void clearStack(Node*& head);
void averageCalculating(Node*& head, double& average);
void averageReplace(Node*& head, double& average);
void menu(int stackSize, Node*& head);
void method(int& choice, Node*& head, double& average);