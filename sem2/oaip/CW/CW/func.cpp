#include <iostream>
#include <fstream>
#include <cstring>
#include "func.hpp"

using namespace std;

void wall() {
    cout << "--------------------------------------------------------------------------------------" << endl;
}

void clear() {
    system("pause");
    system("cls");
}

void createFile(const char* filename) {
    ofstream file(filename, ios::binary | ios::trunc);
    file.close();
}


void readFile(const char* filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "�� ������� ������� ���� ��� ������!\n";
        return;
    }
    Vacancy vac;
    bool foundAny = false;
    while (true) {
        file.read(reinterpret_cast<char*>(&vac), sizeof(Vacancy));
        if (!file.good()) {
            break;
        }
        foundAny = true;
        cout << "ID: " << vac.id << "\n";
        cout << "���������: " << vac.title << "\n";
        cout << "��������: " << vac.company << "\n";
        cout << "��������: " << vac.salary << "\n";
        cout << "����������: " << vac.requirements << "\n";
        cout << "���� ����������: " << vac.publicationDate << "\n";
        cout << "������: " << (vac.isActive ? "�������" : "�������") << "\n";
        wall();
    }
    file.close();
    if (!foundAny) {
        cout << "���� ���� ��� �������� �� �������.\n";
    }
}

void addVacancy(const char* filename) {
    ofstream file(filename, ios::binary | ios::app);
    if (!file) {
        cerr << "�� ������� ������� ���� ��� ������.\n";
        return;
    }
    Vacancy vac;
    cout << "���������� ����� ��������:\n";
    wall();
    inputVacancyFromUser(vac);
    file.write(reinterpret_cast<const char*>(&vac), sizeof(Vacancy));
    file.close();
    cout << "�������� ������� ���������.\n";
}

void updateVacancy(const char* filename, int vacancyId) {
    int recordCount = getRecordCount(filename);
    if (recordCount == 0) {
        cout << "���� ���� ��� �� ������.\n";
        return;
    }
    Vacancy* vacancies = new Vacancy[recordCount];
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "�� ������� ������� ���� ��� ������.\n";
        delete[] vacancies;
        return;
    }
    inFile.read(reinterpret_cast<char*>(vacancies), recordCount * sizeof(Vacancy));
    inFile.close();
    bool found = false;
    for (int i = 0; i < recordCount; i++) {
        if (vacancies[i].id == vacancyId) {
            cout << "������� ������ � ��������:\n";
            wall();
            cout << "ID: " << vacancies[i].id << "\n"
                << "���������: " << vacancies[i].title << "\n"
                << "��������: " << vacancies[i].company << "\n"
                << "��������: " << vacancies[i].salary << "\n"
                << "����������: " << vacancies[i].requirements << "\n"
                << "���� ����������: " << vacancies[i].publicationDate << "\n"
                << "������: " << (vacancies[i].isActive ? "�������" : "�������") << "\n";
            wall();
            cout << "������� ����� ������:\n";
            wall();
            inputVacancyFromUser(vacancies[i]);
            vacancies[i].id = vacancyId;
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "�������� � ����� ID �� �������!\n";
        delete[] vacancies;
        return;
    }
    ofstream outFile(filename, ios::binary | ios::trunc);
    if (!outFile) {
        cerr << "�� ������� ������� ���� ��� ������!\n";
        delete[] vacancies;
        return;
    }
    outFile.write(reinterpret_cast<const char*>(vacancies), recordCount * sizeof(Vacancy));
    outFile.close();
    delete[] vacancies;
    cout << "�������� ������� ���������.\n";
}

