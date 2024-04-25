#include "brute_force.h"

std::vector<int> BruteForce(const std::vector<Rectangle> &rectangles,
                            const std::vector<Point> &points) {
    std::vector<int> result(points.size(), 0);

    for (size_t point_index = 0; point_index < points.size(); ++point_index) {
        for (auto &rectangle: rectangles) {

            auto &current_point = points[point_index];
            if (rectangle.l_d.x <= current_point.x && current_point.x < rectangle.r_u.x &&
                rectangle.l_d.y <= current_point.y && current_point.y < rectangle.r_u.y) {
                ++result[point_index];
            }

        }
    }

    return result;
}