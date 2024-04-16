#include <iostream>
#include <memory>

// CORRECT
std::size_t string_length(const char* s) {
    std::size_t n = 0;
    while (*s != '\0') {
        ++s;
        ++n;
    }
    return n;
}

// Хорошее копирование, но странно, что возвращает указатель
// Она и так пишет результат в d, изменим тип для безопасности
void string_copy(char* d, const char* s) {
    char* p = d;
    while ((*p = *s) != '\0') {
        ++p;
        ++s;
    }
    *p = '\0';
}

// Тут тоже все норм, на первый взгляд
// Просто глубокое копирование
char* string_duplicate(const char* s) {
    std::size_t n = string_length(s);
    char* result = new char[n + 1];
    string_copy(result, s);
    return result;
}

// Тут у нас на вход два указателя, которые мы потом забываем
// А идеально было бы их чистить, потому что в результате все равно есть конкатенация этих строк
char* string_concatenate(const std::unique_ptr<char*>& first, const std::unique_ptr<char*>& second) {
    std::size_t first_len = string_length(*first);
    std::size_t second_len = string_length(*second);
    char* result = new char[first_len + second_len + 1]; // Выделение достаточной памяти
    string_copy(result, *first);
    string_copy(result + first_len, *second);
    return result;
}

int main() {
    std::unique_ptr<char*> first_string(new char*{"Hello, "});
    std::unique_ptr<char*> second_string(new char*{"World!"});

    char* s = string_concatenate(first_string, second_string);
    char* t = string_duplicate(s);
    std::cout << t << '\n';
    delete[] s;
    delete[] t;
}
