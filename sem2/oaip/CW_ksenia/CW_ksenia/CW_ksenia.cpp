#include <iostream>
#include <iomanip>
#include <Windows.h>
using namespace std;

struct Date {
    int day;
    int month;
    int year;
};

struct Time {
    int hours;
    int minutes;
};

struct Patient {
    char fio[100];
    Date date;
    Time time;
    char type_of_procedure[100];
    char doctor[100];
    int cost;
};

void wall() {
    cout << "-------------------------------------------------------------------------" << endl;
}

void clear() {
    system("pause"); // остановка кода
    system("cls"); // очистка экрана
}

void createFile(const char* filename) {
    FILE* file;
    fopen_s(&file, filename, "wb");
    if (!file) {
        cout << "\nОшибка открытия файла!" << endl;
        return;
    }

    int n;
    cout << "Введите количество записей: ";
    cin >> n;
    cin.ignore();

    for (int i = 0; i < n; i++) {
        Patient patients;

        cout << "ФИО: ";
        cin.getline(patients.fio, sizeof(patients.fio));
        cout << "Дата (дд мм гггг): ";
        cin >> patients.date.day >> patients.date.month >> patients.date.year;
        cin.ignore();

        cout << "Время (чч мм): ";
        cin >> patients.time.hours >> patients.time.minutes;
        cin.ignore();

        cout << "Тип процедуры: ";
        cin.getline(patients.type_of_procedure, sizeof(patients.type_of_procedure));

        cout << "Лечащий врач: ";
        cin.getline(patients.doctor, sizeof(patients.doctor));

        cout << "Стоимость приёма: ";
        cin >> patients.cost;

        fwrite(&patients, sizeof(Patient), 1, file);
        cin.ignore();
    }

    fclose(file);
}

void viewFile(const char* filename) {
    FILE* file;
    fopen_s(&file, filename, "rb");
    if (!file) {
        cout << "\nОшибка открытия файла!" << endl;
        return;
    }

    Patient patients;
    wall();
    cout << "Список пациентов:\n";
    wall();
    while (fread(&patients, sizeof(Patient), 1, file)) {
        cout << patients.fio << " "
            << patients.date.day << "."
            << patients.date.month << "."
            << patients.date.year << " "
            << patients.time.hours << ":"
            << patients.time.minutes << " "
            << patients.type_of_procedure << " "
            << patients.doctor << " "
            << patients.cost << " " << endl;
        wall();
    }

    fclose(file);
    clear();
}

void addRecords(const char* filename) {
    FILE* file;
    fopen_s(&file, filename, "ab");
    if (!file) {
        cout << "\nОшибка открытия файла!" << endl;
        return;
    }

    int n;
    cout << "Введите количество добавляемых пациентов: ";
    cin >> n;
    cin.ignore();

    for (int i = 0; i < n; i++) {
        Patient patients;

        cout << "ФИО: ";
        cin.getline(patients.fio, sizeof(patients.fio));

        cout << "Дата (дд мм гггг): ";
        cin >> patients.date.day >> patients.date.month >> patients.date.year;
        cin.ignore();

        cout << "Время (чч мм): ";
        cin >> patients.time.hours >> patients.time.minutes;
        cin.ignore();

        cout << "Тип процедуры: ";
        cin.getline(patients.type_of_procedure, sizeof(patients.type_of_procedure));

        cout << "Лечащий врач: ";
        cin.getline(patients.doctor, sizeof(patients.doctor));

        cout << "Стоимость приёма: ";
        cin >> patients.cost;
        cin.ignore();

        fwrite(&patients, sizeof(Patient), 1, file);
    }

    fclose(file);
}

