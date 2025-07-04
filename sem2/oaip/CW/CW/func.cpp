#include <iostream>
#include <fstream>
#include <cstring>
#include "func.hpp"

using namespace std;

void wall() {
    cout << "--------------------------------------------------------------------------------------------------------------" << endl;
}

void clear() {
    system("pause");
    system("cls");
}

void logAction(const char* functionName, const char* input, const char* result) {
    static int counter = 1;
    FILE* log;
    if (fopen_s(&log, "logs.txt", "a") != 0 || !log) return;

    fprintf(log, "%d. Выполнена функция \"%s\".", counter++, functionName);
    if (input && input[0] != '\0') fprintf(log, " Исходные данные: %s.", input);
    if (result && result[0] != '\0') fprintf(log, " Результат выполнения функции: %s.", result);
    fprintf(log, "\n");

    fclose(log);
}

void viewLogs() {
    FILE* log;
    if (fopen_s(&log, "logs.txt", "r") != 0 || !log) {
        wall();
        cout << "Файл логов не найден или не удалось открыть.\n";
        wall();
        clear();
        return;
    }

    char line[256];
    wall();
    cout << "\n";

    while (fgets(line, sizeof(line), log)) {
        cout << line;
    }

    fclose(log);
    cout << "\n";
    wall();
    clear();
}

void createFile(const char* filename) {
    ofstream file(filename, ios::binary | ios::trunc);
    file.close();
    logAction("createFile", filename, "создан или очищен бинарный файл");
}

void printVacancy(const Vacancy& vac) {
    cout << "ID: " << vac.id << "\n"
        << "Должность: " << vac.title << "\n"
        << "Компания: " << vac.company << "\n"
        << "Зарплата: " << vac.salary << "\n"
        << "Требования: " << vac.requirements << "\n"
        << "Дата публикации: " << vac.publicationDate << "\n"
        << "Статус: " << (vac.isActive ? "Активна" : "Закрыта") << "\n";
    wall();
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
        printVacancy(vac);
    }
    file.close();
    if (!foundAny) {
        cout << "Файл пуст или вакансии не найдены.\n";
        wall();
    }
    logAction("readFile", filename, foundAny ? "данные выведены" : "файл пуст или не найдено записей");
}