void deleteVacancy(const char* filename, int vacancyId) {
    int recordCount = getRecordCount(filename);
    if (recordCount == 0) {
        cout << "���� ���� ��� �� ������.\n";
        return;
    }
    Vacancy* vacancies = new Vacancy[recordCount];
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "�� ������� ������� ���� ��� ������!\n";
        delete[] vacancies;
        return;
    }
    inFile.read(reinterpret_cast<char*>(vacancies), recordCount * sizeof(Vacancy));
    inFile.close();
    int newCount = 0;
    for (int i = 0; i < recordCount; i++) {
        if (vacancies[i].id != vacancyId) {
            newCount++;
        }
    }
    if (newCount == recordCount) {
        cout << "�������� � ID " << vacancyId << " �� �������.\n";
        delete[] vacancies;
        return;
    }
    Vacancy* newVacancies = new Vacancy[newCount];
    int index = 0;
    for (int i = 0; i < recordCount; i++) {
        if (vacancies[i].id != vacancyId) {
            newVacancies[index++] = vacancies[i];
        }
    }
    ofstream outFile(filename, ios::binary | ios::trunc);
    if (!outFile) {
        cerr << "�� ������� ������� ���� ��� ������!\n";
        delete[] vacancies;
        delete[] newVacancies;
        return;
    }
    outFile.write(reinterpret_cast<const char*>(newVacancies), newCount * sizeof(Vacancy));
    outFile.close();
    delete[] vacancies;
    delete[] newVacancies;
    cout << "�������� � ID " << vacancyId << " ������� �������.\n";
}

int getRecordCount(const char* filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        return 0;
    }
    file.seekg(0, ios::end);
    streamoff fileSize = file.tellg();
    file.close();
    if (fileSize <= 0) {
        return 0;
    }
    return static_cast<int>(fileSize / sizeof(Vacancy));
}

void inputVacancyFromUser(Vacancy& vac) {
    cout << "������� ID ��������: ";
    cin >> vac.id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    wall();
    cout << "������� �������� ���������: ";
    cin.getline(vac.title, 50);
    wall();
    cout << "������� �������� ��������: ";
    cin.getline(vac.company, 50);
    wall();
    cout << "������� ��������: ";
    cin >> vac.salary;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    wall();
    cout << "������� ���������� (����� �������): ";
    cin.getline(vac.requirements, 100);
    wall();
    cout << "������� ���� ���������� (��.��.����): ";
    cin.getline(vac.publicationDate, 20);
    wall();
    cout << "������� ������ (1 = �������, 0 = �������): ";
    int status;
    cin >> status;
    vac.isActive = (status == 1);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    wall();
}

void linearSearchBySalary(const char* filename) {
    int recordCount = getRecordCount(filename);
    if (recordCount == 0) {
        cout << "���� ���� ��� �� ������.\n";
        return;
    }
    Vacancy* vacancies = new Vacancy[recordCount];
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "�� ������� ������� ���� ��� ������!\n";
        delete[] vacancies;
        return;
    }
    inFile.read(reinterpret_cast<char*>(vacancies), recordCount * sizeof(Vacancy));
    inFile.close();
    double targetSalary;
    cout << "������� �������� ��� ��������� ������: ";
    cin >> targetSalary;
    wall();
    bool found = false;
    for (int i = 0; i < recordCount; i++) {
        if (vacancies[i].salary == targetSalary) {
            found = true;
            cout << "������� �������� � ��������� " << targetSalary << ":\n";
            cout << "ID: " << vacancies[i].id << "\n";
            cout << "���������: " << vacancies[i].title << "\n";
            cout << "��������: " << vacancies[i].company << "\n";
            cout << "��������: " << vacancies[i].salary << "\n";
            cout << "����������: " << vacancies[i].requirements << "\n";
            cout << "���� ����������: " << vacancies[i].publicationDate << "\n";
            cout << "������: " << (vacancies[i].isActive ? "�������" : "�������") << "\n";
            wall();
        }
    }
    if (!found) {
        cout << "�������� � ��������� " << targetSalary << " �� �������.\n";
        wall();
    }
    delete[] vacancies;
}

