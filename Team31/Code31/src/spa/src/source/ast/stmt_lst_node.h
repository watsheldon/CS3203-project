#ifndef SRC_SPA_SRC_SOURCE_AST_STMT_LST_NODE_H_
#define SRC_SPA_SRC_SOURCE_AST_STMT_LST_NODE_H_

#include <string>
#include <cstring>
#include <memory>

#include "indexed_node.h"
#include "statement_node.h"

namespace spa {
class StatementNode;
class StmtLstNode : public IndexedNode<StmtLstNode> {
  public:
    void AddStatement(std::shared_ptr<StatementNode> statement);
    SharedPtrVec <StatementNode> get_statements() const;

  private:
    SharedPtrVec <StatementNode> statements_;
};
} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_AST_STMT_LST_NODE_H_
