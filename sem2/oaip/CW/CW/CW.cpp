#include <iostream>
#include <fstream>
#include <cstring>
#include "func.hpp"

using namespace std;

const char* FILE_NAME = "vacancies.bin";

int main() {
    setlocale(LC_ALL, "Russian");

    while (true) {
        wall();
        cout << "Список операций:\n";
        wall();
        cout << "1. Создать бинарный файл\n";
        cout << "2. Посмотреть вакансии\n";
        cout << "3. Добавить вакансию\n";
        cout << "4. Изменить информацию о вакансии\n";
        cout << "5. Удалить вакансию\n";
        cout << "6. Линейный поиск по зарплате\n";
        cout << "7. Бинарный поиск по названию должности\n";
        cout << "8. Быстрая сортировка по зарплате\n";
        cout << "9. Сортировка выбором по дате публикации\n";
        cout << "10. Сортировка вставками по названию компании\n";
        cout << "11. Поиск по признаку\n";
        cout << "12. Список вакансий, опубликованных после введенной даты и имеющие статус 'активна'\n";
        cout << "13. Показать отчет по зарплате\n";
        cout << "14. Показать отчет по дате публикации\n";
        cout << "15. Показать отчет по компании\n";
        cout << "16. Показать логи\n";
        cout << "17. Выход\n";
        wall();
        cout << "Выберите действие: ";

        int choice;
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            wall();
            clear();
            wall();
            cout << "Неверный ввод. Введите число.\n";
            wall();
            clear();
            break;
        }

        wall();
        clear();

        switch (choice) {
        case 1:
            createFile(FILE_NAME);
            wall();
            cout << "Файл успешно создан.\n";
            wall();
            clear();
            break;
        case 2:
            wall();
            readFile(FILE_NAME);
            clear();
            break;
        case 3:
            wall();
            addVacancy(FILE_NAME);
            wall();
            clear();
            break;
        case 4:
            wall();
            cout << "Введите ID вакансии для изменения: ";
            int id;
            cin >> id;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка: ID должен быть числом.\n";
                break;
            }
            updateVacancy(FILE_NAME, id);
            clear();
            break;

        case 5:
            wall();
            cout << "Введите ID вакансии для удаления: ";
            cin >> id;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                wall();
                cout << "Ошибка: ID должен быть числом.\n";
                wall();
                clear();
                break;
            }
            deleteVacancy(FILE_NAME, id);
            clear();
            break;
        case 6:
            wall();
            linearSearchBySalary(FILE_NAME);
            clear();
            break;
        case 7:
            wall();
            binarySearchByTitle(FILE_NAME);
            clear();
            break;

        case 8:
            wall();
            quickSortBySalary(FILE_NAME, "");
            clear();
            break;

        case 9:
            wall();
            selectionSortByDate(FILE_NAME, "");
            wall();
            clear();
            break;

        case 10:
            wall();
            insertionSortByCompany(FILE_NAME, "");
            wall();
            clear();
            break;

        case 11:
            wall();
            searchByStatus(FILE_NAME);
            clear();
            break;

        case 12:
            wall();
            listActiveVacanciesAfterDate(FILE_NAME);
            clear();
            break;

        case 13:
            wall();
            printSalaryReport();
            clear();
            break;

        case 14:
            wall();
            printDateReport();
            clear();
            break;

        case 15:
            wall();
            printCompanyReport();
            clear();
            break;
        case 16:
            viewLogs();
            break;

        case 17:
            return 0;
        default:
            wall();
            cout << "Некорректный ввод. Повторите попытку.\n";
            wall();
            clear();
            break;
        }
    }
    return 0;
}