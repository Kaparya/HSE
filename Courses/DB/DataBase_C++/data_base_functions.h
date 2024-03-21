
int FindStudent(int &cur_id, std::vector<Student> v);

int FindVariant(int &cur_id, std::vector<Variant> v);

void
GiveVariants(std::vector<std::vector<StudentVariant>> &testing_table, const std::vector<std::vector<Student>> &students,
             const std::vector<std::vector<Variant>> &variants, const std::vector<std::string> &directory,
             int table_index);

void AddData(std::vector<std::vector<Student>> &students, std::vector<std::vector<Variant>> &variants,
             std::vector<std::vector<StudentVariant>> &testing_table,
             const std::vector<std::string> &students_table_name,
             const std::vector<std::string> &variants_table_name, const std::vector<std::string> &directory);

void DeleteData(std::vector<std::vector<Student>> &students, std::vector<std::vector<Variant>> &variants,
                std::vector<std::vector<StudentVariant>> &testing_table,
                const std::vector<std::string> &students_table_name,
                const std::vector<std::string> &variants_table_name, const std::vector<std::string> &directory);

void ChangeData(std::vector<std::vector<Student>> &students, std::vector<std::vector<Variant>> &variants,
                std::vector<std::vector<StudentVariant>> &testing_table,
                const std::vector<std::string> &students_table_name,
                const std::vector<std::string> &variants_table_name, const std::vector<std::string> &directory);

void PrintRow(std::vector<std::vector<Student>> &students, std::vector<std::vector<Variant>> &variants,
              std::vector<std::vector<StudentVariant>> &testing_table, const std::vector<std::string> &directory);

void OpenDataBase(std::vector<std::vector<Student>> &students, std::vector<std::vector<Variant>> &variants,
                  std::vector<std::vector<StudentVariant>> &testing_table,
                  std::vector<std::string> &students_table_name,
                  std::vector<std::string> &variants_table_name, std::vector<std::string> &directory);

void CloseDataBase(std::vector<std::vector<Student>> &students, std::vector<std::vector<Variant>> &variants,
                   std::vector<std::vector<StudentVariant>> &testing_table,
                   std::vector<std::string> &students_table_name,
                   std::vector<std::string> &variants_table_name, std::vector<std::string> &directory);
