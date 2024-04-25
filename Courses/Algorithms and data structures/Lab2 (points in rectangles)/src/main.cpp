#include <iostream>
#include <vector>

#include "Objects/Rectangle.h"
#include "AdditionalFiles/clock.h"

#include "Algorithms/brute_force.h"
#include "Algorithms/map_algorithm.h"

#include "../tests/tests.h"

int main() {

    RunOrdinaryTests(MapAlgorithm);
    RunBigTests(MapAlgorithm);
    RunRandomTests(MapAlgorithm);
    return 0;

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
    auto result = BruteForce(rectangles, points);
    clock.finish();
    for (auto i : result) {
        std::cout << i << ' ';
    }
    std::cout << clock;

    return 0;
}
