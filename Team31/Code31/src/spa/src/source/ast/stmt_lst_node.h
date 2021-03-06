#ifndef SRC_SPA_SRC_SOURCE_AST_STMT_LST_NODE_H_
#define SRC_SPA_SRC_SOURCE_AST_STMT_LST_NODE_H_

#include <vector>

#include "indexed_node.h"
#include "visitable.h"
#include "visitor.h"

namespace spa {
class StatementNode;
class StmtLstNode : public IndexedNode<StmtLstNode>, public Visitable {
  public:
    StmtLstNode() noexcept = default;
    void AddStatement(const StatementNode *statement) noexcept;
    [[nodiscard]] const std::vector<const StatementNode *> &GetStatements()
            const noexcept;
    void Accept(AstVisitor &visitor) const noexcept override;

  private:
    std::vector<const StatementNode *> statements_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_AST_STMT_LST_NODE_H_
