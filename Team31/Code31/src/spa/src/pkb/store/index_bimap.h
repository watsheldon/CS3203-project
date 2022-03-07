#ifndef SRC_SPA_SRC_PKB_STORE_INDEX_BIMAP_H_
#define SRC_SPA_SRC_PKB_STORE_INDEX_BIMAP_H_

#include <map>
#include <utility>

#include "common/aliases.h"

namespace spa {
// A bidirectional map of (key, val) pairs where key is the index
template <typename V>
class IndexBimap {
  public:
    IndexBimap(std::vector<V> &&vals) : key_to_val_(std::move(vals)) {
        for (int i = 1; i < key_to_val_.size(); ++i) {
            val_to_key_.emplace(key_to_val_[i], i);
        }
    }

    [[nodiscard]] int GetKey(const V &val) const {
        auto iter = val_to_key_.find(val);
        if (iter == val_to_key_.end()) {
            return 0;
        }
        return iter->second;
    }

    [[nodiscard]] const V &GetVal(const int key) const {
        return key_to_val_[key];
    }

    [[nodiscard]] const std::vector<V> &GetAllVals() const {
        return key_to_val_;
    }

    [[nodiscard]] size_t GetValsSize() const { return key_to_val_.size() - 1; }

  private:
    std::vector<V> key_to_val_;
    std::map<V, int> val_to_key_;
};

template <>
class IndexBimap<int> {
  public:
    IndexBimap(size_t key_count, size_t val_count)
            : key_to_val_(key_count + 1, 0), val_to_key_(val_count + 1, 0) {}

    void Set(int key, int val) {
        key_to_val_[key] = val;
        val_to_key_[val] = key;
    }

    [[nodiscard]] int GetKey(int val) const { return val_to_key_[val]; }

    [[nodiscard]] int GetVal(int key) const { return key_to_val_[key]; }

  private:
    std::vector<int> key_to_val_;
    std::vector<int> val_to_key_;
};

template <>
class IndexBimap<std::vector<int>> {
  public:
    IndexBimap(size_t key_count, size_t val_count)
            : key_to_val_(key_count + 1), val_to_key_(val_count + 1) {}

    void Set(int key, std::vector<int> &&vals) {
        key_to_val_[key] = vals;
        for (auto &val : vals) {
            val_to_key_[val].emplace_back(key);
        }
    }

    [[nodiscard]] const std::vector<int> &GetKeys(int val) const {
        return val_to_key_[val];
    }

    [[nodiscard]] const std::vector<int> &GetVals(int key) const {
        return key_to_val_[key];
    }

  private:
    Vec2D<int> key_to_val_;
    Vec2D<int> val_to_key_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_INDEX_BIMAP_H_
