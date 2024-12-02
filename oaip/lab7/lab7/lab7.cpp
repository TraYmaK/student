#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

struct Student {
    string surname;
    string group;
    double physics;
    double math;
    double informatics;
    double average;

    void Average() {
        average = (physics + math + informatics) / 3.0;
    }

    void input() {
        cout << "Введите фамилию студента: ";
        cin >> surname;
        cout << "Введите номер группы: ";
        cin >> group;
        cout << "Введите оценку по физике: ";
        cin >> physics;
        cout << "Введите оценку по математике: ";
        cin >> math;
        cout << "Введите оценку по информатике: ";
        cin >> informatics;
        Average();
    }
};

void CreateFile(string FileName) {
    ofstream file(FileName);

    if (file.is_open()) {
        file.close();
        cout << "Файл успешно создан!\n";
    }
    else {
        cout << "Ошибка.Не удалось создать файл.\n";
        system("pause");
        system("cls");
    }
}

void ReadFile(string FileName) {
    ifstream file(FileName);

    if (file.is_open()) {
        string line;

        while (getline(file, line)) {
            cout << line << endl;
        }

        file.close();
    }
    else {
        cout << "Ошибка.Не удалось открыть файл.\n";
        system("pause");
        system("cls");
    }
}

void AddToFile(string FileName) {
    ofstream file(FileName, ios::app);

    if (file.is_open()) {
        Student student;

        while (true) {
            system("cls");
            cout << "Вы хотите ввести все параметры студента или только конкретные?\n";
            cout << "1. Ввести все параметры.\n";
            cout << "2. Ввести конкретные параметры.\n";
            cout << "Ваш выбор: ";
            int choice;
            cin >> choice;

            if (cin.fail() || (choice != 1 && choice != 2)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Некорректный ввод. Попробуйте снова.\n";
                system("pause");
                continue;
            }

            if (choice == 1) {
                student.input();
            }
            else if (choice == 2) {
                cin.ignore();
                string temp;
                double tempDouble;

                cout << "Введите фамилию студента (оставьте пустым, если не хотите менять): ";
                getline(cin, temp);
                if (!temp.empty()) student.surname = temp;

                cout << "Введите номер группы (оставьте пустым, если не хотите менять): ";
                getline(cin, temp);
                if (!temp.empty()) student.group = temp;

                while (true) {
                    cout << "Введите оценку по физике (введите -1, если не хотите менять): ";
                    cin >> tempDouble;

                    if (!cin.fail() && (tempDouble >= -1 && tempDouble <= 10)) {
                        if (tempDouble >= 0) student.physics = tempDouble;
                        break;
                    }
                    else {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Некорректная оценка. Попробуйте снова.\n";
                        system("pause");
                        system("cls");
                    }
                }

                while (true) {
                    cout << "Введите оценку по математике (введите -1, если не хотите менять): ";
                    cin >> tempDouble;

                    if (!cin.fail() && (tempDouble >= -1 && tempDouble <= 10)) {
                        if (tempDouble >= 0) student.math = tempDouble;
                        break;
                    }
                    else {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Некорректная оценка. Попробуйте снова.\n";
                        system("pause");
                        system("cls");
                    }
                }

                while (true) {
                    cout << "Введите оценку по информатике (введите -1, если не хотите менять): ";
                    cin >> tempDouble;

                    if (!cin.fail() && (tempDouble >= -1 && tempDouble <= 10)) {
                        if (tempDouble >= 0) student.informatics = tempDouble;
                        break;
                    }
                    else {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Некорректная оценка. Попробуйте снова.\n";
                        system("pause");
                        system("cls");
                    }
                }
                student.Average();
            }
            break;
        }

        file << student.surname << " "
            << student.group << " "
            << student.physics << " "
            << student.math << " "
            << student.informatics << " "
            << fixed << setprecision(2) << student.average << endl;

        file.close();

        cout << "Данные успешно занесены в файл!\n";
    }
    else {
        cout << "Ошибка. Не удалось открыть файл.\n";
        system("pause");
        system("cls");
    }
}

