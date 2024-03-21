
void InitDB(std::vector<std::string> &students_table_name, std::vector<std::string> &variants_table_name,
            std::vector<std::string> &directory);

void PrintResultConsole(std::vector<std::vector<StudentVariant>> &testing_table,
                        const std::vector<std::vector<Student>> &students,
                        const std::vector<std::vector<Variant>> &variants, int table_index);

void PrintResultFile(std::vector<std::vector<StudentVariant>> &testing_table,
                     const std::vector<std::vector<Student>> &students,
                     const std::vector<std::vector<Variant>> &variants, const std::vector<std::string> &directory,
                     int table_index);

void PrintResult(std::vector<std::vector<StudentVariant>> &testing_table,
                 const std::vector<std::vector<Student>> &students,
                 const std::vector<std::vector<Variant>> &variants, const std::vector<std::string> &directory);