void addVacancy(const char* filename) {
    Vacancy vac;
    cout << "Заполнение новой вакансии:\n";
    wall();
    inputVacancyFromUser(vac);

    int recordCount = getRecordCount(filename);
    if (recordCount > 0) {
        Vacancy* vacancies = new Vacancy[recordCount];
        ifstream inFile(filename, ios::binary);
        if (!inFile) {
            cerr << "Не удалось открыть файл для чтения.\n";
            delete[] vacancies;
            return;
        }
        inFile.read(reinterpret_cast<char*>(vacancies), recordCount * sizeof(Vacancy));
        inFile.close();

        for (int i = 0; i < recordCount; ++i) {
            if (vacancies[i].id == vac.id) {
                cout << "Ошибка: вакансия с ID " << vac.id << " уже существует. Добавление отменено.\n";
                delete[] vacancies;
                return;
            }
        }
        delete[] vacancies;
    }

    ofstream file(filename, ios::binary | ios::app);
    if (!file) {
        cerr << "Не удалось открыть файл для записи.\n";
        return;
    }

    file.write(reinterpret_cast<const char*>(&vac), sizeof(Vacancy));
    file.close();

    char buf[64];
    sprintf_s(buf, sizeof(buf), "ID = %d", vac.id);
    logAction("addVacancy", buf, "вакансия добавлена");
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
    wall();
    for (int i = 0; i < recordCount; i++) {
        if (vacancies[i].id == vacancyId) {
            cout << "Текущие данные о вакансии:\n";
            wall();
            printVacancy(vacancies[i]);
            cout << "Введите новые данные:\n";
            wall();
            inputVacancyFromUser(vacancies[i]);
            for (int j = 0; j < recordCount; ++j) {
                if (j != i && vacancies[j].id == vacancies[i].id) {
                    cout << "Ошибка: Такой ID уже используется. Обновление отменено.\n";
                    wall();
                    delete[] vacancies;
                    return;
                }
            }

            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Вакансия с таким ID не найдена!\n";
        wall();
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
    char buf[64];
    sprintf_s(buf, sizeof(buf), "ID = %d", vacancyId);
    logAction("updateVacancy", buf, found ? "обновлена" : "не найдена");
    cout << "Вакансия успешно обновлена.\n";
    wall();
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
        wall();
        cout << "Вакансия с ID " << vacancyId << " не найдена.\n";
        wall();
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
    char buf[64];
    sprintf_s(buf, sizeof(buf), "ID = %d", vacancyId);
    logAction("deleteVacancy", buf, newCount < recordCount ? "удалена" : "не найдена");
    delete[] vacancies;
    delete[] newVacancies;
    wall();
    cout << "Вакансия с ID " << vacancyId << " успешно удалена.\n";
    wall();
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
    while (true) {
        cout << "Введите ID вакансии: ";
        if (cin >> vac.id) break;
        wall();
        cout << "Некорректный ввод. Введите целое число.\n";
        wall();
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    wall();

    cout << "Введите название должности: ";
    cin.getline(vac.title, 50);
    wall();

    cout << "Введите название компании: ";
    cin.getline(vac.company, 50);
    wall();

    while (true) {
        cout << "Введите зарплату: ";
        if (cin >> vac.salary) break;
        wall();
        cout << "Некорректный ввод. Введите число.\n";
        wall();
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    wall();

    cout << "Введите требования (одной строкой): ";
    cin.getline(vac.requirements, 100);
    wall();

    cout << "Введите дату публикации (дд.мм.гггг): ";
    cin.getline(vac.publicationDate, 20);
    wall();

    while (true) {
        cout << "Введите статус (1 = активна, 0 = закрыта): ";
        int status;
        if (cin >> status && (status == 0 || status == 1)) {
            vac.isActive = (status == 1);
            break;
        }
        wall();
        cout << "Некорректный ввод. Введите 1 или 0.\n";
        wall();
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
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
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        wall();
        cout << "Некорректный ввод. Зарплата должна быть числом.\n";
        wall();
        return;
    }
    wall();
    bool found = false;
    for (int i = 0; i < recordCount; i++) {
        if (vacancies[i].salary == targetSalary) {
            found = true;
            printVacancy(vacancies[i]);
        }
    }
    if (!found) {
        cout << "Вакансий с зарплатой " << targetSalary << " не найдено.\n";
        wall();
    }
    delete[] vacancies;
    char buf[64];
    sprintf_s(buf, sizeof(buf), "зарплата = %.2f", targetSalary);
    logAction("linearSearchBySalary", buf, found ? "найдено" : "не найдено");
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
                printVacancy(vacancies[temp]);
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
    char buf[128];
    sprintf_s(buf, sizeof(buf), "должность = %s", targetTitle);
    logAction("binarySearchByTitle", buf, found ? "найдено" : "не найдено");
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

    ofstream out("vacancies_sorted_salary.txt");
    if (!out) {
        cerr << "Не удалось создать файл vacancies_sorted_salary.txt\n";
        delete[] vacancies;
        return;
    }

    for (int i = 0; i < recordCount; ++i) {
        out << "ID: " << vacancies[i].id << "\n"
            << "Должность: " << vacancies[i].title << "\n"
            << "Компания: " << vacancies[i].company << "\n"
            << "Зарплата: " << vacancies[i].salary << "\n"
            << "Требования: " << vacancies[i].requirements << "\n"
            << "Дата публикации: " << vacancies[i].publicationDate << "\n"
            << "Статус: " << (vacancies[i].isActive ? "Активна" : "Закрыта") << "\n"
            << "--------------------------------------------------------------------------------------" << endl;
    }

    delete[] vacancies;
    out.close();
    logAction("quickSortBySalary", "", "сохранено в vacancies_sorted_salary.txt");
    cout << "Отчет сохранён в vacancies_sorted_salary.txt\n";
    wall();
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

    for (int i = 0; i < recordCount - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < recordCount; ++j) {
            if (isDateLess(vacancies[j].publicationDate, vacancies[minIdx].publicationDate)) {
                minIdx = j;
            }
        }
        if (minIdx != i) swapVacancy(vacancies[i], vacancies[minIdx]);
    }

    ofstream out("vacancies_sorted_date.txt");
    if (!out) {
        cerr << "Не удалось создать файл vacancies_sorted_date.txt\n";
        delete[] vacancies;
        return;
    }

    for (int i = 0; i < recordCount; ++i) {
        out << "ID: " << vacancies[i].id << "\n"
            << "Должность: " << vacancies[i].title << "\n"
            << "Компания: " << vacancies[i].company << "\n"
            << "Зарплата: " << vacancies[i].salary << "\n"
            << "Требования: " << vacancies[i].requirements << "\n"
            << "Дата публикации: " << vacancies[i].publicationDate << "\n"
            << "Статус: " << (vacancies[i].isActive ? "Активна" : "Закрыта") << "\n"
            << "--------------------------------------------------------------------------------------" << endl;
    }

    delete[] vacancies;
    out.close();
    logAction("selectionSortByDate", "", "сохранено в vacancies_sorted_date.txt");
    cout << "Отчет сохранён в vacancies_sorted_date.txt\n";
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

    for (int i = 1; i < recordCount; ++i) {
        Vacancy key = vacancies[i];
        int j = i - 1;
        while (j >= 0 && strcmp(vacancies[j].company, key.company) > 0) {
            vacancies[j + 1] = vacancies[j];
            j--;
        }
        vacancies[j + 1] = key;
    }

    ofstream out("vacancies_sorted_company.txt");
    if (!out) {
        cerr << "Не удалось создать файл vacancies_sorted_company.txt\n";
        delete[] vacancies;
        return;
    }

    for (int i = 0; i < recordCount; ++i) {
        out << "ID: " << vacancies[i].id << "\n"
            << "Должность: " << vacancies[i].title << "\n"
            << "Компания: " << vacancies[i].company << "\n"
            << "Зарплата: " << vacancies[i].salary << "\n"
            << "Требования: " << vacancies[i].requirements << "\n"
            << "Дата публикации: " << vacancies[i].publicationDate << "\n"
            << "Статус: " << (vacancies[i].isActive ? "Активна" : "Закрыта") << "\n"
            << "--------------------------------------------------------------------------------------" << endl;
    }

    delete[] vacancies;
    out.close();
    logAction("insertionSortByCompany", "", "сохранено в vacancies_sorted_company.txt");
    cout << "Отчет сохранён в vacancies_sorted_company.txt\n";
}

void searchByStatus(const char* filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Ошибка открытия файла.\n";
        return;
    }

    double minSalary, maxSalary;
    char keyword[50];

    cout << "Введите минимальную зарплату: ";
    cin >> minSalary;
    wall();
    cout << "Введите максимальную зарплату: ";
    cin >> maxSalary;
    wall();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Введите ключевое слово из требований: ";
    cin.getline(keyword, 50);

    wall();

    Vacancy results[100];
    int count = 0;
    Vacancy vac;

    while (file.read(reinterpret_cast<char*>(&vac), sizeof(Vacancy))) {
        if (vac.salary >= minSalary && vac.salary <= maxSalary &&
            strstr(vac.requirements, keyword)) {
            results[count++] = vac;
        }
    }
    file.close();

    if (count == 0) {
        cout << "Вакансии по заданным критериям не найдены.\n";
        wall();
        return;
    }

    for (int i = 0; i < count - 1; ++i) {
        for (int j = i + 1; j < count; ++j) {
            if (isDateLess(results[j].publicationDate, results[i].publicationDate)) {
                swapVacancy(results[i], results[j]);
            }
        }
    }

    cout << "Результаты поиска:\n";
    wall();
    for (int i = 0; i < count; ++i) {
        printVacancy(results[i]);
    }
    logAction("searchByStatus", "фильтр по диапазону и ключу", count > 0 ? "найдено" : "не найдено");
}

bool checkDateFormat(const char* date) {
    if (strlen(date) != 10)
        return false;
    if (date[2] != '.' || date[5] != '.')
        return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(date[i])) return false;
    }
    return true;
}

