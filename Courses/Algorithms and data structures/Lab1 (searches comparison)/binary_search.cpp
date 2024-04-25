#include <vector>

bool BinarySearch(const std::vector<std::vector<int>>& data, const int& value) {

    size_t height = data.size(), width = data[0].size();
    size_t row = 0, column = width - 1;

    while (row < height) {
        if (data[row][column] == value) {
            return true;
        }

        if (data[row][column] < value) {
            ++row;
        } else {
            size_t left = 0, right = column;

            while (left < right) {
                size_t mid = (left + right + 1) / 2;
                if (data[row][mid] <= value) {
                    left = mid;
                } else {
                    right = mid - 1;
                }
            }

            if (data[row][left] == value) {
                return true;
            }
            if (left == 0 && data[row][left] > value) {
                break;
            }
            column = left;
        }
    }
    return false;
}
