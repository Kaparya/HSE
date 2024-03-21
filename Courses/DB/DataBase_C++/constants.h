

struct Student {
    int id;
    std::string name, surname, patronymic;
};

struct Variant {
    int id;
    std::string path;
};

struct StudentVariant {
    int student_id;
    int variant_id;
    int grade = -1;
};
