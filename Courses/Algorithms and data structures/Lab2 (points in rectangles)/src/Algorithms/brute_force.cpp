#include "brute_force.h"
#include "../AdditionalFiles/clock.h"

#include "../constants.h"

int flag = 0;

std::vector<int> BruteForce(const std::vector<Rectangle> &rectangles,
                            const std::vector<Point> &points) {
    Clock clock;
    clock.start();
    std::vector<int> result(points.size(), 0);
    clock.finish();
#ifdef TIME_SCORING
    if (flag % 4 == 0) {
        std::cout << "Preparation time:         " << clock.result() << " milliseconds\n";
    }
#endif

    clock.start();
    for (size_t point_index = 0; point_index < points.size(); ++point_index) {
        for (auto &rectangle: rectangles) {

            auto &current_point = points[point_index];
            if (rectangle.l_d.x <= current_point.x && current_point.x < rectangle.r_u.x &&
                rectangle.l_d.y <= current_point.y && current_point.y < rectangle.r_u.y) {
                ++result[point_index];
            }

        }
    }
    clock.finish();
#ifdef TIME_SCORING
    if (flag % 4 == 0) {
        std::cout << "Search time per point:    " << clock.result() / points.size() << " milliseconds\n";
    }
    ++flag;
#endif

    return result;
}