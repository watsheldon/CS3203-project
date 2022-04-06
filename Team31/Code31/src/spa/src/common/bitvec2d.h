#ifndef SPA_SRC_SPA_SRC_COMMON_BITVEC2D_H_
#define SPA_SRC_SPA_SRC_COMMON_BITVEC2D_H_

#include <cstddef>

#include "common/bit_array.h"

namespace spa {
class BitVec2D {
  public:
    BitVec2D(std::size_t n_rows, std::size_t n_cols) noexcept;
    BitVec2D(const BitVec2D &other) = delete;
    BitVec2D(BitVec2D &&other) = delete;
    [[nodiscard]] bool At(std::size_t row, std::size_t col) const noexcept;
    void Set(std::size_t row, std::size_t col) noexcept;
    void Unset(std::size_t row, std::size_t col) noexcept;

  private:
    BitArray bit_array_;
    const std::size_t kCols;
    [[nodiscard]] std::size_t CalcPos(std::size_t row,
                                      std::size_t col) const noexcept;
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_COMMON_BITVEC2D_H_
