#ifndef ORDINARY_TEST_H
#define ORDINARY_TEST_H

#include <vector>
#include "../src/Objects/Rectangle.h"

void RunOrdinaryTests(std::vector<int> (*function)(const std::vector<Rectangle> &, const std::vector<Point> &));

void RunBigTests(std::vector<int> (*function)(const std::vector<Rectangle> &, const std::vector<Point> &),
                 int number_of_rectangles, int number_of_points);

void RunRandomTests(std::vector<int> (*function)(const std::vector<Rectangle> &, const std::vector<Point> &));

#endif