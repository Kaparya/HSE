#include "persistent_st.h"
#include "../AdditionalFiles/clock.h"

#include "../constants.h"

#include <set>
#include <memory>
#include <algorithm>

extern std::ofstream output;

struct CoordsStorage {
    int x = 0;
    int y_start = 0, y_finish = 0;
    bool end = false;

    bool operator<(const CoordsStorage &rhs) const {
        if (x != rhs.x) {
            return x < rhs.x;
        }
        if (end != rhs.end) {
            return end < rhs.end;
        }
        if (y_start != rhs.y_start) {
            return y_start < rhs.y_start;
        }
        return y_finish < rhs.y_finish;
    }
};

struct Node {
    int from = 0, to = 0; // [from, to)
    std::shared_ptr<Node> left = nullptr;
    std::shared_ptr<Node> right = nullptr;
    int value = 0;

    Node() = default;

    Node(int y_f, int y_t) : from(y_f), to(y_t) {}

    Node(int y_f, int y_t, const std::shared_ptr<Node> &left_node,
         const std::shared_ptr<Node> &right_node) :
            from(y_f), to(y_t), left(left_node), right(right_node) {}

    explicit Node(const std::shared_ptr<Node> &rhs) {
        from = rhs->from;
        to = rhs->to;
        left = rhs->left;
        right = rhs->right;
        value = rhs->value;
    }
};

inline void ModifyTree(const std::shared_ptr<Node> &current, const std::shared_ptr<Node> &previous,
                int from, int to, int value) {
    if (!previous || from > to) {
        return;
    }
    if (from <= current->from && current->to <= to) {
        current->value += value;
        return;
    }

    if (previous->left && (previous->left->from <= from && from <= previous->left->to ||
                           previous->left->from <= to && to <= previous->left->to)) {
        current->left = std::make_shared<Node>(previous->left);
        ModifyTree(current->left, previous->left,
                   std::max(from, previous->left->from),
                   std::min(previous->left->to, to), value);
    }

    if (previous->right && (previous->right->from <= from && from <= previous->right->to ||
                            previous->right->from <= to && to <= previous->right->to)) {
        current->right = std::make_shared<Node>(previous->right);
        ModifyTree(current->right, previous->right,
                   std::max(from, previous->right->from),
                   std::min(previous->right->to, to), value);
    }
}

inline int FindAnswer(const std::shared_ptr<Node> &current, int y_coord) {
    if (!current) {
        return 0;
    }

    if (current->left && current->left->from <= y_coord && y_coord <= current->left->to) {
        return current->value + FindAnswer(current->left, y_coord);
    } else if (current->right && current->right->from <= y_coord && y_coord <= current->right->to) {
        return current->value + FindAnswer(current->right, y_coord);
    }
    return current->value;
}

std::vector<int> PersistentSTAlgorithm(const std::vector<Rectangle> &rectangles,
                                       const std::vector<Point> &points) {
    Clock clock, total;
    total.start();
    clock.start();
    // Create set of distinct coordinates, then convert it to std::vector
    std::set<int> distinct_x_coords;
    std::set<int> distinct_y_coords;
    // Make sorted vector of coords convenient for scanline
    std::vector<CoordsStorage> coords_scanline;
    coords_scanline.reserve(rectangles.size() * 2);
    for (auto &rectangle: rectangles) {
        distinct_x_coords.insert(rectangle.r_u.x);
        distinct_x_coords.insert(rectangle.l_d.x);

        distinct_y_coords.insert(rectangle.r_u.y);
        distinct_y_coords.insert(rectangle.l_d.y);

        coords_scanline.push_back({rectangle.l_d.x, rectangle.l_d.y, rectangle.r_u.y});
        coords_scanline.push_back({rectangle.r_u.x, rectangle.l_d.y, rectangle.r_u.y, true});
    }
    std::vector<int> x_coords;
    x_coords.reserve(distinct_x_coords.size());
    for (const auto &x: distinct_x_coords) {
        x_coords.push_back(x);
    }
    std::vector<int> y_coords;
    y_coords.reserve(distinct_y_coords.size());
    for (const auto &y: distinct_y_coords) {
        y_coords.push_back(y);
    }

    // Sorting coords for scanline
    std::sort(coords_scanline.begin(), coords_scanline.end());

    // Building segment tree
    std::vector<std::shared_ptr<Node>> previous_level;
    for (auto y_coord: distinct_y_coords) {
        auto new_one = std::make_shared<Node>(y_coord, y_coord);
        previous_level.push_back(new_one);
    }
    while (previous_level.size() > 1) {
        std::vector<std::shared_ptr<Node>> next_level;

        for (size_t index = 0; index < previous_level.size(); index += 2) {
            std::shared_ptr<Node> left = previous_level[index];
            std::shared_ptr<Node> right = nullptr;
            if (index + 1 < previous_level.size()) {
                right = previous_level[index + 1];
            }
            auto new_one = std::make_shared<Node>(left->from, right ? right->to : left->to, left, right);

            next_level.emplace_back(new_one);
        }

        previous_level = next_level;
    }
    std::shared_ptr<Node> root = nullptr;
    if (previous_level.size() == 1) {
        root = previous_level[0];
    }


    // Scanline algorithm
    std::vector<std::shared_ptr<Node>> roots_by_x;
    std::shared_ptr<Node> prev_root = root;

    size_t index = 0;
    for (const auto &x_coord: x_coords) {

        root = std::make_shared<Node>(prev_root);

        while (index < coords_scanline.size() && coords_scanline[index].x == x_coord) {
            ModifyTree(root, prev_root, coords_scanline[index].y_start, coords_scanline[index].y_finish - 1,
                       coords_scanline[index].end ? -1 : 1);
            prev_root = root;
            root = std::make_shared<Node>(prev_root);
            ++index;
        }

        roots_by_x.push_back(prev_root);
    }
    clock.finish();
    total.finish();
    auto total_result = total.result();
#ifdef TIME_SCORING
    output << clock.result() << ' ';
    std::cout << "Preparation time:         " << clock.result() << " milliseconds\n";
#endif

    total.start();
    clock.start();
    // Find answers for points
    std::vector<int> result(points.size(), 0);

    for (index = 0; index < points.size(); ++index) {
        int x = std::lower_bound(x_coords.begin(), x_coords.end(), points[index].x) - x_coords.begin();
        if (x == x_coords.size()) {
            continue;
        }
        if (points[index].x < x_coords[x]) {
            --x;
            if (x < 0) {
                continue;
            }
        }

        auto y = std::lower_bound(y_coords.begin(), y_coords.end(), points[index].y) - y_coords.begin();
        if (y_coords[y] == points[index].y) {
            result[index] = FindAnswer(roots_by_x[x], y_coords[y]);
        } else if (0 < y && y < y_coords.size()) {
            --y;
            result[index] = FindAnswer(roots_by_x[x], y_coords[y]);
        }
    }
    clock.finish();
    total.finish();
    total_result += total.result();
#ifdef TIME_SCORING
    output << clock.result() / points.size() << ' ';
    std::cout << "Search time per point:    " << clock.result() / points.size() << " milliseconds\n";
    output << total_result << '\n';
    std::cout << "Total time on a big test: " << total_result << " milliseconds" << '\n';
#endif

    return result;
}
