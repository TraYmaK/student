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
        cerr << "Не удалось открыть файл для чтения!\n";
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
        cout << "Должность: " << vac.title << "\n";
        cout << "Компания: " << vac.company << "\n";
        cout << "Зарплата: " << vac.salary << "\n";
        cout << "Требования: " << vac.requirements << "\n";
        cout << "Дата публикации: " << vac.publicationDate << "\n";
        cout << "Статус: " << (vac.isActive ? "Активна" : "Закрыта") << "\n";
        wall();
    }
    file.close();
    if (!foundAny) {
        cout << "Файл пуст или вакансии не найдены.\n";
    }
}

void addVacancy(const char* filename) {
    ofstream file(filename, ios::binary | ios::app);
    if (!file) {
        cerr << "Не удалось открыть файл для записи.\n";
        return;
    }
    Vacancy vac;
    cout << "Заполнение новой вакансии:\n";
    wall();
    inputVacancyFromUser(vac);
    file.write(reinterpret_cast<const char*>(&vac), sizeof(Vacancy));
    file.close();
    cout << "Вакансия успешно добавлена.\n";
}

void updateVacancy(const char* filename, int vacancyId) {
    int recordCount = getRecordCount(filename);
    if (recordCount == 0) {
        cout << "Файл пуст или не найден.\n";
        return;
    }
    Vacancy* vacancies = new Vacancy[recordCount];
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "Не удалось открыть файл для чтения.\n";
        delete[] vacancies;
        return;
    }
    inFile.read(reinterpret_cast<char*>(vacancies), recordCount * sizeof(Vacancy));
    inFile.close();
    bool found = false;
    for (int i = 0; i < recordCount; i++) {
        if (vacancies[i].id == vacancyId) {
            cout << "Текущие данные о вакансии:\n";
            wall();
            cout << "ID: " << vacancies[i].id << "\n"
                << "Должность: " << vacancies[i].title << "\n"
                << "Компания: " << vacancies[i].company << "\n"
                << "Зарплата: " << vacancies[i].salary << "\n"
                << "Требования: " << vacancies[i].requirements << "\n"
                << "Дата публикации: " << vacancies[i].publicationDate << "\n"
                << "Статус: " << (vacancies[i].isActive ? "Активна" : "Закрыта") << "\n";
            wall();
            cout << "Введите новые данные:\n";
            wall();
            inputVacancyFromUser(vacancies[i]);
            vacancies[i].id = vacancyId;
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Вакансия с таким ID не найдена!\n";
        delete[] vacancies;
        return;
    }
    ofstream outFile(filename, ios::binary | ios::trunc);
    if (!outFile) {
        cerr << "Не удалось открыть файл для записи!\n";
        delete[] vacancies;
        return;
    }
    outFile.write(reinterpret_cast<const char*>(vacancies), recordCount * sizeof(Vacancy));
    outFile.close();
    delete[] vacancies;
    cout << "Вакансия успешно обновлена.\n";
}

void deleteVacancy(const char* filename, int vacancyId) {
    int recordCount = getRecordCount(filename);
    if (recordCount == 0) {
        cout << "Файл пуст или не найден.\n";
        return;
    }
    Vacancy* vacancies = new Vacancy[recordCount];
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "Не удалось открыть файл для чтения!\n";
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
        cout << "Вакансия с ID " << vacancyId << " не найдена.\n";
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
        cerr << "Не удалось открыть файл для записи!\n";
        delete[] vacancies;
        delete[] newVacancies;
        return;
    }
    outFile.write(reinterpret_cast<const char*>(newVacancies), newCount * sizeof(Vacancy));
    outFile.close();
    delete[] vacancies;
    delete[] newVacancies;
    cout << "Вакансия с ID " << vacancyId << " успешно удалена.\n";
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
    cout << "Введите ID вакансии: ";
    cin >> vac.id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    wall();
    cout << "Введите название должности: ";
    cin.getline(vac.title, 50);
    wall();
    cout << "Введите название компании: ";
    cin.getline(vac.company, 50);
    wall();
    cout << "Введите зарплату: ";
    cin >> vac.salary;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    wall();
    cout << "Введите требования (одной строкой): ";
    cin.getline(vac.requirements, 100);
    wall();
    cout << "Введите дату публикации (дд.мм.гггг): ";
    cin.getline(vac.publicationDate, 20);
    wall();
    cout << "Введите статус (1 = активна, 0 = закрыта): ";
    int status;
    cin >> status;
    vac.isActive = (status == 1);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    wall();
}