void editRecord(const char* filename) {
    FILE* file;
    fopen_s(&file, filename, "r+b");
    if (!file) {
        cout << "\nОшибка открытия файла!" << endl;
        return;
    }

    Patient patients;
    char fio[100];
    cout << "Введите ФИО пациента для редактирования: ";
    cin.ignore();
    cin.getline(fio, sizeof(fio));

    bool found = false;
    int recordNumber = 0;


    while (fread(&patients, sizeof(Patient), 1, file)) {

        if (strcmp(patients.fio, fio) == 0) {
            found = true;
            long pos = recordNumber * sizeof(Patient);

            cout << "Запись найдена. Введите новые данные:\n";

            cout << "ФИО: ";
            cin.getline(patients.fio, sizeof(patients.fio));

            cout << "Дата (дд мм гггг): ";
            cin >> patients.date.day >> patients.date.month >> patients.date.year;
            cin.ignore();

            cout << "Время (чч мм): ";
            cin >> patients.time.hours >> patients.time.minutes;
            cin.ignore();

            cout << "Тип процедуры: ";
            cin.getline(patients.type_of_procedure, sizeof(patients.type_of_procedure));

            cout << "Лечащий врач: ";
            cin.getline(patients.doctor, sizeof(patients.doctor));

            cout << "Стоимость приёма: ";
            cin >> patients.cost;
            cin.ignore();

            fseek(file, pos, SEEK_SET);
            fwrite(&patients, sizeof(Patient), 1, file);
            cout << "Запись успешно обновлена!" << endl;
            break;
        }
        recordNumber++;
    }

    if (!found) {
        cout << "Пациент с таким ФИО не найден." << endl;
    }

    fclose(file);
}

void deleteRecord(const char* filename) {
    FILE* file;
    fopen_s(&file, filename, "rb");
    if (!file) {
        cout << "\nОшибка открытия файла!" << endl;
        return;
    }

    Patient patients;
    char fio[100];
    cout << "Введите ФИО пациента для удаления: ";
    cin.ignore();
    cin.getline(fio, sizeof(fio));

    FILE* tempFile;
    fopen_s(&tempFile, "temp.bin", "wb");
    if (!tempFile) {
        cout << "\nОшибка создания временного файла!" << endl;
        fclose(file);
        return;
    }

    bool found = false;

    while (fread(&patients, sizeof(Patient), 1, file)) {
        if (strcmp(patients.fio, fio) != 0) {
            fwrite(&patients, sizeof(Patient), 1, tempFile);
        }
        else {
            found = true;
        }
    }

    if (found) {
        cout << "Запись успешно удалена!" << endl;
    }
    else {
        cout << "Пациент с таким ФИО не найден." << endl;
    }

    fclose(file);
    fclose(tempFile);


    remove(filename);
    rename("temp.bin", filename);
}

void linearSearch(const char* filename, const char* fio) {
    FILE* file;
    if (fopen_s(&file, filename, "rb") != 0) {
        cout << "Ошибка открытия файла!\n";
        return;
    }

    Patient patients;
    bool found = false;

    cout << "Результаты поиска:\n";
    while (fread(&patients, sizeof(Patient), 1, file) == 1) {
        if (strcmp(patients.fio, fio) == 0) {
            if (!found) {
                cout << "Пациент найден:\n";
                found = true;
            }
            cout << setw(10) << patients.fio
                << setw(7) << patients.date.day << "." << patients.date.month << "." << patients.date.year
                << setw(7) << patients.time.hours << ":" << patients.time.minutes
                << setw(10) << patients.type_of_procedure
                << setw(10) << patients.doctor
                << setw(5) << patients.cost << endl;
        }
    }

    if (!found) {
        cout << "Пациент с таким ФИО не найден.\n";
    }

    fclose(file);
}

void selectionSortByDate(const char* filename) {
    FILE* file;
    if (fopen_s(&file, filename, "r+b") != 0) {
        cout << "Ошибка открытия файла!\n";
        return;
    }

    long fileSize = 0;
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    int count = fileSize / sizeof(Patient);
    for (int i = 0; i < count - 1; i++) {
        int minIndex = i;
        Patient patientI, patientJ;
        for (int j = i + 1; j < count; j++) {
            fseek(file, i * sizeof(Patient), SEEK_SET);
            fread(&patientI, sizeof(Patient), 1, file);
            fseek(file, j * sizeof(Patient), SEEK_SET);
            fread(&patientJ, sizeof(Patient), 1, file);

            if (patientJ.date.year < patientI.date.year ||
                (patientJ.date.year == patientI.date.year && patientJ.date.month < patientI.date.month) ||
                (patientJ.date.year == patientI.date.year && patientJ.date.month == patientI.date.month && patientJ.date.day < patientI.date.day)) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            Patient temp = patientI;
            fseek(file, i * sizeof(Patient), SEEK_SET);
            fwrite(&patientJ, sizeof(Patient), 1, file);
            fseek(file, minIndex * sizeof(Patient), SEEK_SET);
            fwrite(&temp, sizeof(Patient), 1, file);
        }
    }
    fclose(file);
    cout << "Файл отсортирован по дате\n";
}

