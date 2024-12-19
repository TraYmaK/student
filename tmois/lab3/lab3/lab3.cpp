#include <iostream>
#include <locale>
#include <cmath>

using namespace std;

void wall() {
    cout << "--------------------------------------------------------------------------------------------------------------" << endl;
}

bool check(int* per1, int size, int per2) {
    for (int i = 0; i < size; i++) {
        if (per1[i] == per2) {
            return true;
        }
    }
    return false;
}

int main() {
    setlocale(LC_ALL, "ru");

    int FirstSizeX;

    wall();

    cout << "Введите мощность множества X: ";
    cin >> FirstSizeX;

    wall();

    while (FirstSizeX <= 0 || round(FirstSizeX) != FirstSizeX) {
        cout << "Ошибка: мощность множества должна быть целочисленна и положительна." << endl << "Повторите ввод: ";
        cin >> FirstSizeX;
    }

    int* FirstX = new int[FirstSizeX];

    for (int i = 0; i < FirstSizeX; i++) {
        int element;
        while (true) {
            cout << "Введите элемент " << i + 1 << " множества X: ";
            cin >> element;

            if (check(FirstX, i, element)) {
                cout << "Ошибка: элемент уже существует в множестве X. Повторите ввод." << endl;
            }
            else {
                FirstX[i] = element;
                break;
            }
        }
    }

    int FirstSizeY;

    wall();

    cout << "Введите мощность множества Y: ";
    cin >> FirstSizeY;

    wall();

    while (FirstSizeY <= 0 || round(FirstSizeY) != FirstSizeY) {
        cout << "Ошибка: мощность множества должна быть целочисленна и положительна." << endl << "Повторите ввод: ";
        cin >> FirstSizeY;
    }

    int* FirstY = new int[FirstSizeY];

    for (int i = 0; i < FirstSizeY; i++) {
        int element;
        while (true) {
            cout << "Введите элемент " << i + 1 << " множества Y: ";
            cin >> element;

            if (check(FirstY, i, element)) {
                cout << "Ошибка: элемент уже существует в множестве Y. Повторите ввод." << endl;
            }
            else {
                FirstY[i] = element;
                break;
            }
        }
    }

    wall();

    system("pause");

    system("cls");

    wall();

    int SecondSizeX;

    cout << "Введите мощность множества X: ";
    cin >> SecondSizeX;

    wall();

    while (SecondSizeX <= 0 || round(SecondSizeX) != SecondSizeX) {
        cout << "Ошибка: мощность множества должна быть целочисленна и положительна." << endl << "Повторите ввод: ";
        cin >> SecondSizeX;
    }

    int* SecondX = new int[SecondSizeX];

    for (int i = 0; i < SecondSizeX; i++) {
        int element;
        while (true) {
            cout << "Введите элемент " << i + 1 << " множества X: ";
            cin >> element;

            if (check(SecondX, i, element)) {
                cout << "Ошибка: элемент уже существует в множестве X. Повторите ввод." << endl;
            }
            else {
                SecondX[i] = element;
                break;
            }
        }
    }

    int SecondSizeY;

    wall();

    cout << "Введите мощность множества Y: ";
    cin >> SecondSizeY;

    wall();

    while (SecondSizeY <= 0 || round(SecondSizeY) != SecondSizeY) {
        cout << "Ошибка: мощность множества должна быть целочисленна и положительна." << endl << "Повторите ввод: ";
        cin >> SecondSizeY;
    }

    int* SecondY = new int[SecondSizeY];

    for (int i = 0; i < SecondSizeY; i++) {
        int element;
        while (true) {
            cout << "Введите элемент " << i + 1 << " множества Y: ";
            cin >> element;

            if (check(SecondY, i, element)) {
                cout << "Ошибка: элемент уже существует в множестве Y. Повторите ввод." << endl;
            }
            else {
                SecondY[i] = element;
                break;
            }
        }
    }

    wall();

    system("pause");

    system("cls");

    wall();

    int NumberOfOperationsForX;
    int NumberOfOperationsForY;

    cout << "Введите количетсво комбинаций для X: ";    
    cin >> NumberOfOperationsForX;

    wall();

    cout << "Введите количетсво комбинаций для Y: ";
    cin >> NumberOfOperationsForY;

    wall();

    system("pause");

    system("cls");

    wall();

    int FirstGraphDimension = FirstSizeX * FirstSizeY;
    int TupleDimension = 2;

    int** FirstGraph = new int* [FirstGraphDimension];

    for (int i = 0; i < FirstGraphDimension; ++i) {
        FirstGraph[i] = new int[TupleDimension];
    }

    int FirstIndexX = 0;
    int FirstIndexY = 0;

    for (int i = 0; i < FirstGraphDimension; i++) {
        FirstGraph[i][0] = FirstX[FirstIndexX];
        if ((i + 1) % FirstSizeY == 0) {
            FirstIndexX++;
        }
    }


    for (int i = 0; i < FirstGraphDimension; i++) {
        FirstGraph[i][1] = FirstY[FirstIndexY];
        FirstIndexY++;
        if (FirstIndexY == FirstSizeY) {
            FirstIndexY = 0;
        }
    }

    int SecondGraphDimension = SecondSizeX * SecondSizeY;

    int** SecondGraph = new int* [SecondGraphDimension];

    for (int i = 0; i < SecondGraphDimension; ++i) {
        SecondGraph[i] = new int[TupleDimension];
    }

    int SecondIndexX = 0;
    int SecondIndexY = 0;

    for (int i = 0; i < SecondGraphDimension; i++) {
        SecondGraph[i][0] = SecondX[SecondIndexX];
        if ((i + 1) % SecondSizeY == 0) {
            SecondIndexX++;
        }
    }


    for (int i = 0; i < SecondGraphDimension; i++) {
        SecondGraph[i][1] = SecondY[SecondIndexY];
        SecondIndexY++;
        if (SecondIndexY == SecondSizeY) {
            SecondIndexY = 0;
        }
    }

    int ResultFirstGraphDimension = NumberOfOperationsForX;

    int ResultSecondGraphDimension = NumberOfOperationsForY;

    int** ResultFirstGraph = new int* [NumberOfOperationsForX];

    int** ResultSecondGraph = new int* [NumberOfOperationsForY];


    for (int i = 0; i < NumberOfOperationsForX; i++) {
        int randomIndex = rand() % FirstGraphDimension;
        ResultFirstGraph[i] = FirstGraph[randomIndex];
    }

    for (int i = 0; i < NumberOfOperationsForY; i++) {
        int randomIndex = rand() % SecondGraphDimension;
        ResultSecondGraph[i] = SecondGraph[randomIndex];
    }

    for (int i = 0; i < FirstSizeX; i++) {
        if (i == 0) {
            cout << "Множество X: {" << FirstX[i] << " ";
        }
        else if (i == FirstSizeX - 1) {
            cout << FirstX[i] << "}" << endl;

        }
        else {
            cout << FirstX[i] << " ";
        }
    }

    wall();

    for (int i = 0; i < FirstSizeY; i++) {
        if (i == 0) {
            cout << "Множество Y: {" << FirstY[i] << " ";
        }
        else if (i == FirstSizeY - 1) {
            cout << FirstY[i] << "}" << endl;

        }
        else {
            cout << FirstY[i] << " ";
        }
    }

    wall();

    cout << "X: ";
    for (int i = 0; i < ResultFirstGraphDimension; i++) {
        if (i == 0) cout << "{";
        cout << "<" << ResultFirstGraph[i][0] << ", " << ResultFirstGraph[i][1] << ">";
        if (i < ResultFirstGraphDimension - 1) {
            cout << ", ";
        }
        if (i == ResultFirstGraphDimension - 1) {
            cout << "}";
        }
    }
    cout << endl;

    wall();

    for (int i = 0; i < SecondSizeX; i++) {
        if (i == 0) {
            cout << "Множество X: {" << SecondX[i] << " ";
        }
        else if (i == SecondSizeX - 1) {
            cout << SecondX[i] << "}" << endl;

        }
        else {
            cout << SecondX[i] << " ";
        }
    }

    wall();

    for (int i = 0; i < SecondSizeY; i++) {
        if (i == 0) {
            cout << "Множество Y: {" << SecondY[i] << " ";
        }
        else if (i == SecondSizeY - 1) {
            cout << SecondY[i] << "}" << endl;

        }
        else {
            cout << SecondY[i] << " ";
        }
    }

    wall();

    cout << "Y: ";
    for (int i = 0; i < ResultSecondGraphDimension; i++) {
        if (i == 0) cout << "{";
        cout << "<" << ResultSecondGraph[i][0] << ", " << ResultSecondGraph[i][1] << ">";
        if (i < ResultSecondGraphDimension - 1) {
            cout << ", ";
        }
        if (i == ResultSecondGraphDimension - 1) {
            cout << "}";
        }
    }
    cout << endl;

    wall();

    system("pause");

    system("cls");

    while (true) {

        wall();

        cout << "Список операций:\n";

        wall();

        cout << "1)Обьединение\n2)Пересечение\n3)Разность\n4)Инверсия\n5)Композиция\n";

        wall();

        cout << "Выберите операцию: ";

        int Operation;

        cin >> Operation;

        wall();

        if (Operation == 1) {

            int UnionSize = 0;

            for (int i = 0; i < ResultFirstGraphDimension; i++) {
                for (int j = 0; j < ResultSecondGraphDimension; j++) {
                    if (ResultFirstGraph[i] != ResultSecondGraph[j]) {
                        UnionSize++;
                    }
                }
            }

            int** Union = new int* [UnionSize];

            for (int i = 0; i < UnionSize; i++) {
                delete[] SecondGraph[i];
            }
            delete[] SecondGraph;
        }
        else if (Operation == 2) {

        }
        else if (Operation == 3) {

        }
        else if (Operation == 4) {

        }
        else if (Operation == 5) {

        }
        else {
            exit(1);
        }

    }

    for (int i = 0; i < FirstGraphDimension; i++) {
        delete[] FirstGraph[i];
    }
    delete[] FirstGraph;

    delete[] FirstX;
    delete[] FirstY;

    for (int i = 0; i < SecondGraphDimension; i++) {
        delete[] SecondGraph[i];
    }
    delete[] SecondGraph;

    delete[] SecondX;
    delete[] SecondY;
}