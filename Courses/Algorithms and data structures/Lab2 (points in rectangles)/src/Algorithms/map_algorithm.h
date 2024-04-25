#ifndef MAP_ALGORITHM_H
#define MAP_ALGORITHM_H

#include <vector>

#include "../Objects/Rectangle.h"

std::vector<int> MapAlgorithm(const std::vector<Rectangle> &rectangles,
                              const std::vector<Point> &points);

#endif