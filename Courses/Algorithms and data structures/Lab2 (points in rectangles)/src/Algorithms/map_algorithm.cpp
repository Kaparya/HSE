#include "map_algorithm.h"

#include <set>

std::vector<int> MapAlgorithm(const std::vector<Rectangle> &rectangles,
                              const std::vector<Point> &points) {

    std::vector<int> result(points.size(), 0);

    // Create set of distinct coordinates, then convert it to std::vector
    std::set<size_t> distinct_x_coords, distinct_y_coords;
    for (auto &rectangle: rectangles) {
        distinct_x_coords.insert(rectangle.r_u.x);
        distinct_x_coords.insert(rectangle.l_d.x);

        distinct_y_coords.insert(rectangle.r_u.y);
        distinct_y_coords.insert(rectangle.l_d.y);
    }
    std::vector<size_t> x_coords;
    x_coords.reserve(distinct_x_coords.size());
    for (auto coord: distinct_x_coords) {
        x_coords.push_back(coord);
    }
    std::vector<size_t> y_coords;
    y_coords.reserve(distinct_y_coords.size());
    for (auto coord: distinct_y_coords) {
        y_coords.push_back(coord);
    }

    // Build map
    std::vector<std::vector<int>> points_map(x_coords.size(), std::vector<int>(y_coords.size(), 0));
    for (auto &rectangle: rectangles) {

        size_t x_start = std::lower_bound(x_coords.begin(), x_coords.end(), rectangle.l_d.x) - x_coords.begin();
        size_t x_end = std::lower_bound(x_coords.begin(), x_coords.end(), rectangle.r_u.x) - x_coords.begin();

        size_t y_start = std::lower_bound(y_coords.begin(), y_coords.end(), rectangle.l_d.y) - y_coords.begin();
        size_t y_end = std::lower_bound(y_coords.begin(), y_coords.end(), rectangle.r_u.y) - y_coords.begin();

        for (size_t x = x_start; x < x_end; ++x) {
            for (size_t y = y_start; y < y_end; ++y) {
                ++points_map[x][y];
            }
        }
    }

    // Find answers for points
    for (size_t point_index = 0; point_index < points.size(); ++point_index) {

        int x = std::lower_bound(x_coords.begin(), x_coords.end(), points[point_index].x) - x_coords.begin();
        if (x == x_coords.size()) {
            result[point_index] = 0;
            continue;
        }
        if (points[point_index].x < x_coords[x]) {
            --x;
            if (x < 0) {
                result[point_index] = 0;
                continue;
            }
        }

        int y = std::lower_bound(y_coords.begin(), y_coords.end(), points[point_index].y) - y_coords.begin();
        if (y == y_coords.size()) {
            result[point_index] = 0;
            continue;
        }
        if (points[point_index].y < y_coords[y]) {
            --y;
            if (y < 0) {
                result[point_index] = 0;
                continue;
            }
        }

        result[point_index] = points_map[x][y];
    }

    return result;
}