void binarySearchByDate(const char* filename, Date searchDate) {
    FILE* file;
    selectionSortByDate(filename);

    if (fopen_s(&file, filename, "rb") != 0) {
        cout << "Ошибка открытия файла!\n";
        return;
    }

    long fileSize = 0;
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    int left = 0;
    int right = fileSize / sizeof(Patient) - 1;
    bool found = false;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        Patient patient;
        fseek(file, mid * sizeof(Patient), SEEK_SET);
        fread(&patient, sizeof(Patient), 1, file);

        if (patient.date.year == searchDate.year &&
            patient.date.month == searchDate.month &&
            patient.date.day == searchDate.day) {
            found = true;
            cout << "Пациент найден:\n";
            cout << setw(10) << patient.fio
                << setw(7) << patient.date.day << "." << patient.date.month << "." << patient.date.year
                << setw(7) << patient.time.hours << ":" << patient.time.minutes
                << setw(10) << patient.type_of_procedure
                << setw(10) << patient.doctor
                << setw(5) << patient.cost << endl;

            int leftCheck = mid - 1;
            while (leftCheck >= left) {
                fseek(file, leftCheck * sizeof(Patient), SEEK_SET);
                fread(&patient, sizeof(Patient), 1, file);
                if (patient.date.year == searchDate.year &&
                    patient.date.month == searchDate.month &&
                    patient.date.day == searchDate.day) {
                    cout << "Пациент найден:\n";
                    cout << setw(10) << patient.fio
                        << setw(7) << patient.date.day << "." << patient.date.month << "." << patient.date.year
                        << setw(7) << patient.time.hours << ":" << patient.time.minutes
                        << setw(10) << patient.type_of_procedure
                        << setw(10) << patient.doctor
                        << setw(5) << patient.cost << endl;
                }
                else {
                    break;
                }
                leftCheck--;
            }

            int rightCheck = mid + 1;
            while (rightCheck <= right) {
                fseek(file, rightCheck * sizeof(Patient), SEEK_SET);
                fread(&patient, sizeof(Patient), 1, file);
                if (patient.date.year == searchDate.year &&
                    patient.date.month == searchDate.month &&
                    patient.date.day == searchDate.day) {
                    cout << "Пациент найден:\n";
                    cout << setw(10) << patient.fio
                        << setw(7) << patient.date.day << "." << patient.date.month << "." << patient.date.year
                        << setw(7) << patient.time.hours << ":" << patient.time.minutes
                        << setw(10) << patient.type_of_procedure
                        << setw(10) << patient.doctor
                        << setw(5) << patient.cost << endl;
                }
                else {
                    break;
                }
                rightCheck++;
            }
            break;
        }
        else if (patient.date.year < searchDate.year ||
            (patient.date.year == searchDate.year && patient.date.month < searchDate.month) ||
            (patient.date.year == searchDate.year && patient.date.month == searchDate.month && patient.date.day < searchDate.day)) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    if (!found) {
        cout << "Пациент с такой датой приёма не найден.\n";
    }

    fclose(file);
}