void listActiveVacanciesAfterDate(const char* filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "Ошибка: не удалось открыть файл.\n";
        return;
    }

    char inputDate[20];
    cout << "Введите дату, после которой искать (дд.мм.гггг): ";
    cin >> inputDate;
    wall();

    if (!checkDateFormat(inputDate)) {
        cout << "Ошибка: формат даты должен быть дд.мм.гггг\n";
        return;
    }

    file.seekg(0, ios::end);
    int size = file.tellg() / sizeof(Vacancy);
    file.seekg(0, ios::beg);

    if (size == 0) {
        cout << "Файл пуст.\n";
        return;
    }

    Vacancy* arr = new Vacancy[size];
    file.read(reinterpret_cast<char*>(arr), size * sizeof(Vacancy));
    file.close();

    bool found = false;
    for (int i = 0; i < size; ++i) {
        if (arr[i].isActive && isDateLess(inputDate, arr[i].publicationDate)) {
            printVacancy(arr[i]);
            found = true;
        }
    }

    if (!found) {
        cout << "Нет подходящих вакансий.\n";
        wall();
    }

    delete[] arr;
    char buf[64];
    sprintf_s(buf, sizeof(buf), "дата = %s", inputDate);
    logAction("listActiveVacanciesAfterDate", buf, found ? "найдено" : "не найдено");
}

void printSalaryReport() {
    ifstream file("vacancies_sorted_salary.txt");
    if (!file) {
        cerr << "Не удалось открыть файл vacancies_sorted_salary.txt\n";
        return;
    }

    char buffer[256];
    while (!file.eof()) {
        int i = 0;
        char ch;
        while (i < 255 && file.get(ch)) {
            if (ch == '\n') break;
            buffer[i++] = ch;
        }
        buffer[i] = '\0';
        cout << buffer << "\n";
    }

    file.close();
    logAction("printSalaryReport");
}

void printDateReport() {
    ifstream file("vacancies_sorted_date.txt");
    if (!file) {
        cerr << "Не удалось открыть файл vacancies_sorted_date.txt\n";
        return;
    }

    char buffer[256];
    while (!file.eof()) {
        int i = 0;
        char ch;
        while (i < 255 && file.get(ch)) {
            if (ch == '\n') break;
            buffer[i++] = ch;
        }
        buffer[i] = '\0';
        cout << buffer << "\n";
    }

    file.close();
    logAction("printDateReport");
}

void printCompanyReport() {
    ifstream file("vacancies_sorted_company.txt");
    if (!file) {
        cerr << "Не удалось открыть файл vacancies_sorted_company.txt\n";
        return;
    }

    char buffer[256];
    while (!file.eof()) {
        int i = 0;
        char ch;
        while (i < 255 && file.get(ch)) {
            if (ch == '\n') break;
            buffer[i++] = ch;
        }
        buffer[i] = '\0';
        cout << buffer << "\n";
    }

    file.close();
    logAction("printCompanyReport");
}