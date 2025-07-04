#pragma once

struct Vacancy {
    int  id;
    char title[50];
    char company[50];
    double salary;
    char requirements[100];
    char publicationDate[20];
    bool isActive;
};

void wall();
void clear();
void logAction(const char* functionName, const char* input = "", const char* result = "");
void viewLogs();
void createFile(const char* filename);
void printVacancy(const Vacancy& vac);
void addVacancy(const char* filename);
void readFile(const char* filename);
void updateVacancy(const char* filename, int vacancyId);
void deleteVacancy(const char* filename, int vacancyId);
int  getRecordCount(const char* filename);
void inputVacancyFromUser(Vacancy& vac);
void linearSearchBySalary(const char* filename);
void binarySearchByTitle(const char* filename);
void quickSortBySalary(const char* sourceFilename, const char* sortedFilename);
void selectionSortByDate(const char* sourceFilename, const char* sortedFilename);
void insertionSortByCompany(const char* sourceFilename, const char* sortedFilename);
void searchByStatus(const char* filename);
bool checkDateFormat(const char* date);
void listActiveVacanciesAfterDate(const char* filename);
void printSalaryReport();
void printDateReport();
void printCompanyReport();