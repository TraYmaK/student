#include <iostream>

using namespace std;

void wall() {
    cout << "------------------------------------------------------------------------------------------------------------------------\n";
}

void Method(int& method) {
    while (true) {
        cout << "Методы задания графика: \n";
        cout << "1. Метод перечисления \n";
        cout << "2. Метод высказывания \n";
        wall();
        cout << "Выберите метод задания графика: ";
        cin >> method;

        if (method == 1 || method == 2) {
            break;
        }
        wall();
        cout << "Выберите корректный метод (1 или 2): \n";
        wall();
    }
}

int main() {
    setlocale(LC_ALL, "RU");

    int FirstGraphDimension = 0, SecondGraphDimension = 0, TupleDimension = 2, FirstMethod, SecondMethod, Operation;
    int** FirstGraph = nullptr;
    int** SecondGraph = nullptr;

    wall();

    cout << "Введите размерность первого графика: ";
    while (true) {
        cin >> FirstGraphDimension;
        if (FirstGraphDimension > 0) {
            break;
        }
        cout << "Размерность графика должна быть положительной. Повторите ввод: ";
    }

    wall();

    Method(FirstMethod);

    if (FirstMethod == 1) {
        FirstGraph = new int* [FirstGraphDimension];
        for (int i = 0; i < FirstGraphDimension; ++i) {
            FirstGraph[i] = new int[TupleDimension];
        }

        wall();

        for (int i = 0; i < FirstGraphDimension; ++i) {
            bool isUnique, isValid;
            do {
                isUnique = true;
                isValid = true;
                cout << "Введите элементы кортежа " << i + 1 << " : ";
                cin >> FirstGraph[i][0] >> FirstGraph[i][1];

                if (FirstGraph[i][0] > 100 || FirstGraph[i][1] > 100) {
                    cout << "Элементы кортежей не должны превышать значение 100. Попробуйте снова.\n";
                    isValid = false;
                    continue;
                }

                for (int j = 0; j < i; ++j) {
                    if (FirstGraph[i][0] == FirstGraph[j][0] && FirstGraph[i][1] == FirstGraph[j][1]) {
                        cout << "Такой кортеж уже существует. Введите другой.\n";
                        isUnique = false;
                        break;
                    }
                }
            } while (!isUnique || !isValid);
            wall();
        }
    }

    else if (FirstMethod == 2) {
        FirstGraph = new int* [FirstGraphDimension];
        for (int i = 0; i < FirstGraphDimension; ++i) {
            FirstGraph[i] = new int[TupleDimension];
        }

        int FirstValue = 1;
        for (int i = 0; i < FirstGraphDimension; ++i) {
            for (int j = 0; j < TupleDimension; ++j) {
                FirstGraph[i][j] = FirstValue;
                FirstValue += 2;
            }
        }
        wall();
    }

    cout << "Введите размерность второго графика: ";
    while (true) {
        cin >> SecondGraphDimension;
        if (SecondGraphDimension > 0) {
            break;
        }
        cout << "Размерность графика должна быть положительной. Повторите ввод: ";
    }

    wall();

    Method(SecondMethod);

    if (SecondMethod == 1) {
        SecondGraph = new int* [SecondGraphDimension];
        for (int i = 0; i < SecondGraphDimension; ++i) {
            SecondGraph[i] = new int[TupleDimension];
        }

        wall();

        for (int i = 0; i < SecondGraphDimension; ++i) {
            bool isUnique, isValid;
            do {
                isUnique = true;
                isValid = true;
                cout << "Введите элементы кортежа " << i + 1 << " : ";
                cin >> SecondGraph[i][0] >> SecondGraph[i][1];

                if (SecondGraph[i][0] > 100 || SecondGraph[i][1] > 100) {
                    cout << "Элементы кортежей не должны превышать значение 100. Попробуйте снова.\n";
                    isValid = false;
                    continue;
                }

                for (int j = 0; j < i; ++j) {
                    if (SecondGraph[i][0] == SecondGraph[j][0] && SecondGraph[i][1] == SecondGraph[j][1]) {
                        cout << "Такой кортеж уже существует. Введите другой.\n";
                        isUnique = false;
                        break;
                    }
                }
            } while (!isUnique || !isValid);
            wall();
        }
    }

    else if (SecondMethod == 2) {
        SecondGraph = new int* [SecondGraphDimension];
        for (int i = 0; i < SecondGraphDimension; ++i) {
            SecondGraph[i] = new int[TupleDimension];
        }

        int SecondValue = 1;
        for (int i = 0; i < SecondGraphDimension; ++i) {
            for (int j = 0; j < TupleDimension; ++j) {
                SecondGraph[i][j] = SecondValue;
                SecondValue += 2;
            }
        }
        wall();
    }

    system("cls");

    wall();

    cout << "Первый график:\n";
    for (int i = 0; i < FirstGraphDimension; ++i) {
        if (i == 0) cout << "{";
        cout << "<" << FirstGraph[i][0] << ", " << FirstGraph[i][1] << ">";
        if (i < FirstGraphDimension - 1) {
            cout << ", ";
        }
        if (i == FirstGraphDimension - 1) {
            cout << "}";
        }
    }
    cout << endl;

    wall();

    cout << "Второй график:\n";
    for (int i = 0; i < SecondGraphDimension; ++i) {
        if (i == 0) cout << "{";
        cout << "<" << SecondGraph[i][0] << ", " << SecondGraph[i][1] << ">";
        if (i < SecondGraphDimension - 1) {
            cout << ", ";
        }
        if (i == SecondGraphDimension - 1) {
            cout << "}";
        }
    }
    cout << endl;

    wall();

    cout << "Операции над графиками: \n\n";
    cout << "1. Инверсия \n";
    cout << "2. Композиция \n";
    cout << "3. Проекция на оси \n";
    cout << "4. Декартово произведение \n";

    wall();

    cout << "Выбирите операцию: ";
    cin >> Operation;

    if (Operation == 1) {

        system("cls");
        
        wall();

        cout << "Инверсия: \n";

        wall();

        int** FirstInversion = new int* [FirstGraphDimension];
        for (int i = 0; i < FirstGraphDimension; ++i) {
            FirstInversion[i] = new int[TupleDimension];
        }

        int** SecondInversion = new int* [SecondGraphDimension];
        for (int i = 0; i < SecondGraphDimension; ++i) {
            SecondInversion[i] = new int[TupleDimension];
        }

        for (int i = 0; i < FirstGraphDimension; ++i) {
            FirstInversion[i][0] = FirstGraph[i][1];
            FirstInversion[i][1] = FirstGraph[i][0];
        }

        for (int i = 0; i < SecondGraphDimension; ++i) {
            SecondInversion[i][0] = SecondGraph[i][1];
            SecondInversion[i][1] = SecondGraph[i][0];
        }

        for (int i = 0; i < FirstGraphDimension; ++i) {
            if (i == 0) cout << "{";
            cout << "<" << FirstInversion[i][0] << ", " << FirstInversion[i][1] << ">";
            if (i < FirstGraphDimension - 1) {
                cout << ", ";
            }
            if (i == FirstGraphDimension - 1) {
                cout << "}";
            }
        }
        cout << endl;

        for (int i = 0; i < SecondGraphDimension; ++i) {
            if (i == 0) cout << "{";
            cout << "<" << SecondInversion[i][0] << ", " << SecondInversion[i][1] << ">";
            if (i < SecondGraphDimension - 1) {
                cout << ", ";
            }
            if (i == SecondGraphDimension - 1) {
                cout << "}";
            }
        }
        cout << endl;

        wall();

    }
    else if (Operation == 2) {
        system("cls");

        wall();

        cout << "Композиция графиков 1 и 2: ";
        
        int FirstCounter = 0;

        for (int i = 0; i < FirstGraphDimension; ++i) {
            for (int j = 0; j < SecondGraphDimension; ++j) {
                if (FirstGraph[i][1] == SecondGraph[j][0]) {
                    FirstCounter++;
                }
            }
        }

        if (FirstCounter == 0) {
            cout << "{}\n";
            wall();
        }
        else {
            int** FirstComposition = new int* [FirstCounter];

            for (int i = 0; i < FirstCounter; ++i) {
                FirstComposition[i] = new int[TupleDimension];
            }

            int FirstIndex = 0;
            for (int i = 0; i < FirstGraphDimension; ++i) {
                for (int j = 0; j < SecondGraphDimension; ++j) {
                    if (FirstGraph[i][1] == SecondGraph[j][0]) {
                        FirstComposition[FirstIndex][0] = FirstGraph[i][0];
                        FirstComposition[FirstIndex][1] = SecondGraph[j][1];
                        FirstIndex++;
                    }
                }
            }

            cout << endl;

            for (int i = 0; i < FirstCounter; ++i) {
                if (i == 0) cout << "{";
                cout << "<" << FirstComposition[i][0] << ", " << FirstComposition[i][1] << ">";
                if (i < FirstCounter - 1) {
                    cout << ", ";
                }
                if (i == FirstCounter - 1) {
                    cout << "}";
                }
            }
            cout << endl;

            wall();
        }
        
        cout << "Композиция графиков 2 и 1: \n";

        int SecondCounter = 0;

        for (int i = 0; i < SecondGraphDimension; ++i) {
            for (int j = 0; j < FirstGraphDimension; ++j) {
                if (SecondGraph[i][1] == FirstGraph[j][0]) {
                    SecondCounter++;
                }
            }
        }

        if (SecondCounter == 0) {
            cout << "{}\n";
            wall();
        }
        else {
            int** SecondComposition = new int* [SecondCounter];
            for (int i = 0; i < SecondCounter; ++i) {
                SecondComposition[i] = new int[TupleDimension];
            }

            int SecondIndex = 0;
            for (int i = 0; i < SecondGraphDimension; ++i) {
                for (int j = 0; j < FirstGraphDimension; ++j) {
                    if (SecondGraph[i][1] == FirstGraph[j][0]) {
                        SecondComposition[SecondIndex][0] = SecondGraph[i][0];
                        SecondComposition[SecondIndex][1] = FirstGraph[j][1];
                        SecondIndex++;
                    }
                }
            }

            cout << "{";
            for (int i = 0; i < SecondCounter; ++i) {
                cout << "<" << SecondComposition[i][0] << ", " << SecondComposition[i][1] << ">";
                if (i < SecondCounter - 1) {
                    cout << ", ";
                }
            }
            cout << "}\n";

            wall();
        }
    }
    else if (Operation == 3) {
        system("cls");

        wall();

        cout << "Проекции на оси: \n";

        wall();

        cout << "Проекция первого графика на ось X: [";
        for (int i = 0; i < FirstGraphDimension; ++i) {
            cout << FirstGraph[i][0];
            if (i < FirstGraphDimension - 1) {
                cout << ", ";
            }
        }
        cout << "]\n";

        cout << "Проекция первого графика на ось Y: [";
        for (int i = 0; i < FirstGraphDimension; ++i) {
            cout << FirstGraph[i][1];
            if (i < FirstGraphDimension - 1) {
                cout << ", ";
            }
        }
        cout << "]\n";

        wall();

        cout << "Проекция второго графика на ось X: [";
        for (int i = 0; i < SecondGraphDimension; ++i) {
            cout << SecondGraph[i][0];
            if (i < SecondGraphDimension - 1) {
                cout << ", ";
            }
        }
        cout << "]\n";

        cout << "Проекция второго графика на ось Y: [";
        for (int i = 0; i < SecondGraphDimension; ++i) {
            cout << SecondGraph[i][1];
            if (i < SecondGraphDimension - 1) {
                cout << ", ";
            }
        }
        cout << "]\n";

        wall();
    }
    else if (Operation == 4) {
        system("cls");

        wall();

        cout << "Декартово произведение графиков 1 и 2: ";

        cout << endl;

        wall();

        int CartesianSize = FirstGraphDimension * SecondGraphDimension;
        int** CartesianProduct = new int* [CartesianSize];

        for (int i = 0; i < CartesianSize; ++i) {
            CartesianProduct[i] = new int[4];
        }

        int index = 0;
        for (int i = 0; i < FirstGraphDimension; ++i) {
            for (int j = 0; j < SecondGraphDimension; ++j) {
                CartesianProduct[index][0] = FirstGraph[i][0];
                CartesianProduct[index][1] = FirstGraph[i][1];
                CartesianProduct[index][2] = SecondGraph[j][0];
                CartesianProduct[index][3] = SecondGraph[j][1];
                index++;
            }
        }

        cout << "{";
        for (int i = 0; i < CartesianSize; ++i) {
            cout << "<" << CartesianProduct[i][0] << ", " << CartesianProduct[i][1]
                << ", " << CartesianProduct[i][2] << ", " << CartesianProduct[i][3] << ">";
            if (i < CartesianSize - 1) {
                cout << ", ";
            }
        }
        cout << "}\n";

        wall();
    }
    else {
        exit(1);
    }

    for (int i = 0; i < FirstGraphDimension; ++i) {
        delete[] FirstGraph[i];
    }
    delete[] FirstGraph;

    for (int i = 0; i < SecondGraphDimension; ++i) {
        delete[] SecondGraph[i];
    }
    delete[] SecondGraph;

    return 0;
}
