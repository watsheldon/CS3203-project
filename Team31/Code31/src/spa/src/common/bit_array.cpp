#include "bit_array.h"

#include <cstddef>

namespace spa {
BitArray::BitArray(std::size_t length) noexcept {
    array_ = new Unit[length / kUnitSize + (length % kUnitSize != 0)]();
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
}  // namespace spa
