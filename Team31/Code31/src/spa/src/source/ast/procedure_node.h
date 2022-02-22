#ifndef SRC_SPA_SRC_SOURCE_AST_PROCEDURE_NODE_H_
#define SRC_SPA_SRC_SOURCE_AST_PROCEDURE_NODE_H_

#include "indexed_node.h"
#include "stmt_lst_parent.h"
#include "visitable.h"
#include "visitor.h"

namespace spa {
class StmtLstNode;
class ProcedureNode : public IndexedNode<ProcedureNode>,
                      public StmtLstParent,
                      public Visitable {
  public:
    void AddStmtLst(const StmtLstNode *node) override;
    [[nodiscard]] const StmtLstNode *GetStmtlst() const;
    void Accept(AstVisitor &visitor) const override;

  private:
    const StmtLstNode *stmt_lst_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_AST_PROCEDURE_NODE_H_
