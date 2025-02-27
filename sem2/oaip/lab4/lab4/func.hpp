#pragma once

struct Node;
void wall();
void clear();
void pushFace(Node*& head, Node*& end, int value);
void pushBack(Node*& head, Node*& end, int value);
void userPushFace(Node*& head, Node*& end, int value);
void userPushBack(Node*& head, Node*& end, int value);
int popFace(Node*& head, Node*& end, int value);
int popBack(Node*& head, Node*& end, int value);
void displayStackFace(Node*& head, Node*& end, int value);
void displayStackBack(Node*& head, Node*& end, int value);
void clearStack(Node*& head);
void averageCalculating(Node*& head, double& average);
void averageFaceReplace(Node*& head, double& average);
void averageBackReplace(Node*& end, double& average);
void menu(Node*& head, Node*& end, int stackSize, int value);
void method(Node*& head, Node*& end, double& average, int& choice, int& value, int& poppedFaceValue, int& poppedBackValue);