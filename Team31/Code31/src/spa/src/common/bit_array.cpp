#include "bit_array.h"

#include <algorithm>
#include <cstddef>

namespace spa {
BitArray::BitArray(std::size_t length) noexcept
        : len_(length / kUnitSize + (length % kUnitSize != 0)) {
    array_ = new Unit[len_]();
}
BitArray::~BitArray() { delete[] array_; }
bool BitArray::Get(std::size_t pos) const noexcept {
    auto idx = pos / kUnitSize;
    auto shift = pos % kUnitSize;
    return array_[idx] >> shift & kOne;
}
void BitArray::Set(std::size_t pos) noexcept {
    auto idx = pos / kUnitSize;
    auto shift = pos % kUnitSize;
    array_[idx] |= kOne << shift;
}
void BitArray::Unset(std::size_t pos) noexcept {
    auto idx = pos / kUnitSize;
    auto shift = pos % kUnitSize;
    array_[idx] &= ~(kOne << shift);
}
void BitArray::Reset() noexcept { std::fill(array_, array_ + len_, 0); }
}  // namespace spa