static void sortArrayByTitle(Vacancy* arr, int n) {
    for (int i = 1; i < n; i++) {
        Vacancy key = arr[i];
        int j = i - 1;
        while (j >= 0 && strcmp(arr[j].title, key.title) > 0) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void binarySearchByTitle(const char* filename) {
    int recordCount = getRecordCount(filename);
    if (recordCount == 0) {
        cout << "���� ���� ��� �� ������.\n";
        return;
    }
    Vacancy* vacancies = new Vacancy[recordCount];
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "�� ������� ������� ���� ��� ������!\n";
        delete[] vacancies;
        return;
    }
    inFile.read(reinterpret_cast<char*>(vacancies), recordCount * sizeof(Vacancy));
    inFile.close();
    sortArrayByTitle(vacancies, recordCount);
    char targetTitle[50];
    cout << "������� ��������� ��� ��������� ������: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.getline(targetTitle, 50);
    wall();
    int left = 0;
    int right = recordCount - 1;
    bool found = false;
    while (left <= right) {
        int mid = (left + right) / 2;
        int cmp = strcmp(vacancies[mid].title, targetTitle);
        if (cmp == 0) {
            found = true;
            int temp = mid;
            while (temp >= 0 && strcmp(vacancies[temp].title, targetTitle) == 0) {
                temp--;
            }
            temp++;
            while (temp < recordCount && strcmp(vacancies[temp].title, targetTitle) == 0) {
                cout << "������� ��������:\n";
                cout << "ID: " << vacancies[temp].id << "\n";
                cout << "���������: " << vacancies[temp].title << "\n";
                cout << "��������: " << vacancies[temp].company << "\n";
                cout << "��������: " << vacancies[temp].salary << "\n";
                cout << "����������: " << vacancies[temp].requirements << "\n";
                cout << "���� ����������: " << vacancies[temp].publicationDate << "\n";
                cout << "������: " << (vacancies[temp].isActive ? "�������" : "�������") << "\n";
                wall();
                temp++;
            }
            break;
        }
        else if (cmp < 0) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    if (!found) {
        cout << "�������� � ���������� '" << targetTitle << "' �� �������.\n";
        wall();
    }
    delete[] vacancies;
}

static void swapVacancy(Vacancy& a, Vacancy& b) {
    Vacancy temp = a;
    a = b;
    b = temp;
}

static int partition(Vacancy* arr, int low, int high) {
    double pivot = arr[high].salary;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j].salary < pivot) {
            i++;
            swapVacancy(arr[i], arr[j]);
        }
    }
    swapVacancy(arr[i + 1], arr[high]);
    return (i + 1);
}

static void quickSortRec(Vacancy* arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortRec(arr, low, pi - 1);
        quickSortRec(arr, pi + 1, high);
    }
}

void quickSortBySalary(const char* sourceFilename, const char* sortedFilename) {
    int recordCount = getRecordCount(sourceFilename);
    if (recordCount == 0) {
        cout << "�������� ���� ���� ��� �� ������.\n";
        return;
    }
    Vacancy* vacancies = new Vacancy[recordCount];
    ifstream inFile(sourceFilename, ios::binary);
    if (!inFile) {
        cerr << "�� ������� ������� �������� ����!\n";
        delete[] vacancies;
        return;
    }
    inFile.read(reinterpret_cast<char*>(vacancies), recordCount * sizeof(Vacancy));
    inFile.close();
    quickSortRec(vacancies, 0, recordCount - 1);
    ofstream outFile(sortedFilename, ios::binary | ios::trunc);
    if (!outFile) {
        cerr << "�� ������� ������� ���� ��� ��������������� ������!\n";
        delete[] vacancies;
        return;
    }
    outFile.write(reinterpret_cast<const char*>(vacancies), recordCount * sizeof(Vacancy));
    outFile.close();
    cout << "��������, ��������������� �� �������� (quick sort):\n";
    wall();
    for (int i = 0; i < recordCount; i++) {
        cout << "ID: " << vacancies[i].id << "\n";
        cout << "���������: " << vacancies[i].title << "\n";
        cout << "��������: " << vacancies[i].company << "\n";
        cout << "��������: " << vacancies[i].salary << "\n";
        cout << "����������: " << vacancies[i].requirements << "\n";
        cout << "���� ����������: " << vacancies[i].publicationDate << "\n";
        cout << "������: " << (vacancies[i].isActive ? "�������" : "�������") << "\n";
        wall();
    }
    cout << "��������������� ������ ����� ��������� � ����: " << sortedFilename << "\n";
    delete[] vacancies;
}

