#ifndef SRC_SPA_SRC_COMMON_BIT_ARRAY_H_
#define SRC_SPA_SRC_COMMON_BIT_ARRAY_H_

#include <cstddef>
#include <cstdint>

namespace spa {
class BitArray {
  public:
    explicit BitArray(std::size_t length) noexcept;
    ~BitArray();
    [[nodiscard]] bool Get(std::size_t pos) const noexcept;
    void Set(std::size_t pos) noexcept;
    void Unset(std::size_t pos) noexcept;
    void Reset() noexcept;

  private:
    using Unit = uint64_t;
    static const std::size_t kUnitSize = 64;
    static const Unit kOne = 1;

    const std::size_t len_;
    Unit* array_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_COMMON_BIT_ARRAY_H_
