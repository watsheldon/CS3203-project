#ifndef SRC_SPA_SRC_SOURCE_AST_STMT_LST_NODE_H_
#define SRC_SPA_SRC_SOURCE_AST_STMT_LST_NODE_H_

#include <cstring>

#include <string>
#include <memory>

#include "indexed_node.h"
#include "visitable.h"
#include "visitor.h"

namespace spa {
class StatementNode;
class StmtLstNode : public IndexedNode<StmtLstNode>, public Visitable {
  public:
    void AddStatement(std::shared_ptr<StatementNode> statement);
    [[nodiscard]] SharedPtrVec <StatementNode> get_statements() const;
    void Accept(AstVisitor &visitor) const override;
  private:
    SharedPtrVec <StatementNode> statements_;
};
} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_AST_STMT_LST_NODE_H_
