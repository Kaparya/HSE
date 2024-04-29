#include <iostream>
#include <iomanip>
#include <vector>

#include "constants.h"

#include "Objects/Rectangle.h"

#include "Algorithms/brute_force.h"
#include "Algorithms/map_algorithm.h"
#include "Algorithms/persistent_st.h"

#include "../tests/tests.h"

std::ofstream output;

int main() {

#ifdef TESTING

    RunOrdinaryTests(BruteForce);
    RunBigTests(BruteForce);
    RunRandomTests(BruteForce);

    RunOrdinaryTests(MapAlgorithm);
    RunBigTests(MapAlgorithm);
    RunRandomTests(MapAlgorithm);

    RunOrdinaryTests(PersistentSTAlgorithm);
    RunBigTests(PersistentSTAlgorithm);
    RunRandomTests(PersistentSTAlgorithm);

#endif
#ifdef TIME_SCORING

    output.open("../Results/output_search_comparison.txt");
    output << std::fixed << std::setprecision(10);
    std::cout << std::fixed << std::setprecision(10);

    for (int number_of_rectangles: {10, 1000}) {
        for (int number_of_points: {10, 1000, 1000000}) {
            std::cout << "\n============ N = " << number_of_rectangles << " M = " << number_of_points
                      << " ===========\n";
            std::cout << "----- Brute force algorithm -----\n";
            output << "\n============ N = " << number_of_rectangles << " M = " << number_of_points << " ===========\n";
            RunBigTests(BruteForce, number_of_rectangles, number_of_points);

            std::cout << "--- Map construction algorithm ---\n";
            RunBigTests(MapAlgorithm, number_of_rectangles, number_of_points);

            std::cout << "--- Persistent segment tree algorithm ---\n";
            RunBigTests(PersistentSTAlgorithm, number_of_rectangles, number_of_points);
        }
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

    // First algorithm - brute force
    Clock clock;
    clock.start();
    auto result = PersistentSTAlgorithm(rectangles, points);
    clock.finish();
    auto result_correct = BruteForce(rectangles, points);
    for (auto i : result_correct) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
    for (auto i : result) {
        std::cout << i << ' ';
    }
    std::cout << clock;

#endif

    return 0;
}