static void parseDate(const char* dateStr, int& day, int& month, int& year) {
    char dayStr[3] = { dateStr[0], dateStr[1], '\0' };
    char monthStr[3] = { dateStr[3], dateStr[4], '\0' };
    char yearStr[5] = { dateStr[6], dateStr[7], dateStr[8], dateStr[9], '\0' };
    day = atoi(dayStr);
    month = atoi(monthStr);
    year = atoi(yearStr);
}

static bool isDateLess(const char* d1, const char* d2) {
    int day1, month1, year1;
    int day2, month2, year2;
    parseDate(d1, day1, month1, year1);
    parseDate(d2, day2, month2, year2);
    if (year1 < year2) return true;
    if (year1 > year2) return false;
    if (month1 < month2) return true;
    if (month1 > month2) return false;
    if (day1 < day2) return true;
    return false;
}

void selectionSortByDate(const char* sourceFilename, const char* sortedFilename) {
    int recordCount = getRecordCount(sourceFilename);
    if (recordCount == 0) {
        cout << "�������� ���� ���� ��� �� ������.\n";
        return;
    }
    Vacancy* vacancies = new Vacancy[recordCount];
    ifstream inFile(sourceFilename, ios::binary);
    if (!inFile) {
        cerr << "�� ������� ������� �������� ����!\n";
        delete[] vacancies;
        return;
    }
    inFile.read(reinterpret_cast<char*>(vacancies), recordCount * sizeof(Vacancy));
    inFile.close();
    for (int i = 0; i < recordCount - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < recordCount; j++) {
            if (isDateLess(vacancies[j].publicationDate, vacancies[minIndex].publicationDate)) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            swapVacancy(vacancies[i], vacancies[minIndex]);
        }
    }
    ofstream outFile(sortedFilename, ios::binary | ios::trunc);
    if (!outFile) {
        cerr << "�� ������� ������� ���� ��� ��������������� ������!\n";
        delete[] vacancies;
        return;
    }
    outFile.write(reinterpret_cast<const char*>(vacancies), recordCount * sizeof(Vacancy));
    outFile.close();
    cout << "��������, ��������������� �� ���� (��.��.����):\n";
    wall();
    for (int i = 0; i < recordCount; i++) {
        cout << "ID: " << vacancies[i].id << "\n";
        cout << "���������: " << vacancies[i].title << "\n";
        cout << "��������: " << vacancies[i].company << "\n";
        cout << "��������: " << vacancies[i].salary << "\n";
        cout << "����������: " << vacancies[i].requirements << "\n";
        cout << "���� ����������: " << vacancies[i].publicationDate << "\n";
        cout << "������: " << (vacancies[i].isActive ? "�������" : "�������") << "\n";
        wall();
    }
    cout << "��������������� ������ ��������� � ����: " << sortedFilename << "\n";
    delete[] vacancies;
}

