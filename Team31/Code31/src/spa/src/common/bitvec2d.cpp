#include "bitvec2d.h"

#include <algorithm>
#include <vector>

namespace spa {
BitVec2D::BitVec2D(std::size_t n_rows, std::size_t n_cols)
        : map_(n_rows, std::vector<bool>(n_cols, false)) {}
bool BitVec2D::At(std::size_t row, std::size_t col) const {
    return map_[row][col];
}
void BitVec2D::Set(std::size_t row, std::size_t col) { map_[row][col] = true; }
void BitVec2D::Unset(std::size_t row, std::size_t col) {
    map_[row][col] = false;
}
void BitVec2D::Flip(std::size_t row, std::size_t col) { map_[row][col].flip(); }
void BitVec2D::Reset() {
    std::for_each(map_.begin(), map_.end(), [](std::vector<bool> &row) {
        std::fill(row.begin(), row.end(), false);
    });
}

}  // namespace spa
