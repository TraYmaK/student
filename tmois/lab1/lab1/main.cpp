#include <iostream>

using namespace std;

void wall() {
    cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
}

int main() {
    setlocale(LC_ALL, "RU");

    int sizeA, sizeB;

    wall();

    cout << "Введите мощность множества A: ";
    cin >> sizeA;
    int* A = new int[sizeA];

    wall();

    for (int i = 0; i < sizeA; i++) {
        cout << "Введите элемент " << i+1 << " множества A: ";
        cin >> A[i];
    }

    wall();

    cout << "Введите мощность множества B: ";
    cin >> sizeB;
    int* B = new int[sizeB];

    wall();

    for (int i = 0; i < sizeB; i++) {
        cout << "Введите элемент " << i+1 << " множества B: ";
        cin >> B[i];
    }

    wall();

    int StartUniverse, EndUniverse;

    cout << "Введите начало universe: ";
    cin >> StartUniverse;

    wall();

    cout << "Введите конец universe: ";
    cin >> EndUniverse;

    wall();

    int ResultSize = sizeA + sizeB;
    int* ResultOfTheMerger = new int[ResultSize];
    int UnionIndex = 0;

    for (int i = 0; i < sizeA; i++) {
        ResultOfTheMerger[UnionIndex++] = A[i];
    }

    for (int i = 0; i < sizeB; i++) {
        bool check = false;
        for (int j = 0; j < sizeA; j++) {
            if (B[i] == A[j]) {
                check = true;
                break;
            }
        }
        if (!check) {
            ResultOfTheMerger[UnionIndex++] = B[i];
        }
    }
    
    cout << "Объединение: {";
    for (int i = 0; i < UnionIndex; i++) {
        if (i == UnionIndex - 1) {
            cout << ResultOfTheMerger[i] << "}" << endl;
        }
        else {
            cout << ResultOfTheMerger[i] << " ";
        }
    }

    delete[] ResultOfTheMerger;

    int* IntersectionResult = new int[ResultSize];
    int IntersectionIndex = 0;

    for (int i = 0; i < sizeA; i++) {
        for (int j = 0; j < sizeB; j++) {
            if (A[i] == B[j]) {
                IntersectionResult[IntersectionIndex++] = A[i];
                break;
            }
        }
    }

    cout << "Пересечение: {";
    for (int i = 0; i < IntersectionIndex; i++) {
        if (i == IntersectionIndex - 1) {
            cout << IntersectionResult[i] << "}" << endl;
        }
        else {
            cout << IntersectionResult[i] << " ";
        }
    }

    delete[] IntersectionResult;

    int* FirstDifferenceResult = new int[sizeA];
    int FirstDifferenceIndex = 0;

    for (int i = 0; i < sizeA; i++) {
        bool check = false;
        for (int j = 0; j < sizeB; j++) {
            if (A[i] == B[j]) {
                check = true;
                break;
            }
        }
        if (!check) {
            FirstDifferenceResult[FirstDifferenceIndex++] = A[i];
        }
    }

    cout << "Разность A-B: {";
    for (int i = 0; i < FirstDifferenceIndex; i++) {
        if (i == FirstDifferenceIndex - 1) {
            cout << FirstDifferenceResult[i] << "}" << endl;
        }
        else {
            cout << FirstDifferenceResult[i] << " ";
        }
    }

    wall();
    
    int* SecondDifferenceResult = new int[sizeB];
    int SecondDifferenceIndex = 0;

    for (int i = 0; i < sizeA; i++) {
        bool check = false;
        for (int j = 0; j < sizeB; j++) {
            if (A[j] == B[i]) {
                check = true;
                break;
            }
        }
        if (!check) {
            SecondDifferenceResult[SecondDifferenceIndex++] = B[i];
        }
    }

    cout << "Разность B-A: {";
    for (int i = 0; i < SecondDifferenceIndex; i++) {
        if (i == SecondDifferenceIndex - 1) {
            cout << SecondDifferenceResult[i] << "}" << endl;
        }
        else {
            cout << SecondDifferenceResult[i] << " ";
        }
    }

    wall();
    
    int* SymmetricDifferenceResult = new int[ResultSize];
    int SymmetricDifferenceIndex = 0;

    for (int i = 0; i < FirstDifferenceIndex; i++) {
        SymmetricDifferenceResult[SymmetricDifferenceIndex++] = FirstDifferenceResult[i];
    }

    for (int i = 0; i < SecondDifferenceIndex; i++) {
        SymmetricDifferenceResult[SymmetricDifferenceIndex++] = SecondDifferenceResult[i];
    }

    cout << "Симметрическая разность: {";
    for (int i = 0; i < SymmetricDifferenceIndex; i++) {
        if (i == SymmetricDifferenceIndex - 1) {
            cout << SymmetricDifferenceResult[i] << "}" << endl;
        }
        else {
            cout << SymmetricDifferenceResult[i] << " ";
        }
    }

    wall();

    int UniverseLenght = fabs(StartUniverse) + fabs(EndUniverse + 1);
    int* UniverseResult = new int[UniverseLenght];
    int UniverseIndex = 0;

    for (int i = 0; i < UniverseLenght; i++) {
            UniverseResult[i] = StartUniverse++;
    }

    cout << "Universe: {";
    for (int i = 0; i < UniverseLenght; i++) {
        if (i == UniverseLenght - 1) {
            cout << UniverseResult[i] << "}" << endl;
        }
        else {
            cout << UniverseResult[i] << " ";
        }
    }

    wall();
 
    int FirstAdditionToTheUniverseIndex = 0;
    int* FirstAdditionToTheUniverseResult = new int[UniverseLenght];

    for (int i = 0; i < UniverseLenght; i++) {
        bool check = false;
        for (int j = 0; j < sizeA; j++) {
            if (A[j] == UniverseResult[i]) {
                check = true;  
                break;
            }
        }
        if (!check) {
            FirstAdditionToTheUniverseResult[FirstAdditionToTheUniverseIndex++] = UniverseResult[i];
        }
    }

    cout << "Дополнение до универсума A: {";
    for (int i = 0; i < FirstAdditionToTheUniverseIndex; i++) {
        if (i == FirstAdditionToTheUniverseIndex - 1) {
            cout << FirstAdditionToTheUniverseResult[i] << "}" << endl;
        }
        else {
            cout << FirstAdditionToTheUniverseResult[i] << " ";
        }
    }


    wall();

    int SecondAdditionToTheUniverseIndex = 0;
    int* SecondAdditionToTheUniverseResult = new int[UniverseLenght];

    for (int i = 0; i < UniverseLenght; i++) {
        bool check = false;
        for (int j = 0; j < sizeA; j++) {
            if (B[j] == UniverseResult[i]) {
                check = true;
                break;
            }
        }
        if (!check) {
            SecondAdditionToTheUniverseResult[SecondAdditionToTheUniverseIndex++] = UniverseResult[i];
        }
    }

    cout << "Дополнение до универсума B: {";
    for (int i = 0; i < SecondAdditionToTheUniverseIndex; i++) {
        if (i == SecondAdditionToTheUniverseIndex - 1) {
            cout << SecondAdditionToTheUniverseResult[i] << "}" << endl;
        }
        else {
            cout << SecondAdditionToTheUniverseResult[i] << " ";
        }
    }

    wall();

    delete[] FirstAdditionToTheUniverseResult;
    delete[] SecondAdditionToTheUniverseResult;
    delete[] FirstDifferenceResult;
    delete[] SecondDifferenceResult;
    delete[] SymmetricDifferenceResult;
    delete[] UniverseResult;
    delete[] A;
    delete[] B;

	return 0;
}