void insertionSortByCompany(const char* sourceFilename, const char* sortedFilename) {
    int recordCount = getRecordCount(sourceFilename);
    if (recordCount == 0) {
        cout << "�������� ���� ���� ��� �� ������.\n";
        return;
    }
    Vacancy* vacancies = new Vacancy[recordCount];
    ifstream inFile(sourceFilename, ios::binary);
    if (!inFile) {
        cerr << "�� ������� ������� �������� ����!\n";
        delete[] vacancies;
        return;
    }
    inFile.read(reinterpret_cast<char*>(vacancies), recordCount * sizeof(Vacancy));
    inFile.close();
    for (int i = 1; i < recordCount; i++) {
        Vacancy key = vacancies[i];
        int j = i - 1;
        while (j >= 0 && strcmp(vacancies[j].company, key.company) > 0) {
            vacancies[j + 1] = vacancies[j];
            j--;
        }
        vacancies[j + 1] = key;
    }
    ofstream outFile(sortedFilename, ios::binary | ios::trunc);
    if (!outFile) {
        cerr << "�� ������� ������� ���� ��� ��������������� ������!\n";
        delete[] vacancies;
        return;
    }
    outFile.write(reinterpret_cast<const char*>(vacancies), recordCount * sizeof(Vacancy));
    outFile.close();
    cout << "��������, ��������������� �� �������� �������� (���������� ���������):\n";
    wall();
    for (int i = 0; i < recordCount; i++) {
        cout << "ID: " << vacancies[i].id << "\n";
        cout << "���������: " << vacancies[i].title << "\n";
        cout << "��������: " << vacancies[i].company << "\n";
        cout << "��������: " << vacancies[i].salary << "\n";
        cout << "����������: " << vacancies[i].requirements << "\n";
        cout << "���� ����������: " << vacancies[i].publicationDate << "\n";
        cout << "������: " << (vacancies[i].isActive ? "�������" : "�������") << "\n";
        wall();
    }
    cout << "��������������� ������ ��������� � ����: " << sortedFilename << "\n";
    delete[] vacancies;
}

void searchByStatus(const char* filename) {
    int recordCount = getRecordCount(filename);
    if (recordCount == 0) {
        cout << "���� ���� ��� �� ������.\n";
        return;
    }
    Vacancy* vacancies = new Vacancy[recordCount];
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "�� ������� ������� ���� ��� ������!\n";
        delete[] vacancies;
        return;
    }
    inFile.read(reinterpret_cast<char*>(vacancies), recordCount * sizeof(Vacancy));
    inFile.close();
    cout << "������� ������ ��� ������ (1 = �������, 0 = �������): ";
    int statusInput;
    cin >> statusInput;
    bool status = (statusInput == 1);
    wall();
    bool found = false;
    for (int i = 0; i < recordCount; i++) {
        if (vacancies[i].isActive == status) {
            found = true;
            cout << "�������� �� �������� '" << (status ? "�������" : "�������") << "':\n";
            cout << "ID: " << vacancies[i].id << "\n";
            cout << "���������: " << vacancies[i].title << "\n";
            cout << "��������: " << vacancies[i].company << "\n";
            cout << "��������: " << vacancies[i].salary << "\n";
            cout << "����������: " << vacancies[i].requirements << "\n";
            cout << "���� ����������: " << vacancies[i].publicationDate << "\n";
            wall();
        }
    }
    if (!found) {
        cout << "�� ������� �������� �� �������� '" << (status ? "�������" : "�������") << "'.\n";
    }
    delete[] vacancies;
}

void listActiveCompaniesAfterDate(const char* filename) {
    int recordCount = getRecordCount(filename);
    if (recordCount == 0) {
        cout << "���� ���� ��� �� ������.\n";
        return;
    }
    Vacancy* vacancies = new Vacancy[recordCount];
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "�� ������� ������� ���� ��� ������!\n";
        delete[] vacancies;
        return;
    }
    inFile.read(reinterpret_cast<char*>(vacancies), recordCount * sizeof(Vacancy));
    inFile.close();
    cout << "������� ����, ����� ������� ������ (��.��.����): ";
    char userDate[20];
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.getline(userDate, 20);
    wall();
    bool found = false;
    cout << "�������� � ���������� '�������', ���� ������� ����� " << userDate << ":\n";
    wall();
    for (int i = 0; i < recordCount; i++) {
        if (vacancies[i].isActive) {
            int d1, m1, y1;
            int d2, m2, y2;
            parseDate(vacancies[i].publicationDate, d1, m1, y1);
            parseDate(userDate, d2, m2, y2);
            bool isLater = false;
            if (y1 > y2) isLater = true;
            else if (y1 == y2) {
                if (m1 > m2) isLater = true;
                else if (m1 == m2) {
                    if (d1 > d2) isLater = true;
                }
            }
            if (isLater) {
                found = true;
                cout << vacancies[i].company << "\n";
            }
        }
    }
    if (!found) {
        cout << "���������� �������� �� �������.\n";
    }
    wall();
    delete[] vacancies;
}

