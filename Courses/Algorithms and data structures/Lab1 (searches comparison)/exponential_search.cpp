#include <vector>

bool ExponentialSearch(const std::vector<std::vector<int>> &data, const int &value) {

    size_t height = data.size(), width = data[0].size();
    size_t row = 0, column = width - 1;

    while (row < height) {
        if (data[row][column] == value) {
            return true;
        }

        if (data[row][column] < value) {
            ++row;
        } else {
            int step = 1;
            int current = static_cast<int>(column);

            while (current >= 0) {
                if (data[row][current] < value) {
                    break;
                }

                current -= step;
                step *= 2;
            }

            size_t left = std::max(current, 0), right =
                    std::min(static_cast<size_t> (current + step), data[row].size() - 1);

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