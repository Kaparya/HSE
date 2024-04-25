#ifndef BRUTE_FORCE_H
#define BRUTE_FORCE_H

#include <vector>

#include "../Objects/Rectangle.h"

std::vector<int> BruteForce(const std::vector<Rectangle>& rectangles,
                             const std::vector<Point>& points);

#endif