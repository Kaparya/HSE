#include "tests.h"

#include <iostream>
#include <cassert>

// Brute force is used to generate expected table for 2 other algorithms, due to its simplicity
#include "../src/Algorithms/brute_force.h"
#include "../src/AdditionalFiles/clock.h"

#include "../src/constants.h"

extern std::ofstream output;

using FunctionPtr = std::vector<int> (*)(const std::vector<Rectangle> &, const std::vector<Point> &);

// =====================================
// ========== Ordinary tests ===========
// =====================================

void NormalTest(FunctionPtr function) {
    std::vector<Rectangle> rectangles = {
            {{2, 2}, {6,  8}},
            {{5, 4}, {9,  10}},
            {{4, 0}, {11, 6}},
            {{8, 2}, {12, 12}}
    };
    std::vector<Point> points = {
            {2,  2},
            {12, 12},
            {10, 4},
            {5,  5},
            {2,  10},
            {1,  2},
            {2,  1},
            {-1, -1}
    };
    std::vector<int> expected = {1, 0, 2, 3, 0, 0, 0, 0};

    auto result = function(rectangles, points);

    assert(result.size() == expected.size());
    for (size_t index = 0; index < expected.size(); ++index) {
        assert(result[index] == expected[index]);
    }
    // std::cout << "Normal test PASSED!\n";
}

void ZeroTest(FunctionPtr function) {
    std::vector<Rectangle> rectangles;
    std::vector<Point> points;
    std::vector<int> expected;

    auto result = function(rectangles, points);

    assert(result.size() == expected.size());
    for (size_t index = 0; index < expected.size(); ++index) {
        assert(result[index] == expected[index]);
    }
    // std::cout << "Zero test PASSED!\n";
}

void NoRectanglesTest(FunctionPtr function) {
    std::vector<Rectangle> rectangles;
    std::vector<Point> points = {
            {1,  2},
            {10, 21},
            {34, 4}
    };
    std::vector<int> expected = {0, 0, 0};

    auto result = function(rectangles, points);

    assert(result.size() == expected.size());
    for (size_t index = 0; index < expected.size(); ++index) {
        assert(result[index] == expected[index]);
    }
    // std::cout << "No rectangles test PASSED!\n";
}

void NoPointsTest(FunctionPtr function) {
    std::vector<Rectangle> rectangles = {
            {{2, 2}, {6,  8}},
            {{5, 4}, {9,  10}},
            {{4, 0}, {11, 6}},
            {{8, 2}, {12, 12}}
    };
    std::vector<Point> points;
    std::vector<int> expected;

    auto result = function(rectangles, points);

    assert(result.size() == expected.size());
    for (size_t index = 0; index < expected.size(); ++index) {
        assert(result[index] == expected[index]);
    }
    // std::cout << "No points test PASSED!\n";
}

// -------------------------------------

void RunOrdinaryTests(FunctionPtr function) {
    NormalTest(function);
    ZeroTest(function);
    NoRectanglesTest(function);
    NoPointsTest(function);
}

// =====================================
// ============ Big tests ==============
// =====================================

void BigTest(FunctionPtr function, int number_of_rectangles, int number_of_points) {

    std::srand(0);

    std::vector<Rectangle> rectangles(number_of_rectangles);
    for (int index = 0; index < number_of_rectangles; ++index) {
        if (2 * number_of_rectangles - index > 0) {
            rectangles[index] = {{10 * index,                              10 * index},
                                 {10 * (2 * number_of_rectangles - index), 10 * (2 * number_of_rectangles - index)}};
        } else {
            int x1, y1, x2, y2;
            x1 = rand() % 100;
            y1 = rand() % 100;
            x2 = rand() % 100;
            y2 = rand() % 100;

            Rectangle &new_rect = rectangles[index];
            new_rect.l_d = {std::min(x1, x2), std::min(y1, y2)};
            new_rect.r_u = {std::max(x1, x2), std::max(y1, y2)};
        }
    }

    std::vector<Point> points = {{-1,                -1},
                                 {-1,                1},
                                 {10 * (2 * number_of_rectangles), 10 * (2 * number_of_rectangles)}};
    for (int index = 0; index < number_of_points; ++index) {
        points.push_back({static_cast<int>(std::pow((11317 * index), 31)) % (20 * number_of_rectangles),
                          static_cast<int>(std::pow((2326309 * index), 31)) % (20 * number_of_rectangles)});
    }

    std::vector<int> expected = BruteForce(rectangles, points);

    auto result = function(rectangles, points);

    assert(result.size() == expected.size());
    for (size_t index = 0; index < expected.size(); ++index) {
        assert(result[index] == expected[index]);
    }
    // std::cout << "Big test PASSED!\n";
}

// -------------------------------------

void RunBigTests(FunctionPtr function, int number_of_rectangles, int number_of_points) {
    BigTest(function, number_of_rectangles, number_of_points);
}

// =====================================
// =========== Random tests ============
// =====================================

#include <random>

void RandomTest(FunctionPtr function) {
    const int number = 1000;

    srand(0);

    std::vector<Rectangle> rectangles(number);
    for (int index = 0; index < number; ++index) {
        int x1, y1, x2, y2;
        x1 = rand() % 100;
        y1 = rand() % 100;
        x2 = rand() % 100;
        y2 = rand() % 100;

        Rectangle &new_rect = rectangles[index];
        new_rect.l_d = {std::min(x1, x2), std::min(y1, y2)};
        new_rect.r_u = {std::max(x1, x2), std::max(y1, y2)};
    }

    std::vector<Point> points(number);
    for (int index = 0; index < number; ++index) {
        points[index] = {rand() % 150 - 25, rand() % 150 - 25};
    }

    std::vector<int> expected = BruteForce(rectangles, points);

    auto result = function(rectangles, points);

    assert(result.size() == expected.size());
    for (size_t index = 0; index < expected.size(); ++index) {
        assert(result[index] == expected[index]);
    }
    // std::cout << "Random test PASSED!\n";
}

// -------------------------------------

void RunRandomTests(FunctionPtr function) {
    RandomTest(function);
}
