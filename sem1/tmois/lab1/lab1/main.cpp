#include <iostream>

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
    setlocale(LC_ALL, "RU");

    double sizeA, sizeB;

    int StartUniverse, EndUniverse;

    wall();

    cout << "������� ������ universe: ";
    cin >> StartUniverse;

    wall();

    cout << "������� ����� universe: ";
    cin >> EndUniverse;

    while (StartUniverse > EndUniverse || StartUniverse == EndUniverse) {
        wall();
        cout << "����� ���������� ������ ���� ������ ������ ��� ������. ��������� ����: \n";
        cout << "������� ������ universe: ";
        cin >> StartUniverse;
        cout << "������� ����� universe: ";
        cin >> EndUniverse;
    }

    wall();

    cout << "������� �������� ��������� A: ";
    cin >> sizeA;

    wall();

    int UniverseLenght = EndUniverse - StartUniverse + 1;

    while (sizeA <= 0 || sizeA > UniverseLenght || round(sizeA) != sizeA) {
        cout << "������: �������� ������� ������ ���� ������������, ������������ � ������ ��� ����� ����� ����������." << endl << "��������� ����: ";
        cin >> sizeA;
    }

    int* A = new int[sizeA];

    for (int i = 0; i < sizeA; i++) {
        int element;
        while (true) {
            cout << "������� ������� " << i + 1 << " ��������� A: ";
            cin >> element;

            if (element < StartUniverse || element > EndUniverse) {
                cout << "������: ������� ������� �� ������� ����������. ��������� ����." << endl;
            }
            else if (check(A, i, element)) {
                cout << "������: ������� ��� ���������� � ��������� A. ��������� ����." << endl;
            }
            else {
                A[i] = element;
                break;
            }
        }
    }

    wall();

    cout << "������� �������� ��������� B: ";
    cin >> sizeB;

    wall();

    while (sizeB <= 0 || sizeB > UniverseLenght || round(sizeB) != sizeB) {
        cout << "������: �������� ������� ������ ���� ������������, ������������ � ������ ��� ����� ����� ����������." << endl << "��������� ����: ";
        cin >> sizeB;
    }

    int* B = new int[sizeB];

    for (int i = 0; i < sizeB; i++) {
        int element;
        while (true) {
            cout << "������� ������� " << i + 1 << " ��������� B: ";
            cin >> element;

            if (element < StartUniverse || element > EndUniverse) {
                cout << "������: ������� ������� �� ������� ����������. ��������� ����." << endl;
            }
            else if (check(B, i, element)) {
                cout << "������: ������� ��� ���������� � ��������� B. ��������� ����." << endl;
            }
            else {
                B[i] = element;
                break;
            }
        }
    }


    wall();

    int* UniverseResult = new int[UniverseLenght];
    int UniverseIndex = 0;

    for (int i = 0; i < UniverseLenght; i++) {
        UniverseResult[i] = StartUniverse++;
    }

    if (UniverseLenght == 0) {
        cout << "Universe �����������" << endl;
    }
    else {
        cout << "Universe: {";
        for (int i = 0; i < UniverseLenght; i++) {
            if (i == UniverseLenght - 1) {
                cout << UniverseResult[i] << "}" << endl;
            }
            else {
                cout << UniverseResult[i] << " ";
            }
        }
    }

    wall();

    for (int i = 0; i < sizeA; i++) {
        if (i == 0) {
            cout << "������ A: {" << A[i] << " ";
        }
        else if(i == sizeA - 1){
            cout << A[i] << "}" << endl;

        }
        else {
            cout << A[i] << " ";
        }
    }

    for (int i = 0; i < sizeB; i++) {
        if (i == 0) {
            cout << "������ B: {" << B[i] << " ";
        }
        else if (i == sizeB - 1) {
            cout << B[i] << "}" << endl;

        }
        else {
            cout << B[i] << " ";
        }
    }

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
    
    if (UnionIndex == 0) {
        cout << "����������� �����������" << endl;
    }
    else {
        cout << "�����������: {";
        for (int i = 0; i < UnionIndex; i++) {
            if (i == UnionIndex - 1) {
                cout << ResultOfTheMerger[i] << "}" << endl;
            }
            else {
                cout << ResultOfTheMerger[i] << " ";
            }
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

    if (IntersectionIndex == 0) {
        cout << "����������� �����������" << endl;
    }
    else {
        cout << "�����������: {";
        for (int i = 0; i < IntersectionIndex; i++) {
            if (i == IntersectionIndex - 1) {
                cout << IntersectionResult[i] << "}" << endl;
            }
            else {
                cout << IntersectionResult[i] << " ";
            }
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

    if (FirstDifferenceIndex == 0) {
        cout << "�������� A-B �����������" << endl;
    }
    else {
        cout << "�������� A-B: {";
        for (int i = 0; i < FirstDifferenceIndex; i++) {
            if (i == FirstDifferenceIndex - 1) {
                cout << FirstDifferenceResult[i] << "}" << endl;
            }
            else {
                cout << FirstDifferenceResult[i] << " ";
            }
        }
    }
    
    int* SecondDifferenceResult = new int[sizeB];
    int SecondDifferenceIndex = 0;

    for (int i = 0; i < sizeB; i++) {
        bool check = false;
        for (int j = 0; j < sizeA; j++) {
            if (B[i] == A[j]) {
                check = true;
                break;
            }
        }
        if (!check) {
            SecondDifferenceResult[SecondDifferenceIndex++] = B[i];
        }
    }

    if (SecondDifferenceIndex == 0) {
        cout << "�������� B-A �����������" << endl;
    }
    else {
        cout << "�������� B-A: {";
        for (int i = 0; i < SecondDifferenceIndex; i++) {
            if (i == SecondDifferenceIndex - 1) {
                cout << SecondDifferenceResult[i] << "}" << endl;
            }
            else {
                cout << SecondDifferenceResult[i] << " ";
            }
        }
    }
    
    int* SymmetricDifferenceResult = new int[ResultSize];
    int SymmetricDifferenceIndex = 0;

    for (int i = 0; i < FirstDifferenceIndex; i++) {
        SymmetricDifferenceResult[SymmetricDifferenceIndex++] = FirstDifferenceResult[i];
    }

    for (int i = 0; i < SecondDifferenceIndex; i++) {
        SymmetricDifferenceResult[SymmetricDifferenceIndex++] = SecondDifferenceResult[i];
    }

    if (SymmetricDifferenceIndex == 0) {
        cout << "�������������� �������� �����������" << endl;
    }
    else {
        cout << "�������������� ��������: {";
        for (int i = 0; i < SymmetricDifferenceIndex; i++) {
            if (i == SymmetricDifferenceIndex - 1) {
                cout << SymmetricDifferenceResult[i] << "}" << endl;
            }
            else {
                cout << SymmetricDifferenceResult[i] << " ";
            }
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

    if (FirstAdditionToTheUniverseIndex == 0) {
        cout << "���������� ���������� A �����������" << endl;
    }
    else {
        cout << "���������� �� ���������� A: {";
        for (int i = 0; i < FirstAdditionToTheUniverseIndex; i++) {
            if (i == FirstAdditionToTheUniverseIndex - 1) {
                cout << FirstAdditionToTheUniverseResult[i] << "}" << endl;
            }
            else {
                cout << FirstAdditionToTheUniverseResult[i] << " ";
            }
        }
    }

    wall();

    int SecondAdditionToTheUniverseIndex = 0;
    int* SecondAdditionToTheUniverseResult = new int[UniverseLenght];

    for (int i = 0; i < UniverseLenght; i++) {
        bool check = false;
        for (int j = 0; j < sizeB; j++) {
            if (B[j] == UniverseResult[i]) {
                check = true;
                break;
            }
        }
        if (!check) {
            SecondAdditionToTheUniverseResult[SecondAdditionToTheUniverseIndex++] = UniverseResult[i];
        }
    }

    if (SecondAdditionToTheUniverseIndex == 0) {
        cout << "���������� ���������� B �����������" << endl;
    }
    else {
        cout << "���������� �� ���������� B: {";
        for (int i = 0; i < SecondAdditionToTheUniverseIndex; i++) {
            if (i == SecondAdditionToTheUniverseIndex - 1) {
                cout << SecondAdditionToTheUniverseResult[i] << "}" << endl;
            }
            else {
                cout << SecondAdditionToTheUniverseResult[i] << " ";
            }
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