void quickSortByDate(const char* filename) {
    FILE* file;
    if (fopen_s(&file, filename, "rb+") != 0) {
        cout << "Ошибка открытия файла!\n";
        return;
    }

    Patient* patients = (Patient*)malloc(100 * sizeof(Patient));
    if (!patients) {
        cout << "Ошибка выделения памяти!\n";
        fclose(file);
        return;
    }

    int n = 0;

    while (fread(&patients[n], sizeof(Patient), 1, file) == 1) n++;

    int low = 0, high = n - 1;
    Patient pivot;
    int i, j;

    int* left = (int*)malloc(n * sizeof(int));
    int* right = (int*)malloc(n * sizeof(int));
    if (!left || !right) {
        cout << "Ошибка выделения памяти!\n";
        free(patients);
        fclose(file);
        return;
    }

    int p = -1;

    left[++p] = low;
    right[p] = high;

    while (p >= 0) {
        low = left[p];
        high = right[p--];

        pivot = patients[(low + high) / 2];
        i = low;
        j = high;

        while (i <= j) {
            while (patients[i].date.year < pivot.date.year ||
                (patients[i].date.year == pivot.date.year && patients[i].date.month < pivot.date.month) ||
                (patients[i].date.year == pivot.date.year && patients[i].date.month == pivot.date.month && patients[i].date.day < pivot.date.day)) {
                i++;
            }
            while (patients[j].date.year > pivot.date.year ||
                (patients[j].date.year == pivot.date.year && patients[j].date.month > pivot.date.month) ||
                (patients[j].date.year == pivot.date.year && patients[j].date.month == pivot.date.month && patients[j].date.day > pivot.date.day)) {
                j--;
            }

            if (i <= j) {
                Patient temp = patients[i];
                patients[i] = patients[j];
                patients[j] = temp;
                i++;
                j--;
            }
        }

        if (low < j) {
            left[++p] = low;
            right[p] = j;
        }
        if (i < high) {
            left[++p] = i;
            right[p] = high;
        }
    }

    freopen_s(&file, filename, "wb", file);
    fwrite(patients, sizeof(Patient), n, file);
    fclose(file);

    free(patients);
    free(left);
    free(right);

    cout << "Файл отсортирован по дате приёма\n";
}

