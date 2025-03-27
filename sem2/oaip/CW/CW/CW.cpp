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
        cout << "12. Список компаний, опубликованных после введенной даты и имеющие статус 'активна'\n";
        cout << "13. Посмотреть файл, отсортированный по зарплате\n";
        cout << "14. Посмотреть файл, отсортированный по дате\n";
        cout << "15. Посмотреть файл, отсортированный по компании\n";
        cout << "16. Выход\n";
        wall();
        cout << "Выберите действие: ";

        int choice;
        cin >> choice;

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
        case 4: {
            wall();
            cout << "Введите ID вакансии для изменения: ";
            int id;
            cin >> id;
            wall();
            updateVacancy(FILE_NAME, id);
            wall();
            clear();
            break;
        }
        case 5: {
            wall();
            cout << "Введите ID вакансии для удаления: ";
            int id;
            cin >> id;
            wall();
            deleteVacancy(FILE_NAME, id);
            wall();
            clear();
            break;
        }
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
            quickSortBySalary(FILE_NAME, "vacancies_sorted_salary.bin");
            wall();
            clear();
            break;

        case 9:
            wall();
            selectionSortByDate(FILE_NAME, "vacancies_sorted_date.bin");
            wall();
            clear();
            break;

        case 10:
            wall();
            insertionSortByCompany(FILE_NAME, "vacancies_sorted_company.bin");
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
            listActiveCompaniesAfterDate(FILE_NAME);
            clear();
            break;

        case 13:
            wall();
            readSortedSalaryFile();
            clear();
            break;

        case 14:
            wall();
            readSortedDateFile();
            clear();
            break;

        case 15:
            wall();
            readSortedCompanyFile();
            clear();
            break;

        case 16:
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