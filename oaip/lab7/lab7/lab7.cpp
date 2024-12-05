#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm>

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
        cout << "File created successfully!\n";
    }
    else {
        cout << "Error. Failed to create file.\n";
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
        cout << "Error. Failed to open file.\n";
    }
}

void AddToFile(string FileName) {
    ofstream file(FileName, ios::app);

    if (file.is_open()) {
        Student student;

        student.input();

        file << "Surname: " << student.surname
            << " Group: " << student.group
            << " Physics: " << student.physics
            << " Math: " << student.math
            << " Informatics: " << student.informatics
            << " Average Grade: " << fixed << setprecision(2) << student.average
            << endl;

        file.close();

        cout << "Data successfully added to the file!\n";
    }
    else {
        cout << "Error. Failed to open file.\n";
    }
}

void FindAverage(string FileName) {
    ifstream file(FileName);

    if (!file.is_open()) {
        cout << "Error. Failed to open file.\n";
        return;
    }

    string groupToFind;
    double minAverage;

    cout << "Enter the group number to search: ";
    cin >> groupToFind;

    cout << "Enter the minimum average grade: ";
    cin >> minAverage;

    if (cin.fail() || minAverage < 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input for minimum average grade.\n";
        return;
    }

    cout << "\nList of students in group \"" << groupToFind
        << "\" with an average grade above " << minAverage << ":\n";

    string line;
    bool found = false;

    while (getline(file, line)) {
        size_t groupPos = line.find("Group: ");
        if (groupPos != string::npos) {
            string group = line.substr(groupPos + 7, line.find(" ", groupPos + 7) - (groupPos + 7));

            size_t avgPos = line.find("Average Grade: ");
            if (avgPos != string::npos) {
                double average = stod(line.substr(avgPos + 14));

                if (group == groupToFind && average > minAverage) {
                    cout << line << endl;
                    found = true;
                }
            }
        }
    }

    if (!found) {
        cout << "No students in this group have an average grade above the entered value.\n";
    }

    file.close();
}

void EditFile(string FileName) {
    ifstream file(FileName);

    if (!file.is_open()) {
        cout << "Error. Failed to open file.\n";
        return;
    }

    ofstream tempFile("temp.txt");
    if (!tempFile.is_open()) {
        cout << "Error. Failed to create temporary file.\n";
        file.close();
        return;
    }

    string line;
    bool found = false;

    cout << "Enter the surname of the student you want to edit: ";
    string targetSurname;
    cin >> targetSurname;

    while (getline(file, line)) {
        size_t pos = line.find("Surname: ");
        if (pos != string::npos) {
            size_t endPos = line.find(" ", pos + 9);
            if (endPos == string::npos) endPos = line.length();
            string surname = line.substr(pos + 9, endPos - (pos + 9));

            if (surname == targetSurname) {
                found = true;
                cout << "Editing student data: " << line << endl;

                Student student;
                student.surname = surname;

                try {
                    size_t posGroup = line.find("Group: ");
                    size_t endGroup = line.find(" ", posGroup + 7);
                    if (endGroup == string::npos) endGroup = line.length();
                    student.group = line.substr(posGroup + 7, endGroup - (posGroup + 7));

                    size_t posPhysics = line.find("Physics: ");
                    size_t endPhysics = line.find(" ", posPhysics + 9);
                    if (endPhysics == string::npos) endPhysics = line.length();
                    string physicsStr = line.substr(posPhysics + 9, endPhysics - (posPhysics + 9));
                    student.physics = stod(physicsStr);

                    size_t posMath = line.find("Math: ");
                    size_t endMath = line.find(" ", posMath + 6);
                    if (endMath == string::npos) endMath = line.length();
                    string mathStr = line.substr(posMath + 6, endMath - (posMath + 6));
                    student.math = stod(mathStr);

                    size_t posInformatics = line.find("Informatics: ");
                    size_t endInformatics = line.find(" ", posInformatics + 13);
                    if (endInformatics == string::npos) endInformatics = line.length();
                    string informaticsStr = line.substr(posInformatics + 13, endInformatics - (posInformatics + 13));
                    student.informatics = stod(informaticsStr);

                    size_t posAverage = line.find("Average Grade: ");
                    if (posAverage != string::npos) {
                        string averageStr = line.substr(posAverage + 14);
                        student.average = stod(averageStr);
                    }
                    else {
                        student.Average();
                    }
                }
                catch (exception& e) {
                    cout << "Error parsing student data: " << e.what() << endl;
                    tempFile << line << endl;
                    continue;
                }

                
                cin.ignore();
                cout << "Enter new surname (leave empty to keep unchanged): ";
                string tempStr;
                getline(cin, tempStr);
                if (!tempStr.empty()) student.surname = tempStr;

                cout << "Enter new group (leave empty to keep unchanged): ";
                getline(cin, tempStr);
                if (!tempStr.empty()) student.group = tempStr;

                cout << "Enter new physics grade (or -1 to keep unchanged): ";
                double tempDouble;
                cin >> tempDouble;
                if (tempDouble >= 0) student.physics = tempDouble;

                cout << "Enter new math grade (or -1 to keep unchanged): ";
                cin >> tempDouble;
                if (tempDouble >= 0) student.math = tempDouble;

                cout << "Enter new informatics grade (or -1 to keep unchanged): ";
                cin >> tempDouble;
                if (tempDouble >= 0) student.informatics = tempDouble;

                student.Average();

                tempFile << "Surname: " << student.surname
                    << " Group: " << student.group
                    << " Physics: " << student.physics
                    << " Math: " << student.math
                    << " Informatics: " << student.informatics
                    << " Average Grade: " << fixed << setprecision(2) << student.average
                    << endl;
            }
            else {
                tempFile << line << endl;
            }
        }
        else {
            tempFile << line << endl;
        }
    }

    file.close();
    tempFile.close();

    if (!found) {
        cout << "Student with surname \"" << targetSurname << "\" not found.\n";
        remove("temp.txt");
        return;
    }

    if (remove(FileName.c_str()) != 0 || rename("temp.txt", FileName.c_str()) != 0) {
        cout << "Error updating the file.\n";
    }
    else {
        cout << "Data updated successfully.\n";
    }
}

