#include <iostream>

std::size_t string_length(const char* s) {
    std::size_t n = 0;
    while (*s != '\0') {
        ++s;
        ++n;
    }
    return n;
}

char* string_copy(char* d, const char* s) {
    char* p = d;
    while ((*p = *s) != '\0') {
        ++p;
        ++s;
    }
    *p = '\0';
    return d;
}

char* string_duplicate(const char* s) {
    std::size_t n = string_length(s);
    char* result = new char[n + 1];
    string_copy(result, s);
    return result;
}

char* string_concatenate(const char* first, const char* second) {
    std::size_t first_len = string_length(first);
    std::size_t second_len = string_length(second);
    char* result = new char[first_len + second_len + 1];
    string_copy(result, first);
    string_copy(result + first_len, second);
    return result;
}

int main() {
    char* s = string_concatenate("Hello, ", "World!");
    char* t = string_duplicate(s);
    std::cout << t << '\n';
    delete[] s;
    delete[] t;
}
