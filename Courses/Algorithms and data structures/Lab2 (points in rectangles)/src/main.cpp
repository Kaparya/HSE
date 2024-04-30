#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include "constants.h"

#include "AdditionalFiles/clock.h"

#include "Objects/Rectangle.h"

#include "Algorithms/brute_force.h"
#include "Algorithms/map_algorithm.h"
#include "Algorithms/persistent_st.h"

#include "../tests/tests.h"

std::ofstream output;

int main() {

    // All defines in file constants.h
#ifdef TESTING

    RunOrdinaryTests(BruteForce);
    RunBigTests(BruteForce);
    RunRandomTests(BruteForce);
    std::cout << "Brute: All tests PASSED!\n";

    RunOrdinaryTests(MapAlgorithm);
    RunBigTests(MapAlgorithm);
    RunRandomTests(MapAlgorithm);
    std::cout << "Map:   All tests PASSED!\n";

    RunOrdinaryTests(PersistentSTAlgorithm);
    RunBigTests(PersistentSTAlgorithm);
    RunRandomTests(PersistentSTAlgorithm);
    std::cout << "Tree:  All tests PASSED!\n";

#endif
#ifdef TIME_SCORING

    int number_of_points = 10;
    output.open("../Description/Results/graph_building_total1_" + std::to_string(number_of_points) + ".txt");
    output << std::fixed << std::setprecision(10);
    std::cout << std::fixed << std::setprecision(10);

    for (int number_of_rectangles = 10; number_of_rectangles <= 4010; number_of_rectangles += 500) {
        //for (int number_of_points: {10, 1000, 1000000}) {
        std::cout << "\n============ N = " << number_of_rectangles << " M = " << number_of_points
                  << " ===========\n";
        std::cout << "----- Brute force algorithm -----\n";
        output << number_of_rectangles << "\n";
        RunBigTests(BruteForce, number_of_rectangles, number_of_points);

        std::cout << "--- Map construction algorithm ---\n";
        RunBigTests(MapAlgorithm, number_of_rectangles, number_of_points);

        std::cout << "--- Persistent segment tree algorithm ---\n";
        RunBigTests(PersistentSTAlgorithm, number_of_rectangles, number_of_points);
        //}
    }

    output.close();

#endif
#ifdef MANUAL_INPUT

    // Rectangles reading
    int number_of_rects;
    std::cin >> number_of_rects;
    std::vector<Rectangle> rectangles(number_of_rects);
    for (size_t index = 0; index < number_of_rects; ++index) {

        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;

        Rectangle &new_rect = rectangles[index];
        new_rect.l_d = {std::min(x1, x2), std::min(y1, y2)};
        new_rect.r_u = {std::max(x1, x2), std::max(y1, y2)};
    }

    // Points reading
    int number_of_points;
    std::cin >> number_of_points;
    std::vector<Point> points(number_of_points);
    for (size_t index = 0; index < number_of_points; ++index) {

        int x, y;
        std::cin >> x >> y;

        points[index] = {x, y};
    }

    auto result_brute = BruteForce(rectangles, points);
    auto result_map = MapAlgorithm(rectangles, points);
    auto result_tree = PersistentSTAlgorithm(rectangles, points);
    std::cout << "Brute: ";
    for (auto i : result_brute) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
    std::cout << "Map:   ";
    for (auto i : result_map) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
    std::cout << "Tree:  ";
    for (auto i : result_tree) {
        std::cout << i << ' ';
    }

#endif

    return 0;
}
