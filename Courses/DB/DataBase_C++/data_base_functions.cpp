#include <iostream>
#include <vector>
#include "constants.h"
#include "file_work.h"
#include "prints.h"
#include "data_base_functions.h"


extern std::vector<int> max_name_length;
extern std::vector<int> max_path_length;
extern std::vector<int> current_id_student;
extern std::vector<int> current_id_variant;

int FindStudent(int &cur_id, std::vector<Student> v) {
    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i].id == cur_id) {
            return i;
        }
    }
    return 0;
}

int FindVariant(int &cur_id, std::vector<Variant> v) {
    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i].id == cur_id) {
            return i;
        }
    }
    return 0;
}

void GiveVariants(std::vector<std::vector<StudentVariant>> &testing_table,
                  const std::vector<std::vector<Student>> &students, const std::vector<std::vector<Variant>> &variants,
                  const std::vector<std::string> &directory, int table_index) {

    if (table_index == -1) {
        table_index = 0;
        if (students.size() > 1) {
            table_index = 0;
            std::cout << "\nChoose data base:";
            int user_response = -1;
            while (user_response < 0 || user_response > students.size()) {
                for (size_t i = 0; i < students.size(); i++) {
                    std::cout << '\n' << i + 1 << ". Give variants in " << directory[i];
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
    }

    if (variants[table_index].empty()) {
        return;
    }

    while (testing_table[table_index].size() < students[table_index].size()) {
        testing_table[table_index].push_back({});
    }

    for (size_t i = 0; i < students[table_index].size(); ++i) {
        testing_table[table_index][i] = {students[table_index][i].id,
                                         variants[table_index][rand() % variants[table_index].size()].id};
    }

    std::cout << "\n+Variants were given correctly+\n";

    WriteTestingTable(testing_table, directory, table_index);
}

void AddStudent(std::vector<std::vector<Student>> &students, std::vector<std::vector<Variant>> &variants,
                const std::vector<std::string> &students_table_name,
                std::vector<std::vector<StudentVariant>> &testing_table,
                const std::vector<std::string> &directory, int table_index) {
    Student new_student;
    getline(std::cin, new_student.surname);
    std::cout << "\n\nInput surname of the student: ";
    getline(std::cin, new_student.surname);
    std::cout << "Input name of the student: ";
    getline(std::cin, new_student.name);
    std::cout << "Input patronymic of the student: ";
    getline(std::cin, new_student.patronymic);
    new_student.id = current_id_student[table_index]++;

    for (const auto &elem: students[table_index]) {
        if (elem.surname == new_student.surname && elem.name == new_student.name &&
            elem.patronymic == new_student.patronymic) {
            std::cout << "\n-Student already in table-\n";
            return;
        }
    }

    max_name_length[table_index] = std::max(max_name_length[table_index],
                                            static_cast<int>(new_student.surname.size() + new_student.name.size() +
                                                             new_student.patronymic.size() + 2));

    students[table_index].push_back(new_student);
    if (!variants[table_index].empty()) {
        testing_table[table_index].push_back(
                {.student_id = new_student.id, .variant_id = rand() % static_cast<int>(variants[table_index].size())});
    }
    std::cout << "\n+Added correctly+\n";

    WriteDataStudents(students, students_table_name, table_index);
    WriteTestingTable(testing_table, directory, table_index);
}

void AddVariant(std::vector<std::vector<Student>> &students, std::vector<std::vector<Variant>> &variants,
                std::vector<std::vector<StudentVariant>> &testing_table,
                const std::vector<std::string> &students_table_name,
                const std::vector<std::string> &variants_table_name, const std::vector<std::string> &directory,
                int table_index) {
    Variant new_variant;
    getline(std::cin, new_variant.path);
    std::cout << "\n\nInput path to the variant: ";
    getline(std::cin, new_variant.path);
    new_variant.id = current_id_variant[table_index]++;

    for (const auto &elem: variants[table_index]) {
        if (elem.path == new_variant.path) {
            std::cout << "\n-Variant already in table-\n";
            return;
        }
    }

    max_path_length[table_index] = std::max(max_path_length[table_index], static_cast<int>(new_variant.path.size()));

    variants[table_index].push_back(new_variant);
    std::cout << "\n+Added correctly+\n";

    std::string user_response;

    while (user_response != "1" && user_response != "2") {
        std::cout << "\nRecalculate all variants? (1 - YES / 2 - NO): ";
        std::cin >> user_response;
    }

    if (user_response == "1") {
        GiveVariants(testing_table, students, variants, directory, table_index);
        WriteTestingTable(testing_table, directory, table_index);
    }

    WriteDataVariants(variants, variants_table_name, table_index);
}

void AddData(std::vector<std::vector<Student>> &students, std::vector<std::vector<Variant>> &variants,
             std::vector<std::vector<StudentVariant>> &testing_table,
             const std::vector<std::string> &students_table_name, const std::vector<std::string> &variants_table_name,
             const std::vector<std::string> &directory) {

    std::cout << "\n-Add data-";

    int table_index = 0;
    if (students.size() > 1) {
        std::cout << "\nChoose data base:";
        int user_response = -1;
        while (user_response < 0 || user_response > students.size()) {
            for (size_t i = 0; i < students.size(); i++) {
                std::cout << '\n' << i + 1 << ". Add to " << directory[i];
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

    std::string user_response;
    while (user_response != "0" && user_response != "1" && user_response != "2") {
        std::cout << "\n1. Add to students.";
        std::cout << "\n2. Add to variants.";
        std::cout << "\n0. Exit";

        std::cout << "\nType the number(1/2/0): ";
        std::cin >> user_response;
    }

    switch (stoi(user_response)) {
        case 0:
            return;
        case 1:
            AddStudent(students, variants, students_table_name, testing_table, directory, table_index);
            break;
        case 2:
            AddVariant(students, variants, testing_table, students_table_name, variants_table_name, directory,
                       table_index);
    }
}

void DeleteStudent(std::vector<std::vector<Student>> &students, const std::vector<std::string> &students_table_name,
                   std::vector<std::vector<StudentVariant>> &testing_table, const std::vector<std::string> &directory,
                   int table_index) {
    Student new_student;
    getline(std::cin, new_student.surname);
    std::cout << "\n\nInput surname of the student: ";
    getline(std::cin, new_student.surname);
    std::cout << "Input name of the student: ";
    getline(std::cin, new_student.name);
    std::cout << "Input patronymic of the student: ";
    getline(std::cin, new_student.patronymic);

    int new_max_name_length = 10;
    int erase_i = -1;
    for (size_t i = 0; i < students[table_index].size(); ++i) {
        if (students[table_index][i].surname == new_student.surname &&
            students[table_index][i].name == new_student.name &&
            students[table_index][i].patronymic == new_student.patronymic && erase_i == -1) {
            erase_i = i;
        } else {
            new_max_name_length = std::max(new_max_name_length,
                                           (static_cast<int>(students[table_index][i].surname.size() +
                                                             students[table_index][i].name.size() +
                                                             students[table_index][i].patronymic.size())) /
                                           ((students[table_index][i].surname[0] >= 'a' &&
                                             students[table_index][i].surname[0] <= 'z' ||
                                             students[table_index][i].surname[0] >= 'A' &&
                                             students[table_index][i].surname[0] <= 'Z' ||
                                             students[table_index][i].surname[0] >= '0' &&
                                             students[table_index][i].surname[0] <= '9') ? 1 : 2) +
                                           3);
        }
    }
    if (erase_i != -1) {
        if (max_name_length[table_index] ==
            (static_cast<int>(students[table_index][erase_i].surname.size() +
                              students[table_index][erase_i].name.size() +
                              students[table_index][erase_i].patronymic.size())) /
            ((students[table_index][erase_i].surname[0] >= 'a' && students[table_index][erase_i].surname[0] <= 'z' ||
              students[table_index][erase_i].surname[0] >= 'A' && students[table_index][erase_i].surname[0] <= 'Z' ||
              students[table_index][erase_i].surname[0] >= '0' && students[table_index][erase_i].surname[0] <= '9') ? 1
                                                                                                                    : 2) +
            3) {
            max_name_length[table_index] = new_max_name_length;
        }
        students[table_index].erase(students[table_index].begin() + erase_i);
        if (!testing_table[table_index].empty()) {
            testing_table[table_index].erase(testing_table[table_index].begin() + erase_i);
        }
        std::cout << "\n+Deleted correctly+\n";

        WriteTestingTable(testing_table, directory, table_index);
        WriteDataStudents(students, students_table_name, table_index);
        return;
    }

    std::cout << "\n-Incorrect student-\n";
}

void DeleteVariant(std::vector<std::vector<Student>> &students, std::vector<std::vector<Variant>> &variants,
                   std::vector<std::vector<StudentVariant>> &testing_table,
                   const std::vector<std::string> &students_table_name,
                   const std::vector<std::string> &variants_table_name, const std::vector<std::string> &directory,
                   int table_index) {
    Variant new_variant;
    getline(std::cin, new_variant.path);
    std::cout << "\n\nInput path to the variant: ";
    getline(std::cin, new_variant.path);

    int erase_i = -1;
    int new_max_path_length = 13;
    for (auto i = 0; i < variants[table_index].size(); i++) {
        if (erase_i != -1) {
            --variants[table_index][i].id;
        }
        if (variants[table_index][i].path == new_variant.path && erase_i == -1) {
            erase_i = i;
        } else {
            new_max_path_length = std::max(new_max_path_length,
                                           static_cast<int>(variants[table_index][i].path.size()) + 1);
        }
    }

    if (erase_i != -1) {
        for (auto &tests: testing_table[table_index]) {
            if (tests.variant_id == variants[table_index][erase_i].id) {
                tests.variant_id = rand() % (variants[table_index].size() - 1);
            }
        }
        if (max_path_length[table_index] == static_cast<int>(variants[table_index][erase_i].path.size()) + 1) {
            max_path_length[table_index] = new_max_path_length;
        }
        variants[table_index].erase(variants[table_index].begin() + erase_i);

        std::cout << "\n+Deleted correctly+\n";

        WriteTestingTable(testing_table, directory, table_index);
        WriteDataVariants(variants, variants_table_name, table_index);
        return;
    }

    std::cout << "\n-Incorrect variant path-\n";
}

void DeleteData(std::vector<std::vector<Student>> &students, std::vector<std::vector<Variant>> &variants,
                std::vector<std::vector<StudentVariant>> &testing_table,
                const std::vector<std::string> &students_table_name,
                const std::vector<std::string> &variants_table_name, const std::vector<std::string> &directory) {
    std::cout << "\n-Delete data-";

    int table_index = 0;
    if (students.size() > 1) {
        std::cout << "\nChoose data base:";
        int user_response = -1;
        while (user_response < 0 || user_response > students.size()) {
            for (size_t i = 0; i < students.size(); i++) {
                std::cout << '\n' << i + 1 << ". Delete from " << directory[i];
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

    std::string user_response;
    while (user_response != "0" && user_response != "1" && user_response != "2") {
        std::cout << "\n1. Delete student.";
        std::cout << "\n2. Delete variant.";
        std::cout << "\n0. Exit";

        std::cout << "\nType the number(1/2/0): ";
        std::cin >> user_response;
    }

    switch (stoi(user_response)) {
        case 0:
            return;
        case 1:
            DeleteStudent(students, students_table_name, testing_table,
                          directory, table_index);
            break;
        case 2:
            DeleteVariant(students, variants, testing_table,
                          students_table_name, variants_table_name, directory,
                          table_index);
    }
}

void ChangeStudent(std::vector<std::vector<Student>> &students, const std::vector<std::string> &students_table_name,
                   int table_index) {
    Student old_student;
    getline(std::cin, old_student.surname);
    std::cout << "\n\nInput OLD surname of the student: ";
    getline(std::cin, old_student.surname);
    std::cout << "Input OLD name of the student: ";
    getline(std::cin, old_student.name);
    std::cout << "Input OLD patronymic of the student: ";
    getline(std::cin, old_student.patronymic);


    int new_max_name_length = 10;
    int erase_i = -1;
    for (size_t i = 0; i < students[table_index].size(); ++i) {
        if (students[table_index][i].surname == old_student.surname &&
            students[table_index][i].name == old_student.name &&
            students[table_index][i].patronymic == old_student.patronymic && erase_i == -1) {
            erase_i = i;
        } else {
            new_max_name_length = std::max(new_max_name_length,
                                           (static_cast<int>(students[table_index][i].surname.size() +
                                                             students[table_index][i].name.size() +
                                                             students[table_index][i].patronymic.size())) /
                                           ((students[table_index][i].surname[0] >= 'a' &&
                                             students[table_index][i].surname[0] <= 'z' ||
                                             students[table_index][i].surname[0] >= 'A' &&
                                             students[table_index][i].surname[0] <= 'Z' ||
                                             students[table_index][i].surname[0] >= '0' &&
                                             students[table_index][i].surname[0] <= '9')
                                            ? 1 : 2) + 3);
        }
    }

    if (erase_i != -1) {

        Student new_student;
        std::cout << "\n\nInput NEW surname of the student: ";
        getline(std::cin, new_student.surname);
        std::cout << "Input NEW name of the student: ";
        getline(std::cin, new_student.name);
        std::cout << "Input NEW patronymic of the student: ";
        getline(std::cin, new_student.patronymic);

        for (const auto &elem: students[table_index]) {
            if (elem.surname == new_student.surname && elem.name == new_student.name &&
                elem.patronymic == new_student.patronymic) {
                std::cout << "\n-Student already in table-\n";
                return;
            }
        }

        new_max_name_length = std::max(new_max_name_length,
                                       (static_cast<int>(new_student.surname.size() + new_student.name.size() +
                                                         new_student.patronymic.size())) /
                                       ((new_student.surname[0] >= 'a' && new_student.surname[0] <= 'z' ||
                                         new_student.surname[0] >= 'A' && new_student.surname[0] <= 'Z' ||
                                         new_student.surname[0] >= '0' && new_student.surname[0] <= '9') ? 1 : 2) + 3);

        if (max_name_length[table_index] ==
            (static_cast<int>(students[table_index][erase_i].surname.size() +
                              students[table_index][erase_i].name.size() +
                              students[table_index][erase_i].patronymic.size())) /
            ((students[table_index][erase_i].surname[0] >= 'a' && students[table_index][erase_i].surname[0] <= 'z' ||
              students[table_index][erase_i].surname[0] >= 'A' && students[table_index][erase_i].surname[0] <= 'Z' ||
              students[table_index][erase_i].surname[0] >= '0' && students[table_index][erase_i].surname[0] <= '9') ? 1
                                                                                                                    : 2) +
            3) {
            max_name_length[table_index] = new_max_name_length;
        }


        new_student.id = students[table_index][erase_i].id;
        students[table_index][erase_i] = new_student;
        std::cout << "\n+Changed correctly+\n";

        WriteDataStudents(students, students_table_name, table_index);
        return;
    }

    std::cout << "\n-Incorrect student-\n";
}

void ChangeVariant(std::vector<std::vector<Variant>> &variants, const std::vector<std::string> &variants_table_name,
                   int table_index) {
    Variant old_variant;
    getline(std::cin, old_variant.path);
    std::cout << "\n\nInput path to OLD variant: ";
    getline(std::cin, old_variant.path);

    int erase_i = -1;
    int new_max_path_length = 13;
    for (auto i = 0; i < variants[table_index].size(); i++) {
        if (variants[table_index][i].path == old_variant.path && erase_i == -1) {
            erase_i = i;
        } else {
            new_max_path_length = std::max(new_max_path_length,
                                           static_cast<int>(variants[table_index][i].path.size()) + 1);
        }
    }

    if (erase_i != -1) {
        Variant new_variant;
        std::cout << "\n\nInput path to NEW variant: ";
        getline(std::cin, new_variant.path);

        for (const auto &elem: variants[table_index]) {
            if (elem.path == new_variant.path) {
                std::cout << "\n-Variant already in table-\n";
                return;
            }
        }

        new_max_path_length = std::max(new_max_path_length, static_cast<int>(new_variant.path.size()) + 1);

        if (max_path_length[table_index] == static_cast<int>(variants[table_index][erase_i].path.size()) + 1) {
            max_path_length[table_index] = new_max_path_length;
        }

        new_variant.id = variants[table_index][erase_i].id;
        variants[table_index][erase_i] = new_variant;
        std::cout << "\n+Changed correctly+\n";

        WriteDataVariants(variants, variants_table_name, table_index);
        return;
    }

    std::cout << "\n-Incorrect variant path-\n";
}

void ChangeStudentsVariant(std::vector<std::vector<Student>> &students, std::vector<std::vector<Variant>> &variants,
                           std::vector<std::vector<StudentVariant>> &testing_table,
                           const std::vector<std::string> &students_table_name,
                           const std::vector<std::string> &variants_table_name,
                           const std::vector<std::string> &directory,
                           int table_index) {
    Student new_student;
    getline(std::cin, new_student.surname);
    std::cout << "\n\nInput data of the student whose variant you want to change";
    std::cout << "\nInput surname of the student: ";
    getline(std::cin, new_student.surname);
    std::cout << "Input name of the student: ";
    getline(std::cin, new_student.name);
    std::cout << "Input patronymic of the student: ";
    getline(std::cin, new_student.patronymic);

    int student_ind = -1;
    for (size_t i = 0; i < students[table_index].size(); ++i) {
        if (students[table_index][i].surname == new_student.surname &&
            students[table_index][i].name == new_student.name &&
            students[table_index][i].patronymic == new_student.patronymic) {
            student_ind = i;
            new_student.id = students[table_index][i].id;
            break;
        }
    }

    if (student_ind != -1) {

        Variant new_variant;
        std::cout << "\nInput path to new variant of this student: ";
        getline(std::cin, new_variant.path);

        int variant_ind = -1;

        for (size_t i = 0; i < variants[table_index].size(); ++i) {
            if (variants[table_index][i].path == new_variant.path) {
                variant_ind = i;
                new_variant.id = variants[table_index][i].id;
                break;
            }
        }
        if (variant_ind == -1) {
            std::cout << "\n-Incorrect path-\n";
            return;
        }

        testing_table[table_index][student_ind].variant_id = new_variant.id;

        std::cout << "\n+Student's variant was changed correctly+\n";

        WriteTestingTable(testing_table, directory, table_index);
        return;
    }

    std::cout << "\n-Incorrect student-\n";
}

void ChangeGrade(std::vector<std::vector<Student>> &students, std::vector<std::vector<StudentVariant>> &testing_table,
                 const std::vector<std::string> &directory, int table_index) {
    Student new_student;
    getline(std::cin, new_student.surname);
    std::cout << "\n\nInput data of the student whose grade you want to change";
    std::cout << "\nInput surname of the student: ";
    getline(std::cin, new_student.surname);
    std::cout << "Input name of the student: ";
    getline(std::cin, new_student.name);
    std::cout << "Input patronymic of the student: ";
    getline(std::cin, new_student.patronymic);

    int student_ind = -1;
    for (size_t i = 0; i < students[table_index].size(); ++i) {
        if (students[table_index][i].surname == new_student.surname &&
            students[table_index][i].name == new_student.name &&
            students[table_index][i].patronymic == new_student.patronymic) {
            student_ind = i;
            new_student.id = students[table_index][i].id;
            break;
        }
    }

    if (student_ind != -1) {

        int grade = -1;
        while (grade < 0 || grade > 10) {
            std::cout << "\nInput student's grade (0 - 10): ";
            std::cin >> grade;
        }


        for (auto &value: testing_table[table_index]) {
            if (value.student_id == new_student.id) {
                value.grade = grade;
                break;
            }
        }

        std::cout << "\n+Student's grade was changed correctly+\n";

        WriteTestingTable(testing_table, directory, table_index);
        return;
    }

    std::cout << "\n-Incorrect student-\n";
}

void ChangeData(std::vector<std::vector<Student>> &students, std::vector<std::vector<Variant>> &variants,
                std::vector<std::vector<StudentVariant>> &testing_table,
                const std::vector<std::string> &students_table_name,
                const std::vector<std::string> &variants_table_name, const std::vector<std::string> &directory) {
    std::cout << "\n-Change data-";

    int table_index = 0;
    if (students.size() > 1) {
        std::cout << "\nChoose data base:";
        int user_response = -1;
        while (user_response < 0 || user_response > students.size()) {
            for (size_t i = 0; i < students.size(); i++) {
                std::cout << '\n' << i + 1 << ". Change in " << directory[i];
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

    std::string user_response;
    while (user_response != "0" && user_response != "1" && user_response != "2" && user_response != "3" &&
           user_response != "4") {
        std::cout << "\n1. Change student.";
        std::cout << "\n2. Change variant path.";
        std::cout << "\n3. Change current student variant.";
        std::cout << "\n4. Change grade.";
        std::cout << "\n0. Exit";

        std::cout << "\nType the number(1/2/3/4/0): ";
        std::cin >> user_response;
    }

    switch (stoi(user_response)) {
        case 0:
            return;
        case 1:
            ChangeStudent(students, students_table_name, table_index);
            break;
        case 2:
            ChangeVariant(variants, variants_table_name, table_index);
            break;
        case 3:
            ChangeStudentsVariant(students, variants, testing_table, students_table_name,
                                  variants_table_name, directory, table_index);
            break;
        case 4:
            ChangeGrade(students, testing_table, directory, table_index);
    }
}

void PrintRow(std::vector<std::vector<Student>> &students, std::vector<std::vector<Variant>> &variants,
              std::vector<std::vector<StudentVariant>> &testing_table, const std::vector<std::string> &directory) {
    std::cout << "\n-Print row-";

    int table_index = 0;
    if (students.size() > 1) {
        std::cout << "\nChoose data base:";
        int user_response = -1;
        while (user_response < 0 || user_response > students.size()) {
            for (size_t i = 0; i < students.size(); i++) {
                std::cout << '\n' << i + 1 << ". Add to " << directory[i];
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

    std::string user_response;
    while (user_response != "0" && user_response != "1") {
        std::cout << "\n1. Print row from testing_table.";
        std::cout << "\n0. Exit";

        std::cout << "\nType the number(1/0): ";
        std::cin >> user_response;
    }

    switch (stoi(user_response)) {
        case 0:
            return;
        case 1: {

            Student new_student;
            std::cout << "\n\nInput the name of the student, whose data you want to know";
            getline(std::cin, new_student.surname);
            std::cout << "\nInput surname of the student: ";
            getline(std::cin, new_student.surname);
            std::cout << "Input name of the student: ";
            getline(std::cin, new_student.name);
            std::cout << "Input patronymic of the student: ";
            getline(std::cin, new_student.patronymic);

            for (size_t i = 0; i < students[table_index].size(); ++i) {
                Student &elem = students[table_index][i];
                if (elem.surname == new_student.surname && elem.name == new_student.name &&
                    elem.patronymic == new_student.patronymic && !testing_table[table_index].empty()) {
                    std::cout << '\n' << "----------------------------------------------";
                    std::cout << '\n' << elem.surname << ' ' << elem.name << ' ' << elem.patronymic << ' ';
                    int index = FindVariant(testing_table[table_index][i].variant_id, variants[table_index]);
                    std::cout << variants[table_index][index].path;
                    if (testing_table[table_index][i].grade != -1) {
                        std::cout << ' ' << testing_table[table_index][i].grade;
                    }
                    std::cout << "\n----------------------------------------------\n";
                    return;
                }
            }

        }
    }

    std::cout << "\n-Incorrect student-\n";
}

void OpenDataBase(std::vector<std::vector<Student>> &students, std::vector<std::vector<Variant>> &variants,
                  std::vector<std::vector<StudentVariant>> &testing_table,
                  std::vector<std::string> &students_table_name,
                  std::vector<std::string> &variants_table_name, std::vector<std::string> &directory) {

    testing_table.push_back({});
    students.push_back({});
    variants.push_back({});

    InitDB(students_table_name, variants_table_name, directory);

    ReadDataStudents(students, students_table_name, directory.size() - 1);
    ReadDataVariants(variants, variants_table_name, directory.size() - 1);

    GiveVariants(testing_table, students, variants, directory, directory.size() - 1);
}

void CloseDataBase(std::vector<std::vector<Student>> &students, std::vector<std::vector<Variant>> &variants,
                   std::vector<std::vector<StudentVariant>> &testing_table,
                   std::vector<std::string> &students_table_name,
                   std::vector<std::string> &variants_table_name, std::vector<std::string> &directory) {

    std::string db_name;
    std::cout << "\n\n-Close data base-";
    std::cout << "\nInput the name of the data base you want to close: ";
    std::cin >> db_name;

    int erase_i = -1;
    for (size_t i = 0; i < directory.size(); i++) {
        if (directory[i] == db_name) {
            erase_i = i;
            break;
        }
    }

    if (erase_i == -1) {
        std::cout << "\n\n-Incorrect database name-";
        return;
    }

    for (size_t i = erase_i; i + 1 < directory.size(); ++i) {
        std::swap(students[i], students[i + 1]);
        std::swap(variants[i], variants[i + 1]);
        std::swap(testing_table[i], testing_table[i + 1]);
        std::swap(students_table_name[i], students_table_name[i + 1]);
        std::swap(variants_table_name[i], variants_table_name[i + 1]);
        std::swap(directory[i], directory[i + 1]);
        std::swap(current_id_student[i], current_id_student[i + 1]);
        std::swap(current_id_variant[i], current_id_variant[i + 1]);
        std::swap(max_name_length[i], max_name_length[i + 1]);
        std::swap(max_path_length[i], max_path_length[i + 1]);
    }
    students.pop_back();
    variants.pop_back();
    testing_table.pop_back();
    students_table_name.pop_back();
    variants_table_name.pop_back();
    directory.pop_back();
    current_id_student.pop_back();
    current_id_variant.pop_back();
    max_name_length.pop_back();
    max_path_length.pop_back();

    std::cout << "\n+" << db_name << " closed correctly+";
}