void FindAverage(string FileName) {
    ifstream file(FileName);

    if (!file.is_open()) {
        cout << "Ошибка. Не удалось открыть файл.\n";
        system("pause");
        system("cls");
        return;
    }

    string groupToFind;
    double minAverage;

    cout << "Введите номер группы для поиска: ";
    cin >> groupToFind;

    cout << "Введите минимальный средний балл: ";
    cin >> minAverage;

    if (cin.fail() || minAverage < 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Некорректный ввод минимального среднего балла.\n";
        system("pause");
        system("cls");
        return;
    }

    cout << "\nСписок студентов группы \"" << groupToFind
        << "\", у которых средний балл выше " << minAverage << ":\n";

    string surname, group;
    double physics, math, informatics, average;
    bool found = false;

    while (file >> surname >> group >> physics >> math >> informatics >> average) {
        if (group == groupToFind && average > minAverage) {
            cout << surname << " " << group << " "
                << physics << " " << math << " "
                << informatics << " Средний балл: "
                << fixed << setprecision(2) << average << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "В данной группе нет студентов со средним баллом, который выше введённого.\n";
    }

    file.close();
    system("pause");
    system("cls");
}

void EditFile(string FileName) {
    ifstream file(FileName);

    if (!file.is_open()) {
        cout << "Ошибка. Не удалось открыть файл.\n";
        return;
    }

    ofstream tempFile("W:\\test\\temp.txt");
    if (!tempFile.is_open()) {
        cout << "Ошибка. Не удалось создать временный файл.\n";
        return;
    }

    string surname, group;
    double physics, math, informatics, average;
    bool found = false;

    cout << "Введите фамилию студента, которого хотите отредактировать: ";
    string targetSurname;
    cin >> targetSurname;

    while (file >> surname >> group >> physics >> math >> informatics >> average) {
        if (surname == targetSurname) {
            found = true;
            cout << "Редактируем данные студента: " << surname << endl;

            string tempStr;
            double tempDouble;

            cout << "Введите новую фамилию (оставьте пустым, чтобы не менять): ";
            cin.ignore();
            getline(cin, tempStr);
            if (!tempStr.empty()) surname = tempStr;

            cout << "Введите новую группу (оставьте пустым, чтобы не менять): ";
            getline(cin, tempStr);
            if (!tempStr.empty()) group = tempStr;

            cout << "Введите новую оценку по физике (или -1, чтобы не менять): ";
            cin >> tempDouble;
            if (tempDouble >= 0) physics = tempDouble;

            cout << "Введите новую оценку по математике (или -1, чтобы не менять): ";
            cin >> tempDouble;
            if (tempDouble >= 0) math = tempDouble;

            cout << "Введите новую оценку по информатике (или -1, чтобы не менять): ";
            cin >> tempDouble;
            if (tempDouble >= 0) informatics = tempDouble;

            average = (physics + math + informatics) / 3.0;
        }

        tempFile << surname << " " << group << " "
            << physics << " " << math << " "
            << informatics << " " << fixed << setprecision(2) << average << endl;
    }

    file.close();
    tempFile.close();

    if (!found) {
        cout << "Студент с фамилией \"" << targetSurname << "\" не найден.\n";
        remove("W:\\test\\temp.txt");
        system("pause");
        system("cls");
        return;
    }

    if (remove(FileName.c_str()) != 0 || rename("W:\\test\\temp.txt", FileName.c_str()) != 0) {
        cout << "Ошибка при обновлении файла.\n";
    }
    else {
        cout << "Данные успешно обновлены.\n";
    }
}

void DeleteStudent(string FileName) {
    ifstream file(FileName);

    if (!file.is_open()) {
        cout << "Ошибка. Не удалось открыть файл.\n";
        system("pause");
        system("cls");
        return;
    }

    ofstream tempFile("W:\\test\\temp.txt");
    if (!tempFile.is_open()) {
        cout << "Ошибка. Не удалось создать временный файл.\n";
        system("pause");
        system("cls");
        return;
    }

    string surname, group;
    double physics, math, informatics, average;
    bool found = false;

    cout << "Введите фамилию студента, которого хотите удалить: ";
    string targetSurname;
    cin >> targetSurname;

    while (file >> surname >> group >> physics >> math >> informatics >> average) {
        if (surname == targetSurname) {
            found = true;
            cout << "Студент \"" << surname << "\" успешно удалён.\n";
            system("pause");
            system("cls");
            continue;
        }

        tempFile << surname << " " << group << " "
            << physics << " " << math << " "
            << informatics << " " << fixed << setprecision(2) << average << endl;
    }

    file.close();
    tempFile.close();

    if (!found) {
        cout << "Студент с фамилией \"" << targetSurname << "\" не найден.\n";
        remove("W:\\test\\temp.txt");
        system("pause");
        system("cls");
        return;
    }

    if (remove(FileName.c_str()) != 0 || rename("W:\\test\\temp.txt", FileName.c_str()) != 0) {
        cout << "Ошибка при обновлении файла.\n";
        system("pause");
        system("cls");
    }
    else {
        cout << "Данные успешно обновлены.\n";
        system("pause");
        system("cls");
    }
}

void Sort(string FileName) {
    ifstream file(FileName);

    if (!file.is_open()) {
        cout << "Ошибка. Не удалось открыть файл.\n";
        system("pause");
        system("cls");
        return;
    }

    Student students[100];
    int count = 0;

    while (file >> students[count].surname >> students[count].group
        >> students[count].physics >> students[count].math
        >> students[count].informatics >> students[count].average) {
        count++;
    }

    file.close();

    if (count == 0) {
        cout << "Файл пуст. Сортировка не требуется.\n";
        system("pause");
        system("cls");
        return;
    }

    cout << "Выберите тип сортировки:\n";
    cout << "1. По фамилии (алфавитный порядок).\n";
    cout << "2. По среднему баллу (по возрастанию).\n";
    cout << "Ваш выбор: ";
    int choice;
    cin >> choice;

    if (cin.fail() || (choice != 1 && choice != 2)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Некорректный ввод! Сортировка отменена.\n";
        system("pause");
        system("cls");
        return;
    }

    if (choice == 1) {
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (students[j].surname > students[j + 1].surname) {
                    swap(students[j], students[j + 1]);
                }
            }
        }
    }
    else if (choice == 2) {
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (students[j].average > students[j + 1].average) {
                    swap(students[j], students[j + 1]);
                }
            }
        }
    }

    ofstream outFile(FileName);
    if (!outFile.is_open()) {
        cout << "Ошибка. Не удалось открыть файл для записи.\n";
        system("pause");
        system("cls");
        return;
    }

    for (int i = 0; i < count; i++) {
        outFile << students[i].surname << " " << students[i].group << " "
            << students[i].physics << " " << students[i].math << " "
            << students[i].informatics << " "
            << fixed << setprecision(2) << students[i].average << endl;
    }

    outFile.close();

    cout << "Сортировка выполнена успешно.\n";
    system("pause");
    system("cls");
}

