#ifndef SRC_SPA_SRC_COMMON_PRIORITY_SET_H_
#define SRC_SPA_SRC_COMMON_PRIORITY_SET_H_

#include <functional>
#include <queue>
#include <set>
#include <vector>

namespace spa {
template <class T, class Container = std::vector<T>,
          class Compare = std::less<typename Container::value_type>>
class PrioritySet {
  public:
    using container_type = Container;
    using value_type = typename Container::value_type;
    using size_type = typename Container::size_type;
    PrioritySet() : PrioritySet(Compare(), Container()) {}
    PrioritySet(const Compare& compare, Container&& cont) : set_(cont) {
        std::for_each(set_.begin(), set_.end(), priority_queue_.emplace);
    }
    template <typename... Args>
    void emplace(Args&&... args) {
        const auto& [itr, inserted] = set_.emplace(std::forward<Args>(args)...);
        if (inserted) {
            priority_queue_.emplace(std::forward<Args>(args)...);
        }
    }
    [[nodiscard]] bool empty() const { return set_.empty(); }
    [[nodiscard]] size_type size() const noexcept { return set_.size(); }
    void push(const value_type& value) {
        const auto& [itr, inserted] = set_.insert(value);
        if (inserted) {
            priority_queue_.push(value);
        }
    }
    void push(value_type&& value) {
        const auto& [itr, inserted] = set_.insert(value);
        if (inserted) {
            priority_queue_.push(std::move(value));
        }
    }
    void pop() {
        set_.erase(priority_queue_.top());
        priority_queue_.pop();
    }

  private:
    std::set<T, Compare> set_;
    std::priority_queue<T, Container, Compare> priority_queue_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_COMMON_PRIORITY_SET_H_
