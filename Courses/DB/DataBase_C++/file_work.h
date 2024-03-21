
void ReadDataStudents(std::vector<std::vector<Student>> &students, const std::vector<std::string> &students_table_name,
                      int table_index);

void ReadDataVariants(std::vector<std::vector<Variant>> &variants, const std::vector<std::string> &variants_table_name,
                      int table_index);

void WriteTestingTable(const std::vector<std::vector<StudentVariant>> &testing_table,
                       const std::vector<std::string> &directory,
                       int table_index);

void WriteDataStudents(const std::vector<std::vector<Student>> &students,
                       const std::vector<std::string> &students_table_name, int table_index);

void WriteDataVariants(const std::vector<std::vector<Variant>> &variants,
                       const std::vector<std::string> &variants_table_name,
                       int table_index);