void linearSearchBySalary(const char* filename) {
    int recordCount = getRecordCount(filename);
    if (recordCount == 0) {
        cout << "Файл пуст или не найден.\n";
        return;
    }
    Vacancy* vacancies = new Vacancy[recordCount];
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "Не удалось открыть файл для чтения!\n";
        delete[] vacancies;
        return;
    }
    inFile.read(reinterpret_cast<char*>(vacancies), recordCount * sizeof(Vacancy));
    inFile.close();
    double targetSalary;
    cout << "Введите зарплату для линейного поиска: ";
    cin >> targetSalary;
    wall();
    bool found = false;
    for (int i = 0; i < recordCount; i++) {
        if (vacancies[i].salary == targetSalary) {
            found = true;
            cout << "Найдена вакансия с зарплатой " << targetSalary << ":\n";
            cout << "ID: " << vacancies[i].id << "\n";
            cout << "Должность: " << vacancies[i].title << "\n";
            cout << "Компания: " << vacancies[i].company << "\n";
            cout << "Зарплата: " << vacancies[i].salary << "\n";
            cout << "Требования: " << vacancies[i].requirements << "\n";
            cout << "Дата публикации: " << vacancies[i].publicationDate << "\n";
            cout << "Статус: " << (vacancies[i].isActive ? "Активна" : "Закрыта") << "\n";
            wall();
        }
    }
    if (!found) {
        cout << "Вакансий с зарплатой " << targetSalary << " не найдено.\n";
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
        cout << "Файл пуст или не найден.\n";
        return;
    }
    Vacancy* vacancies = new Vacancy[recordCount];
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "Не удалось открыть файл для чтения!\n";
        delete[] vacancies;
        return;
    }
    inFile.read(reinterpret_cast<char*>(vacancies), recordCount * sizeof(Vacancy));
    inFile.close();
    sortArrayByTitle(vacancies, recordCount);
    char targetTitle[50];
    cout << "Введите должность для бинарного поиска: ";
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
                cout << "Найдена вакансия:\n";
                cout << "ID: " << vacancies[temp].id << "\n";
                cout << "Должность: " << vacancies[temp].title << "\n";
                cout << "Компания: " << vacancies[temp].company << "\n";
                cout << "Зарплата: " << vacancies[temp].salary << "\n";
                cout << "Требования: " << vacancies[temp].requirements << "\n";
                cout << "Дата публикации: " << vacancies[temp].publicationDate << "\n";
                cout << "Статус: " << (vacancies[temp].isActive ? "Активна" : "Закрыта") << "\n";
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
        cout << "Вакансий с должностью '" << targetTitle << "' не найдено.\n";
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
        cout << "Исходный файл пуст или не найден.\n";
        return;
    }
    Vacancy* vacancies = new Vacancy[recordCount];
    ifstream inFile(sourceFilename, ios::binary);
    if (!inFile) {
        cerr << "Не удалось открыть исходный файл!\n";
        delete[] vacancies;
        return;
    }
    inFile.read(reinterpret_cast<char*>(vacancies), recordCount * sizeof(Vacancy));
    inFile.close();
    quickSortRec(vacancies, 0, recordCount - 1);
    ofstream outFile(sortedFilename, ios::binary | ios::trunc);
    if (!outFile) {
        cerr << "Не удалось создать файл для отсортированных данных!\n";
        delete[] vacancies;
        return;
    }
    outFile.write(reinterpret_cast<const char*>(vacancies), recordCount * sizeof(Vacancy));
    outFile.close();
    cout << "Вакансии, отсортированные по зарплате (quick sort):\n";
    wall();
    for (int i = 0; i < recordCount; i++) {
        cout << "ID: " << vacancies[i].id << "\n";
        cout << "Должность: " << vacancies[i].title << "\n";
        cout << "Компания: " << vacancies[i].company << "\n";
        cout << "Зарплата: " << vacancies[i].salary << "\n";
        cout << "Требования: " << vacancies[i].requirements << "\n";
        cout << "Дата публикации: " << vacancies[i].publicationDate << "\n";
        cout << "Статус: " << (vacancies[i].isActive ? "Активна" : "Закрыта") << "\n";
        wall();
    }
    cout << "Отсортированные данные также сохранены в файл: " << sortedFilename << "\n";
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
        cout << "Исходный файл пуст или не найден.\n";
        return;
    }
    Vacancy* vacancies = new Vacancy[recordCount];
    ifstream inFile(sourceFilename, ios::binary);
    if (!inFile) {
        cerr << "Не удалось открыть исходный файл!\n";
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
        cerr << "Не удалось создать файл для отсортированных данных!\n";
        delete[] vacancies;
        return;
    }
    outFile.write(reinterpret_cast<const char*>(vacancies), recordCount * sizeof(Vacancy));
    outFile.close();
    cout << "Вакансии, отсортированные по дате (дд.мм.гггг):\n";
    wall();
    for (int i = 0; i < recordCount; i++) {
        cout << "ID: " << vacancies[i].id << "\n";
        cout << "Должность: " << vacancies[i].title << "\n";
        cout << "Компания: " << vacancies[i].company << "\n";
        cout << "Зарплата: " << vacancies[i].salary << "\n";
        cout << "Требования: " << vacancies[i].requirements << "\n";
        cout << "Дата публикации: " << vacancies[i].publicationDate << "\n";
        cout << "Статус: " << (vacancies[i].isActive ? "Активна" : "Закрыта") << "\n";
        wall();
    }
    cout << "Отсортированные данные сохранены в файл: " << sortedFilename << "\n";
    delete[] vacancies;
}

