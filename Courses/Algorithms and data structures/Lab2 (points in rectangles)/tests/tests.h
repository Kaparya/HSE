#ifndef ORDINARY_TEST_H
#define ORDINARY_TEST_H

#include <vector>
#include "../src/Objects/Rectangle.h"

void RunOrdinaryTests(std::vector<int> (*function)(const std::vector<Rectangle> &, const std::vector<Point> &));

void RunBigTests(std::vector<int> (*function)(const std::vector<Rectangle> &, const std::vector<Point> &),
                 int number_of_rectangles = 1000, int number_of_points = 1000);

void RunRandomTests(std::vector<int> (*function)(const std::vector<Rectangle> &, const std::vector<Point> &));

#endif