void selectionSortByProcedure(const char* filename) {
    FILE* file;
    if (fopen_s(&file, filename, "r+b") != 0) {
        cout << "Ошибка открытия файла!\n";
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    int count = fileSize / sizeof(Patient);

    for (int i = 0; i < count - 1; i++) {
        int minIndex = i;
        Patient patientI, patientJ;

        for (int j = i + 1; j < count; j++) {
            fseek(file, minIndex * sizeof(Patient), SEEK_SET);
            fread(&patientI, sizeof(Patient), 1, file);

            fseek(file, j * sizeof(Patient), SEEK_SET);
            fread(&patientJ, sizeof(Patient), 1, file);

            if (strcmp(patientJ.type_of_procedure, patientI.type_of_procedure) < 0) {
                minIndex = j;
            }
        }

        if (minIndex != i) {
            Patient temp;

            fseek(file, minIndex * sizeof(Patient), SEEK_SET);
            fread(&temp, sizeof(Patient), 1, file);

            fseek(file, i * sizeof(Patient), SEEK_SET);
            fwrite(&temp, sizeof(Patient), 1, file);

            fseek(file, minIndex * sizeof(Patient), SEEK_SET);
            fwrite(&patientI, sizeof(Patient), 1, file);
        }
    }

    fclose(file);
    cout << "Файл отсортирован по типу процедуры\n";
}

void insertionSortByCost(const char* filename) {
    FILE* file;
    if (fopen_s(&file, filename, "rb+") != 0) {
        cout << "Ошибка открытия файла!\n";
        return;
    }

    long fileSize = 0;
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    int count = fileSize / sizeof(Patient);
    for (int i = 1; i < count; i++) {
        Patient key;
        fseek(file, i * sizeof(Patient), SEEK_SET);
        fread(&key, sizeof(Patient), 1, file);

        int j = i - 1;
        while (j >= 0) {
            Patient current;
            fseek(file, j * sizeof(Patient), SEEK_SET);
            fread(&current, sizeof(Patient), 1, file);

            if (current.cost > key.cost) {
                fseek(file, (j + 1) * sizeof(Patient), SEEK_SET);
                fwrite(&current, sizeof(Patient), 1, file);
                j--;
            }
            else {
                break;
            }
        }
        fseek(file, (j + 1) * sizeof(Patient), SEEK_SET);
        fwrite(&key, sizeof(Patient), 1, file);
    }

    fclose(file);
    cout << "Файл отсортирован по стоимости приёма\n";
}

void linearSearchByProcedure(const char* filename, const char* procedureType) {
    FILE* file;
    if (fopen_s(&file, filename, "rb") != 0) {
        cout << "Ошибка открытия файла!\n";
        return;
    }

    FILE* outputFile;
    if (fopen_s(&outputFile, "outputFile.txt", "w") != 0) {
        cout << "Ошибка при создании выходного файла!\n";
        fclose(file);
        return;
    }

    Patient patient;
    bool found = false;

    printf("%-20s %-15s %-15s %-15s %-20s %-15s\n", "ФИО", "Дата", "Время", "Тип процедуры", "Лечащий врач", "Стоимость");

    while (fread(&patient, sizeof(Patient), 1, file) == 1) {
        if (strcmp(patient.type_of_procedure, procedureType) == 0) {
            found = true;

            printf("%-20s %-15d.%d.%d %-15d:%d %-20s %-15s %-15d\n",
                patient.fio,
                patient.date.day, patient.date.month, patient.date.year,
                patient.time.hours, patient.time.minutes,
                patient.type_of_procedure,
                patient.doctor,
                patient.cost);

            fprintf(outputFile, "%-20s %-15d.%d.%d %-15d:%d %-20s %-15s %-15d\n",
                patient.fio,
                patient.date.day, patient.date.month, patient.date.year,
                patient.time.hours, patient.time.minutes,
                patient.type_of_procedure,
                patient.doctor,
                patient.cost);
        }
    }

    if (!found) {
        printf("Нет записей с типом процедуры \"%s\".\n", procedureType);
    }

    fclose(file);
    fclose(outputFile);
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "RU");

    const char* filename = "patients.txt";
    int choice;

    do {
        wall();
        cout << "Меню:\n";
        wall();
        cout << "1. Создание файла\n";
        cout << "2. Просмотр файла\n";
        cout << "3. Добавление записей\n";
        cout << "4. Редактирование\n";
        cout << "5. Удаление\n";
        cout << "6. Линейный поиск в файле по ФИО пациента\n";
        cout << "7. Бинарный поиск в файле по дате приёма\n";
        cout << "8. QuickSort по дате приёма\n";
        cout << "9. Сортировка выбором по типу процедуры\n";
        cout << "10. Сортировка вставками по стоимости приёма\n";
        cout << "11. Поиск по типу процедуры\n";
        cout << "0. Выход" << endl;
        wall();
        cout << "Выберите пункт: ";
        cin >> choice;
        wall();
        clear();

        switch (choice) {
        case 1: createFile(filename); break;
        case 2: viewFile(filename); break;
        case 3: addRecords(filename); break;
        case 4: editRecord(filename); break;
        case 5: deleteRecord(filename); break;
        case 6: {
            char fio[100];
            cout << "Введите ФИО пациента для поиска: ";
            cin.ignore();
            cin.getline(fio, sizeof(fio));
            linearSearch(filename, fio);
            break;
        }
        case 7: {
            Date searchDate;
            cout << "Введите дату (дд мм гггг): ";
            cin >> searchDate.day >> searchDate.month >> searchDate.year;
            binarySearchByDate(filename, searchDate);
            break;
        }
        case 8: quickSortByDate(filename); break;
        case 9: selectionSortByProcedure(filename); break;
        case 10: insertionSortByCost(filename); break;
        case 11: {
            char procedureType[100];
            cout << "Введите тип процедуры для поиска: ";
            cin.ignore();
            cin.getline(procedureType, sizeof(procedureType));
            linearSearchByProcedure(filename, procedureType);
            break;
        }
        case 0: cout << "Выход...\n"; break;
        default: cout << "Неверный выбор! Попробуйте снова." << endl;
        }
    } while (choice != 0);

    return 0;
}