void insertionSortByCompany(const char* sourceFilename, const char* sortedFilename) {
    int recordCount = getRecordCount(sourceFilename);
    if (recordCount == 0) {
        cout << "Исходный файл пуст или не найден.\n";
        return;
    }
    Vacancy* vacancies = new Vacancy[recordCount];
    ifstream inFile(sourceFilename, ios::binary);
    if (!inFile) {
        cerr << "Не удалось открыть исходный файл!\n";
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
        cerr << "Не удалось создать файл для отсортированных данных!\n";
        delete[] vacancies;
        return;
    }
    outFile.write(reinterpret_cast<const char*>(vacancies), recordCount * sizeof(Vacancy));
    outFile.close();
    cout << "Вакансии, отсортированные по названию компании (сортировка вставками):\n";
    wall();
    for (int i = 0; i < recordCount; i++) {
        cout << "ID: " << vacancies[i].id << "\n";
        cout << "Должность: " << vacancies[i].title << "\n";
        cout << "Компания: " << vacancies[i].company << "\n";
        cout << "Зарплата: " << vacancies[i].salary << "\n";
        cout << "Требования: " << vacancies[i].requirements << "\n";
        cout << "Дата публикации: " << vacancies[i].publicationDate << "\n";
        cout << "Статус: " << (vacancies[i].isActive ? "Активна" : "Закрыта") << "\n";
        wall();
    }
    cout << "Отсортированные данные сохранены в файл: " << sortedFilename << "\n";
    delete[] vacancies;
}