void readSortedSalaryFile() {
    cout << "������ ����� vacancies_sorted_salary.bin\n";
    wall();
    ifstream file("vacancies_sorted_salary.bin", ios::binary);
    if (!file) {
        cerr << "�� ������� ������� ���� vacancies_sorted_salary.bin\n";
        return;
    }
    Vacancy vac;
    bool foundAny = false;
    while (true) {
        file.read(reinterpret_cast<char*>(&vac), sizeof(Vacancy));
        if (!file.good()) {
            break;
        }
        foundAny = true;
        cout << "ID: " << vac.id << "\n";
        cout << "���������: " << vac.title << "\n";
        cout << "��������: " << vac.company << "\n";
        cout << "��������: " << vac.salary << "\n";
        cout << "����������: " << vac.requirements << "\n";
        cout << "���� ����������: " << vac.publicationDate << "\n";
        cout << "������: " << (vac.isActive ? "�������" : "�������") << "\n";
        wall();
    }
    file.close();
    if (!foundAny) {
        cout << "���� ���� ��� �������� �� �������.\n";
    }
}

void readSortedDateFile() {
    cout << "������ ����� vacancies_sorted_date.bin\n";
    wall();
    ifstream file("vacancies_sorted_date.bin", ios::binary);
    if (!file) {
        cerr << "�� ������� ������� ���� vacancies_sorted_date.bin\n";
        return;
    }
    Vacancy vac;
    bool foundAny = false;
    while (true) {
        file.read(reinterpret_cast<char*>(&vac), sizeof(Vacancy));
        if (!file.good()) {
            break;
        }
        foundAny = true;
        cout << "ID: " << vac.id << "\n";
        cout << "���������: " << vac.title << "\n";
        cout << "��������: " << vac.company << "\n";
        cout << "��������: " << vac.salary << "\n";
        cout << "����������: " << vac.requirements << "\n";
        cout << "���� ����������: " << vac.publicationDate << "\n";
        cout << "������: " << (vac.isActive ? "�������" : "�������") << "\n";
        wall();
    }
    file.close();
    if (!foundAny) {
        cout << "���� ���� ��� �������� �� �������.\n";
    }
}

void readSortedCompanyFile() {
    cout << "������ ����� vacancies_sorted_company.bin\n";
    wall();
    ifstream file("vacancies_sorted_company.bin", ios::binary);
    if (!file) {
        cerr << "�� ������� ������� ���� vacancies_sorted_company.bin\n";
        return;
    }
    Vacancy vac;
    bool foundAny = false;
    while (true) {
        file.read(reinterpret_cast<char*>(&vac), sizeof(Vacancy));
        if (!file.good()) {
            break;
        }
        foundAny = true;
        cout << "ID: " << vac.id << "\n";
        cout << "���������: " << vac.title << "\n";
        cout << "��������: " << vac.company << "\n";
        cout << "��������: " << vac.salary << "\n";
        cout << "����������: " << vac.requirements << "\n";
        cout << "���� ����������: " << vac.publicationDate << "\n";
        cout << "������: " << (vac.isActive ? "�������" : "�������") << "\n";
        wall();
    }
    file.close();
    if (!foundAny) {
        cout << "���� ���� ��� �������� �� �������.\n";
    }
}