void DeleteStudent(string FileName) {
    ifstream file(FileName);

    if (!file.is_open()) {
        cout << "Error. Failed to open file.\n";
        return;
    }

    ofstream tempFile("temp.txt");
    if (!tempFile.is_open()) {
        cout << "Error. Failed to create temporary file.\n";
        file.close();
        return;
    }

    cout << "Enter the surname of the student you want to delete: ";
    string targetSurname;
    cin.ignore();
    getline(cin, targetSurname);

    string line;
    bool found = false;

    while (getline(file, line)) {
        size_t posSurname = line.find("Surname: ");
        if (posSurname != string::npos) {
            size_t posGroup = line.find(" Group: ", posSurname);
            if (posGroup == string::npos) posGroup = line.length();
            string surname = line.substr(posSurname + 9, posGroup - (posSurname + 9));

            if (surname == targetSurname) {
                found = true;
                cout << "Student \"" << surname << "\" successfully deleted.\n";
                continue;
            }
        }
        tempFile << line << endl;
    }

    file.close();
    tempFile.close();

    if (!found) {
        cout << "Student with surname \"" << targetSurname << "\" not found.\n";
        remove("temp.txt");
        return;
    }

    if (remove(FileName.c_str()) != 0 || rename("temp.txt", FileName.c_str()) != 0) {
        cout << "Error updating the file.\n";
    }
    else {
        cout << "Data updated successfully.\n";
    }
}

void Sort(string FileName) {
    ifstream file(FileName);

    if (!file.is_open()) {
        cout << "Error. Failed to open file.\n";
        return;
    }

    Student students[100];
    int count = 0;

    string line;
    while (getline(file, line)) {
        size_t surnamePos = line.find("Surname: ");
        if (surnamePos != string::npos) {
            size_t posGroup = line.find(" Group: ", surnamePos);
            if (posGroup == string::npos) continue;
            Student student;
            student.surname = line.substr(surnamePos + 9, posGroup - (surnamePos + 9));

            size_t physicsPos = line.find(" Physics: ", posGroup);
            if (physicsPos == string::npos) continue;
            student.group = line.substr(posGroup + 8, physicsPos - (posGroup + 8));

            size_t mathPos = line.find(" Math: ", physicsPos);
            if (mathPos == string::npos) continue;
            student.physics = stod(line.substr(physicsPos + 9, mathPos - (physicsPos + 9)));

            size_t informaticsPos = line.find(" Informatics: ", mathPos);
            if (informaticsPos == string::npos) continue;
            student.math = stod(line.substr(mathPos + 7, informaticsPos - (mathPos + 7)));

            size_t averagePos = line.find(" Average Grade: ", informaticsPos);
            if (averagePos == string::npos) continue;
            student.informatics = stod(line.substr(informaticsPos + 14, averagePos - (informaticsPos + 14)));

            student.average = stod(line.substr(averagePos + 15));

            students[count++] = student;
        }
    }

    file.close();

    if (count == 0) {
        cout << "The file is empty. Sorting is not required.\n";
        return;
    }

    cout << "Choose sorting type:\n";
    cout << "1. By surname (alphabetical order).\n";
    cout << "2. By average grade (ascending).\n";
    cout << "3. By average grade (descending).\n";
    cout << "Your choice: ";
    int choice;
    cin >> choice;

    if (cin.fail() || (choice < 1 || choice > 3)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Sorting canceled.\n";
        return;
    }

    if (choice == 1) {
        sort(students, students + count, [](const Student& a, const Student& b) {
            return a.surname < b.surname;
            });
    }
    else if (choice == 2) {
        sort(students, students + count, [](const Student& a, const Student& b) {
            return a.average < b.average;
            });
    }
    else if (choice == 3) {
        sort(students, students + count, [](const Student& a, const Student& b) {
            return a.average > b.average;
            });
    }

    ofstream outFile(FileName);
    if (!outFile.is_open()) {
        cout << "Error. Failed to open file for writing.\n";
        return;
    }

    for (int i = 0; i < count; i++) {
        outFile << "Surname: " << students[i].surname
            << " Group: " << students[i].group
            << " Physics: " << students[i].physics
            << " Math: " << students[i].math
            << " Informatics: " << students[i].informatics
            << " Average Grade: " << fixed << setprecision(2) << students[i].average
            << endl;
    }

    outFile.close();

    cout << "Sorting completed successfully.\n";
}

void Exit() {
    exit(1);
}

void HighBar() {
    cout << "List of operations: \n";
    cout << "1. Create file.\n";
    cout << "2. Read file contents.\n";
    cout << "3. Add data to file.\n";
    cout << "4. Search for average grade higher than the input value in a specific group.\n";
    cout << "5. Edit data.\n";
    cout << "6. Delete student data.\n";
    cout << "7. Sort data.\n";
    cout << "8. Exit\n\n";
    cout << "Choose an operation: ";
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
                cout << "Invalid input! Please try again.\n";
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
                cout << "Invalid choice! Please try again.\n";
                system("pause");
                system("cls");
            }
        } while (choice < 1 || choice > 8);
        system("pause");
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    system("chcp 1251");
    string FileName = "W:\\test\\student.txt";
    menu(FileName);
    return 0;
}