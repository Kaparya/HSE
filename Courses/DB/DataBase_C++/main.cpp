#include <iostream>
#include <string>
#include <time.h>
#include "constants.h"
#include "file_work.h"
#include "prints.h"
#include "data_base_functions.h"
#include "backup.h"

std::vector<int> max_name_length;
std::vector<int> max_path_length;
std::vector<int> current_id_student;
std::vector<int> current_id_variant;

int AskAction() {

    std::string user_response;
    while (user_response != "0" && user_response != "1" && user_response != "2"
           && user_response != "3" && user_response != "4" && user_response != "5"
           && user_response != "6" && user_response != "7" && user_response != "8"
           && user_response != "9" && user_response != "10") {

        std::cout << "\n\n-Choose what to do-";
        std::cout << "\n1. Add something";
        std::cout << "\n2. Delete something";
        std::cout << "\n3. Change something";
        std::cout << "\n4. Make testing table";
        std::cout << "\n5. Print testing table";
        std::cout << "\n6. Print table row";

        std::cout << "\n\n7. Open one more data base";
        std::cout << "\n8. Close opened data base";

        std::cout << "\n\n9. Make backup";
        std::cout << "\n10. Revert backup\n";

        std::cout << "\n0. Exit";
        std::cout << "\nInput the number (0/1/2/3/4/5/6/7/8/9/10): ";

        std::cin >> user_response;
    }

    return std::stoi(user_response);
}

int main() {
    srand(time(nullptr));
    rand();

    std::vector<int> max_name_length_backup;
    std::vector<int> max_path_length_backup;
    std::vector<int> current_id_student_backup;
    std::vector<int> current_id_variant_backup;
    std::vector<std::string> students_table_name_backup, variants_table_name_backup, directory_backup;
    std::vector<std::vector<Student>> students_backup;
    std::vector<std::vector<Variant>> variants_backup;
    std::vector<std::vector<StudentVariant>> testing_table_backup;

    std::vector<std::string> students_table_name, variants_table_name, directory;
    std::vector<std::vector<Student>> students;
    std::vector<std::vector<Variant>> variants;
    std::vector<std::vector<StudentVariant>> testing_table;
    OpenDataBase(students, variants, testing_table, students_table_name, variants_table_name, directory);

    int user_response = -1;
    while (user_response != 0) {

        user_response = AskAction();

        switch (user_response) {
            case 0:
                break;
            case 1:
                AddData(students, variants, testing_table, students_table_name, variants_table_name, directory);
                break;
            case 2:
                DeleteData(students, variants, testing_table, students_table_name, variants_table_name, directory);
                break;
            case 3:
                ChangeData(students, variants, testing_table, students_table_name, variants_table_name, directory);
                break;
            case 4:
                GiveVariants(testing_table, students, variants, directory, -1);
                break;
            case 5:
                PrintResult(testing_table, students, variants, directory);
                break;
            case 6:
                PrintRow(students, variants, testing_table, directory);
                break;
            case 7:
                OpenDataBase(students, variants, testing_table, students_table_name, variants_table_name, directory);
                break;
            case 8:
                CloseDataBase(students, variants, testing_table, students_table_name, variants_table_name, directory);
                break;
            case 9:
                Backup(students_table_name, variants_table_name, directory, students, variants, testing_table,
                       max_name_length_backup, max_path_length_backup, current_id_student_backup,
                       current_id_variant_backup, students_table_name_backup, variants_table_name_backup,
                       directory_backup, students_backup, variants_backup, testing_table_backup, max_name_length,
                       max_path_length, current_id_student, current_id_variant);
                break;
            case 10:
                RevertBackup(students_table_name, variants_table_name, directory, students, variants, testing_table,
                             max_name_length_backup, max_path_length_backup, current_id_student_backup,
                             current_id_variant_backup, students_table_name_backup, variants_table_name_backup,
                             directory_backup, students_backup, variants_backup, testing_table_backup, max_name_length,
                             max_path_length, current_id_student, current_id_variant);
        }

        if (directory.empty()) {
            std::cout << "\n\nAll data bases are closed => EXIT";
            return 0;
        }
    }

    return 0;
}
