#include <iostream>
#include <fstream>
#include <filesystem>
#include "constants.h"
#include "data_base_functions.h"
#include "prints.h"

extern std::vector<int> max_name_length;
extern std::vector<int> max_path_length;
extern std::vector<int> current_id_student;
extern std::vector<int> current_id_variant;

void MakeNewFiles(std::vector<std::string> &students_table_name, std::vector<std::string> &variants_table_name,
                  std::vector<std::string> &directory) {

    std::string dir;
    std::cout << "Input new data base name: ";
    std::cin >> dir;

    while (std::filesystem::exists(dir)) {
        std::cout << "\n\nDirectory already exists :(";
        std::cout << "\nInput new data base name: ";
        std::cin >> dir;
    }

    std::filesystem::create_directory(dir);
    directory.push_back(dir);

    std::string students_table, variants_table;
    students_table = dir + "/students.txt";
    variants_table = dir + "/variants.txt";

    std::ofstream fin(students_table);
    fin.close();
    std::ofstream fin1(variants_table);
    fin1.close();

    students_table_name.push_back(students_table);
    variants_table_name.push_back(variants_table);
}

void AskDBName(std::vector<std::string> &students_table_name, std::vector<std::string> &variants_table_name,
               std::vector<std::string> &directory) {

    std::string dir;
    std::cout << "\nType the name of your database: ";
    std::cin >> dir;

    while (!std::filesystem::exists(dir)) {
        std::cout << "\n\nCan't find data base :(";
        std::cout << "\nType the name of your database: ";
        std::cin >> dir;
    }
    directory.push_back(dir);

    std::string students_table;
    std::cout << "\nType the name of file with students' table: ";
    std::cin >> students_table;
    students_table_name.push_back(students_table);

    std::string variants_table;
    std::cout << "\nType the name of file with variants' table: ";
    std::cin >> variants_table;
    variants_table_name.push_back(variants_table);
}

void AskFileNames(std::vector<std::string> &students_table_name, std::vector<std::string> &variants_table_name,
                  std::vector<std::string> &directory) {

    std::string dir;
    std::cout << "\nType the name of your database: ";
    std::cin >> dir;
    directory.push_back(dir);

    std::filesystem::create_directory(dir);

    std::string students_table;
    std::cout << "\nType the name of file with students' table: ";
    std::cin >> students_table;
    students_table_name.push_back(students_table);

    std::string variants_table;
    std::cout << "\nType the name of file with variants' table: ";
    std::cin >> variants_table;
    variants_table_name.push_back(variants_table);
}

void InitDB(std::vector<std::string> &students_table_name, std::vector<std::string> &variants_table_name,
            std::vector<std::string> &directory) {

    std::cout << "\n-Initialising database-\n";

    std::string user_response;


    max_name_length.push_back(10);
    max_path_length.push_back(13);
    current_id_student.push_back(0);
    current_id_variant.push_back(0);

    while (user_response != "1" && user_response != "2" && user_response != "3") {
        std::cout << "1. Create empty database\n";
        std::cout << "2. Take data from existing data base\n";
        std::cout << "3. Take data from existing files\n";

        std::cout << "Type the number (1/2/3): ";
        std::cin >> user_response;
    }

    switch (std::stoi(user_response)) {
        case 1:
            MakeNewFiles(students_table_name, variants_table_name, directory);
            break;
        case 2:
            AskDBName(students_table_name, variants_table_name, directory);
            break;
        case 3:
            AskFileNames(students_table_name, variants_table_name, directory);
    }
}

std::string AddSpaces(int already, const int &needed) {
    std::string result;
    for (int i = already; i < needed; i++) {
        result += " ";
    }
    return result;
}

void PrintResultConsole(std::vector<std::vector<StudentVariant>> &testing_table,
                        const std::vector<std::vector<Student>> &students,
                        const std::vector<std::vector<Variant>> &variants, int table_index) {

    std::string header = "\nfull_name" + AddSpaces(9, max_name_length[table_index]);
    header += "path_to_file" + AddSpaces(12, max_path_length[table_index]) + "mark\n";
    std::cout << header;
    for (auto student_variant: testing_table[table_index]) {
        int index = FindStudent(student_variant.student_id, students[table_index]);
        std::cout << students[table_index][index].surname
                  << ' ' << students[table_index][index].name
                  << ' ' << students[table_index][index].patronymic;
        int help = students[table_index][index].surname.size();
        if (!(students[table_index][index].surname[0] >= 'a' &&
              students[table_index][index].surname[0] <= 'z' ||
              students[table_index][index].surname[0] >= 'A' &&
              students[table_index][index].surname[0] <= 'Z' ||
              students[table_index][index].surname[0] >= '0' &&
              students[table_index][index].surname[0] <= '9')) {
            help = (help + 1) / 2;
        }
        int help1 = students[table_index][index].name.size();
        if (!(students[table_index][index].name[0] >= 'a' &&
              students[table_index][index].name[0] <= 'z' ||
              students[table_index][index].name[0] >= 'A' &&
              students[table_index][index].name[0] <= 'Z' ||
              students[table_index][index].name[0] >= '0' &&
              students[table_index][index].name[0] <= '9')) {
            help1 = (help1 + 1) / 2;
        }
        int help2 = students[table_index][index].patronymic.size();
        if (!(students[table_index][index].patronymic[0] >= 'a' &&
              students[table_index][index].patronymic[0] <= 'z' ||
              students[table_index][index].patronymic[0] >= 'A' &&
              students[table_index][index].patronymic[0] <= 'Z' ||
              students[table_index][index].patronymic[0] >= '0' &&
              students[table_index][index].patronymic[0] <= '9')) {
            help2 = (help2 + 1) / 2;
        }
        std::cout << AddSpaces(help + help1 + help2 + 2, max_name_length[table_index]);
        index = FindVariant(student_variant.variant_id, variants[table_index]);
        std::cout << variants[table_index][index].path
                  << AddSpaces(static_cast<int>(variants[table_index][index].path.size()),
                               max_path_length[table_index]);
        if (student_variant.grade == -1) {
            std::cout << " \n";
        } else {
            std::cout << student_variant.grade << '\n';
        }
    }
}

