#include "bitvec2d.h"

#include <cstddef>

namespace spa {
BitVec2D::BitVec2D(std::size_t n_rows, std::size_t n_cols) noexcept
        : bit_array_(n_rows * n_cols), kCols(n_cols) {}
bool BitVec2D::At(std::size_t row, std::size_t col) const noexcept {
    return bit_array_.Get(CalcPos(row, col));
}
void BitVec2D::Set(std::size_t row, std::size_t col) noexcept {
    bit_array_.Set(CalcPos(row, col));
}
void BitVec2D::Unset(std::size_t row, std::size_t col) noexcept {
    bit_array_.Unset(CalcPos(row, col));
}
std::size_t BitVec2D::CalcPos(std::size_t row, std::size_t col) const noexcept {
    return row * kCols + col;
}

}  // namespace spa
