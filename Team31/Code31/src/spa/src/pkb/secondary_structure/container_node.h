#ifndef SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CONTAINER_NODE_H_
#define SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CONTAINER_NODE_H_

namespace spa {

class ContainerNode {
  public:
    ContainerNode() noexcept = default;

    void SetParent(int p) noexcept;
    void SetFirstChild(int c) noexcept;
    void SetNextSibling(int s) noexcept;
    [[nodiscard]] int GetParent() const noexcept;
    [[nodiscard]] int GetFirstChild() const noexcept;
    [[nodiscard]] int GetNextSibling() const noexcept;

  private:
    int parent_ = 0;
    int first_child_ = 0;
    int next_sibling_ = 0;
};

}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CONTAINER_NODE_H_
