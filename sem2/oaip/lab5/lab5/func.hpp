#pragma once

struct Node {
    char data;
    Node* next;

    Node(char value) : data(value), next(nullptr) {}
};
void wall();
void clear();
void push(Node*& head, char value);
char pop(Node*& head);
int prioritization(char operation);
void convertation(const char* expression, char* RVNexpression);
int menu();
void method(char* expression, char* RVNexpression);
void takeExpression(char* expression);
void printExpression(char* expression);
void printRPNExpression(char* RVNexpression);
void calcRPN(const char* RPNexpression);