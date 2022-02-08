#ifndef SRC_SPA_SRC_SOURCE_AST_PROCEDURE_H_
#define SRC_SPA_SRC_SOURCE_AST_PROCEDURE_H_

#include <memory>

#include "indexed_node.h"
#include "visitable.h"
#include "visitor.h"

namespace spa {
class StmtLstNode;
class ProcedureNode : public IndexedNode<ProcedureNode>, public Visitable {
  public:
    void set_stmtlst(std::shared_ptr<StmtLstNode> stmtLst);
    [[nodiscard]] std::shared_ptr<StmtLstNode> get_stmtlst() const;
  private:
    void Accept(AstVisitor &visitor) const override;

  private:
    std::shared_ptr<StmtLstNode> stmt_lst_;
};
} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_AST_PROCEDURE_H_