void searchByStatus(const char* filename) {
    int recordCount = getRecordCount(filename);
    if (recordCount == 0) {
        cout << "Файл пуст или не найден.\n";
        return;
    }
    Vacancy* vacancies = new Vacancy[recordCount];
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "Не удалось открыть файл для чтения!\n";
        delete[] vacancies;
        return;
    }
    inFile.read(reinterpret_cast<char*>(vacancies), recordCount * sizeof(Vacancy));
    inFile.close();
    cout << "Введите статус для поиска (1 = активна, 0 = закрыта): ";
    int statusInput;
    cin >> statusInput;
    bool status = (statusInput == 1);
    wall();
    bool found = false;
    for (int i = 0; i < recordCount; i++) {
        if (vacancies[i].isActive == status) {
            found = true;
            cout << "Вакансия со статусом '" << (status ? "активна" : "закрыта") << "':\n";
            cout << "ID: " << vacancies[i].id << "\n";
            cout << "Должность: " << vacancies[i].title << "\n";
            cout << "Компания: " << vacancies[i].company << "\n";
            cout << "Зарплата: " << vacancies[i].salary << "\n";
            cout << "Требования: " << vacancies[i].requirements << "\n";
            cout << "Дата публикации: " << vacancies[i].publicationDate << "\n";
            wall();
        }
    }
    if (!found) {
        cout << "Не найдено вакансий со статусом '" << (status ? "активна" : "закрыта") << "'.\n";
    }
    delete[] vacancies;
}

void listActiveCompaniesAfterDate(const char* filename) {
    int recordCount = getRecordCount(filename);
    if (recordCount == 0) {
        cout << "Файл пуст или не найден.\n";
        return;
    }
    Vacancy* vacancies = new Vacancy[recordCount];
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "Не удалось открыть файл для чтения!\n";
        delete[] vacancies;
        return;
    }
    inFile.read(reinterpret_cast<char*>(vacancies), recordCount * sizeof(Vacancy));
    inFile.close();
    cout << "Введите дату, после которой искать (дд.мм.гггг): ";
    char userDate[20];
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.getline(userDate, 20);
    wall();
    bool found = false;
    cout << "Компании с вакансиями 'активна', дата которых позже " << userDate << ":\n";
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
        cout << "Подходящих компаний не найдено.\n";
    }
    wall();
    delete[] vacancies;
}

void readSortedSalaryFile() {
    cout << "Чтение файла vacancies_sorted_salary.bin\n";
    wall();
    ifstream file("vacancies_sorted_salary.bin", ios::binary);
    if (!file) {
        cerr << "Не удалось открыть файл vacancies_sorted_salary.bin\n";
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
        cout << "Должность: " << vac.title << "\n";
        cout << "Компания: " << vac.company << "\n";
        cout << "Зарплата: " << vac.salary << "\n";
        cout << "Требования: " << vac.requirements << "\n";
        cout << "Дата публикации: " << vac.publicationDate << "\n";
        cout << "Статус: " << (vac.isActive ? "Активна" : "Закрыта") << "\n";
        wall();
    }
    file.close();
    if (!foundAny) {
        cout << "Файл пуст или вакансии не найдены.\n";
    }
}

void readSortedDateFile() {
    cout << "Чтение файла vacancies_sorted_date.bin\n";
    wall();
    ifstream file("vacancies_sorted_date.bin", ios::binary);
    if (!file) {
        cerr << "Не удалось открыть файл vacancies_sorted_date.bin\n";
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
        cout << "Должность: " << vac.title << "\n";
        cout << "Компания: " << vac.company << "\n";
        cout << "Зарплата: " << vac.salary << "\n";
        cout << "Требования: " << vac.requirements << "\n";
        cout << "Дата публикации: " << vac.publicationDate << "\n";
        cout << "Статус: " << (vac.isActive ? "Активна" : "Закрыта") << "\n";
        wall();
    }
    file.close();
    if (!foundAny) {
        cout << "Файл пуст или вакансии не найдены.\n";
    }
}

void readSortedCompanyFile() {
    cout << "Чтение файла vacancies_sorted_company.bin\n";
    wall();
    ifstream file("vacancies_sorted_company.bin", ios::binary);
    if (!file) {
        cerr << "Не удалось открыть файл vacancies_sorted_company.bin\n";
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
        cout << "Должность: " << vac.title << "\n";
        cout << "Компания: " << vac.company << "\n";
        cout << "Зарплата: " << vac.salary << "\n";
        cout << "Требования: " << vac.requirements << "\n";
        cout << "Дата публикации: " << vac.publicationDate << "\n";
        cout << "Статус: " << (vac.isActive ? "Активна" : "Закрыта") << "\n";
        wall();
    }
    file.close();
    if (!foundAny) {
        cout << "Файл пуст или вакансии не найдены.\n";
    }
}