void Exit() {
    exit(1);
}

void HighBar() {
    cout << "Список операций: \n";
    cout << "1. Создать файл.\n";
    cout << "2. Прочесть содержимое файла.\n";
    cout << "3. Добавить данные в файл.\n";
    cout << "4. Поиск среднего балла, выше введённого с клавиатуры, в определённой группе.\n";
    cout << "5. Редактирование.\n";
    cout << "6. Удаление данных о студенте.\n";
    cout << "7. Сортировка.\n";
    cout << "8. Выход\n\n";
    cout << "Выберите операцию: ";
}

void menu(string FileName) {
    bool processing = true;
    while (processing) {
        system("cls");
        int choice;
        do {
            HighBar();
            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Некорректный ввод! Попробуйте снова.\n";
                continue;
            }

            switch (choice) {
            case 1:
                CreateFile(FileName);
                break;
            case 2:
                ReadFile(FileName);
                break;
            case 3:
                AddToFile(FileName);
                break;
            case 4:
                FindAverage(FileName);
                break;
            case 5:
                EditFile(FileName);
                break;
            case 6:
                DeleteStudent(FileName);
                break;
            case 7:
                Sort(FileName);
                break;
            case 8:
                Exit();
                break;
            default:
                cout << "Неверный выбор! Попробуйте снова.\n";
                system("pause");
                system("cls");
            }
        } while (choice < 1 || choice > 3);
        system("pause");
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    string FileName = "W:\\test\\student.txt";
    menu(FileName);
    return 0;
}