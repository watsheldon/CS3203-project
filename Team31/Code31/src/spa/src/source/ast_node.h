#ifndef SRC_SPA_SRC_SOURCE_AST_NODE_H_
#define SRC_SPA_SRC_SOURCE_AST_NODE_H_

#include <memory>
#include <vector>

namespace spa {

template <typename T>
using SharedPtrVec = std::vector<std::shared_ptr<T>>;

class AstNode {
    public:
    virtual ~AstNode() = 0;
};

} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_AST_NODE_H_