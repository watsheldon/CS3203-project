#ifndef SRC_SPA_SRC_PKB_CONTAINER_NODE_H_
#define SRC_SPA_SRC_PKB_CONTAINER_NODE_H_

namespace spa {

class ContainerNode {
  public:
    ContainerNode();

    void setParent(int p);
    void setChild(int c);
    void setSibling(int s);
    int getParent();
    int getChild();
    int getSibling();

  private:
    int parent;
    int first_child;
    int first_sibling;
};

}  // namespace spa

#endif //SRC_SPA_SRC_PKB_CONTAINER_NODE_H_
