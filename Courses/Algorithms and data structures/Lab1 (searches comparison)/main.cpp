#include "generate_table.h"
#include "linear_search.h"
#include "binary_search.h"
#include "exponential_search.h"
#include "constants.h"
#include "clock.h"

#include <iostream>
#include <vector>
#include <iomanip>

void TestLinearTable(const int &starts) {
    Clock clock;
    int target = 2 * WIDTH + 1;

    std::vector<std::vector<std::vector<int>>> data;
    for (int i = 1; i <= WIDTH; i *= 2) {
        auto table = OrdinaryTableGeneration(i);
        data.push_back(table);
    }

    std::vector<double> time_linear(14, 0), time_binary(14, 0), time_exponential(14, 0);
    for (int j = 0; j < starts; j++) {
        int ind = 0;
        for (int index = 0; index <= 13; ++index) {

            clock.start();
            LinearSearch(data[index], target);
            clock.finish();
            time_linear[ind] += clock.result();

            clock.start();
            BinarySearch(data[index], target);
            clock.finish();
            time_binary[ind] += clock.result();

            clock.start();
            ExponentialSearch(data[index], target);
            clock.finish();
            time_exponential[ind++] += clock.result();
        }
    }

    std::cout << "\nResults for Ordinary table\n";
    for (size_t index = 0; index < 14; ++index) {
        std::cout << std::fixed << std::setprecision(5) << time_linear[index] / starts << ' '
                  << time_binary[index] / starts << ' '
                  << time_exponential[index] / starts << "\n";
    }
}

void TestHyperbolicTable(const int &starts) {
    Clock clock;
    int target = 16 * WIDTH + 1;

    std::vector<std::vector<std::vector<int>>> data;
    for (int i = 1; i <= WIDTH; i *= 2) {
        auto table = HyperbolicTableGeneration(i);
        data.push_back(table);
    }

    std::vector<double> time_linear(14, 0), time_binary(14, 0), time_exponential(14, 0);
    for (int j = 0; j < starts; j++) {
        int ind = 0;
        for (int index = 0; index <= 13; ++index) {

            clock.start();
            LinearSearch(data[index], target);
            clock.finish();
            time_linear[ind] += clock.result();

            clock.start();
            BinarySearch(data[index], target);
            clock.finish();
            time_binary[ind] += clock.result();

            clock.start();
            ExponentialSearch(data[index], target);
            clock.finish();
            time_exponential[ind++] += clock.result();
        }
    }

    std::cout << "\nResults for Hyperbolic table\n";
    for (size_t index = 0; index < 14; ++index) {
        std::cout << std::fixed << std::setprecision(5) << time_linear[index] / starts << ' '
                  << time_binary[index] / starts << ' '
                  << time_exponential[index] / starts << "\n";
    }
}

int main() {
    Clock clock;
    int starts;
    std::cout << "Input the number of tests: ";
    std::cin >> starts;

    TestLinearTable(starts);
    TestHyperbolicTable(starts);

    return 0;
}
