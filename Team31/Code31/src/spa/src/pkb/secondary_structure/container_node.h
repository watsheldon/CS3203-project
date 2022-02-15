#ifndef SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CONTAINER_NODE_H_
#define SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CONTAINER_NODE_H_

namespace spa {

class ContainerNode {
  public:
    ContainerNode() = default;

    void SetParent(int p);
    void SetFirstChild(int c);
    void SetNextSibling(int s);
    [[nodiscard]] int GetParent() const;
    [[nodiscard]] int GetFirstChild() const;
    [[nodiscard]] int GetNextSibling() const;

  private:
    int parent_ = 0;
    int first_child_ = 0;
    int next_sibling_ = 0;
};

}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CONTAINER_NODE_H_
