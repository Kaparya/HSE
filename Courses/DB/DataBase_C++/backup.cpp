#include <iostream>
#include <vector>
#include "constants.h"
#include "file_work.h"

void Backup(std::vector<std::string> &students_table_name, std::vector<std::string> &variants_table_name,
            std::vector<std::string> &directory, std::vector<std::vector<Student>> &students,
            std::vector<std::vector<Variant>> &variants, std::vector<std::vector<StudentVariant>> &testing_table,
            std::vector<int> &max_name_length_backup, std::vector<int> &max_path_length_backup,
            std::vector<int> &current_id_student_backup, std::vector<int> &current_id_variant_backup,
            std::vector<std::string> &students_table_name_backup, std::vector<std::string> &variants_table_name_backup,
            std::vector<std::string> &directory_backup, std::vector<std::vector<Student>> &students_backup,
            std::vector<std::vector<Variant>> &variants_backup,
            std::vector<std::vector<StudentVariant>> &testing_table_backup, std::vector<int> max_name_length,
            std::vector<int> max_path_length, std::vector<int> current_id_student,
            std::vector<int> current_id_variant) {
    students_table_name_backup = students_table_name;
    variants_table_name_backup = variants_table_name;
    directory_backup = directory;
    students_backup = students;
    variants_backup = variants;
    testing_table_backup = testing_table;
    max_name_length_backup = max_name_length;
    max_path_length_backup = max_path_length;
    current_id_student_backup = current_id_student;
    current_id_variant_backup = current_id_variant;

    std::cout << "\n\n+++++++++++++";
    std::cout << "\n+Backup done+";
    std::cout << "\n+++++++++++++";
}

void RevertBackup(std::vector<std::string> &students_table_name, std::vector<std::string> &variants_table_name,
                  std::vector<std::string> &directory, std::vector<std::vector<Student>> &students,
                  std::vector<std::vector<Variant>> &variants, std::vector<std::vector<StudentVariant>> &testing_table,
                  std::vector<int> &max_name_length_backup, std::vector<int> &max_path_length_backup,
                  std::vector<int> &current_id_student_backup, std::vector<int> &current_id_variant_backup,
                  std::vector<std::string> &students_table_name_backup,
                  std::vector<std::string> &variants_table_name_backup,
                  std::vector<std::string> &directory_backup, std::vector<std::vector<Student>> &students_backup,
                  std::vector<std::vector<Variant>> &variants_backup,
                  std::vector<std::vector<StudentVariant>> &testing_table_backup, std::vector<int> max_name_length,
                  std::vector<int> max_path_length, std::vector<int> current_id_student,
                  std::vector<int> current_id_variant) {
    if (directory_backup.empty()) {
        std::cout << "\n\n-----------------------------";
        std::cout << "\n-There is no previous backup-";
        std::cout << "\n-----------------------------";
        return;
    }
    students_table_name = students_table_name_backup;
    variants_table_name = variants_table_name_backup;
    directory = directory_backup;
    students = students_backup;
    variants = variants_backup;
    testing_table = testing_table_backup;
    max_name_length = max_path_length_backup;
    max_path_length = max_path_length_backup;
    current_id_student = current_id_student_backup;
    current_id_variant = current_id_variant_backup;

    for (int table_index = 0; table_index < directory.size(); ++table_index) {
        WriteDataStudents(students, students_table_name, table_index);
        WriteTestingTable(testing_table, directory, table_index);
        WriteDataVariants(variants, variants_table_name, table_index);
    }

    std::cout << "\n\n+++++++++++++++++";
    std::cout << "\n+Backup returned+";
    std::cout << "\n+++++++++++++++++";
}
