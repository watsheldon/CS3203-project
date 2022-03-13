#ifndef SPA_SRC_SPA_SRC_COMMON_BITVEC2D_H_
#define SPA_SRC_SPA_SRC_COMMON_BITVEC2D_H_

#include <cstddef>

#include "common/aliases.h"

namespace spa {
class BitVec2D {
  public:
    BitVec2D(std::size_t n_rows, std::size_t n_cols) noexcept;
    [[nodiscard]] bool At(std::size_t row, std::size_t col) const noexcept;
    void Set(std::size_t row, std::size_t col) noexcept;
    void Unset(std::size_t row, std::size_t col) noexcept;
    void Flip(std::size_t row, std::size_t col) noexcept;
    void Reset() noexcept;

  private:
    Vec2D<bool> map_;
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_COMMON_BITVEC2D_H_
