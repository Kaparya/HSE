#include <iostream>
#include <fstream>
#include <vector>
#include "constants.h"
#include "file_work.h"

extern std::vector<int> max_name_length;
extern std::vector<int> max_path_length;
extern std::vector<int> current_id_student;
extern std::vector<int> current_id_variant;

void ReadDataStudents(std::vector<std::vector<Student>> &students, const std::vector<std::string> &students_table_name,
                      int table_index) {
    std::ifstream fin;
    fin.open(students_table_name.back());
    if (fin.fail()) {
        std::cout << "\n\nWrong students file!";
        exit(0);
    }

    Student current;


    int &id = current_id_student[table_index];
    while (!fin.eof()) {
        current.id = id++;
        getline(fin, current.surname, ';');
        getline(fin, current.name, ';');
        getline(fin, current.patronymic);

        if (fin.eof()) {
            break;
        }

        while (current.patronymic.back() == '\n' || current.patronymic.back() == '\r') {
            current.patronymic.pop_back();
        }

        max_name_length.back() = std::max(max_name_length.back(),
                                          (static_cast<int>(current.surname.size() + current.name.size() +
                                                            current.patronymic.size())) /
                                          ((current.surname[0] >= 'a' && current.surname[0] <= 'z' ||
                                            current.surname[0] >= 'A' && current.surname[0] <= 'Z' ||
                                            current.surname[0] >= '0' && current.surname[0] <= '9') ? 1 : 2) + 3);

        bool fl = true;
        for (const auto &elem: students[table_index]) {
            if (elem.surname == current.surname && elem.name == current.name && elem.patronymic == current.patronymic) {
                fl = false;
                --id;
                break;
            }
        }

        if ((!current.surname.empty() || !current.name.empty() || !current.patronymic.empty()) && fl) {
            students[table_index].push_back(current);
        }
    }

    fin.close();
}

void ReadDataVariants(std::vector<std::vector<Variant>> &variants, const std::vector<std::string> &variants_table_name,
                      int table_index) {
    std::ifstream fin;
    fin.open(variants_table_name.back());
    if (fin.fail()) {
        std::cout << "\n\nWrong variants file!";
        exit(0);
    }

    Variant current;

    int &id = current_id_variant[table_index];
    while (!fin.eof()) {
        current.id = id++;
        getline(fin, current.path);
        while (current.path.back() == '\n' || current.path.back() == '\r') {
            current.path.pop_back();
        }

        max_path_length.back() = std::max(max_path_length.back(), static_cast<int>(current.path.size()) + 1);

        bool fl = true;
        for (const auto &elem: variants[table_index]) {
            if (elem.path == current.path) {
                fl = false;
                --id;
                break;
            }
        }

        if (!current.path.empty() && fl) {
            variants[table_index].push_back(current);
        }
    }

    fin.close();
}

void WriteTestingTable(const std::vector<std::vector<StudentVariant>> &testing_table,
                       const std::vector<std::string> &directory,
                       int table_index) {
    std::ofstream file_testing;
    file_testing.open(directory[table_index] + "/testing_table.txt");

    for (const auto &value: testing_table[table_index]) {
        file_testing << value.student_id << ' ' << value.variant_id << ' ' << value.grade << '\n';
    }

    file_testing.close();
}

void WriteDataStudents(const std::vector<std::vector<Student>> &students,
                       const std::vector<std::string> &students_table_name, int table_index) {
    std::ofstream fout;
    fout.open(students_table_name[table_index]);

    for (const auto &value: students[table_index]) {
        fout << value.surname << ';' << value.name << ';' << value.patronymic << '\n';
    }

    fout.close();
}

void WriteDataVariants(const std::vector<std::vector<Variant>> &variants,
                       const std::vector<std::string> &variants_table_name,
                       int table_index) {
    std::ofstream fout;
    fout.open(variants_table_name[table_index]);

    for (const auto &value: variants[table_index]) {
        fout << value.path << '\n';
    }

    fout.close();
}