void PrintResultFile(std::vector<std::vector<StudentVariant>> &testing_table,
                     const std::vector<std::vector<Student>> &students,
                     const std::vector<std::vector<Variant>> &variants, const std::vector<std::string> &directory,
                     int table_index) {
    std::ofstream file_result;
    file_result.open(directory[table_index] + "/result.txt");
    std::string header = "full_name" + AddSpaces(9, max_name_length[table_index]);
    header += "path_to_file" + AddSpaces(12, max_path_length[table_index]) + "mark\n";
    file_result << header;
    for (auto student_variant: testing_table[table_index]) {
        int index = FindStudent(student_variant.student_id, students[table_index]);
        file_result << students[table_index][index].surname
                    << ' ' << students[table_index][index].name
                    << ' ' << students[table_index][index].patronymic;
        int help = students[table_index][index].surname.size();
        if (students[table_index][index].surname == "A") {
            help = help;
        }
        if (!(students[table_index][index].surname[0] >= 'a' &&
              students[table_index][index].surname[0] <= 'z' ||
              students[table_index][index].surname[0] >= 'A' &&
              students[table_index][index].surname[0] <= 'Z' ||
              students[table_index][index].surname[0] >= '0' &&
              students[table_index][index].surname[0] <= '9')) {
            help = (help + 1) / 2;
        }
        int help1 = students[table_index][index].name.size();
        if (!(students[table_index][index].name[0] >= 'a' &&
              students[table_index][index].name[0] <= 'z' ||
              students[table_index][index].name[0] >= 'A' &&
              students[table_index][index].name[0] <= 'Z' ||
              students[table_index][index].name[0] >= '0' &&
              students[table_index][index].name[0] <= '9')) {
            help1 = (help1 + 1) / 2;
        }
        int help2 = students[table_index][index].patronymic.size();
        if (!(students[table_index][index].patronymic[0] >= 'a' &&
              students[table_index][index].patronymic[0] <= 'z' ||
              students[table_index][index].patronymic[0] >= 'A' &&
              students[table_index][index].patronymic[0] <= 'Z' ||
              students[table_index][index].patronymic[0] >= '0' &&
              students[table_index][index].patronymic[0] <= '9')) {
            help2 = (help2 + 1) / 2;
        }
        file_result << AddSpaces(help + help1 + help2 + 2, max_name_length[table_index]);
        index = FindVariant(student_variant.variant_id, variants[table_index]);
        file_result << variants[table_index][index].path
                    << AddSpaces(static_cast<int>(variants[table_index][index].path.size()),
                                 max_path_length[table_index]);
        if (student_variant.grade == -1) {
            file_result << " \n";
        } else {
            file_result << student_variant.grade << '\n';
        }
    }
}

void PrintResult(std::vector<std::vector<StudentVariant>> &testing_table,
                 const std::vector<std::vector<Student>> &students,
                 const std::vector<std::vector<Variant>> &variants, const std::vector<std::string> &directory) {

    std::cout << "\n-Printing table-\n";
    int table_index = 0;
    if (students.size() > 1) {
        std::cout << "\nChoose data base:";
        int user_response = -1;
        while (user_response < 0 || user_response > students.size()) {
            for (size_t i = 0; i < students.size(); i++) {
                std::cout << '\n' << i + 1 << ". Print " << directory[i];
            }
            std::cout << "\n0. Exit";

            std::cout << "\nType the number: ";
            std::cin >> user_response;
        }
        if (!user_response) {
            return;
        } else {
            table_index = user_response - 1;
        }
    }

    if (students[table_index].empty()) {
        std::cout << "\n-Students are empty.-";
        return;
    } else if (variants[table_index].empty()) {
        std::cout << "\n-Variants are empty.-";
        return;
    }

    std::string user_response;
    while (user_response != "1" && user_response != "2") {
        std::cout << "1. Print in console\n" << "2. Print in file\n"
                  << "Type the number (1/2): ";
        std::cin >> user_response;
    }

    switch (std::stoi(user_response)) {
        case 1:
            PrintResultConsole(testing_table, students, variants, table_index);
            break;
        case 2:
            PrintResultFile(testing_table, students, variants, directory, table_index);
    }
}
