#ifndef SRC_SPA_SRC_SOURCE_AST_PROCEDURE_H_
#define SRC_SPA_SRC_SOURCE_AST_PROCEDURE_H_

#include <memory>

#include "indexed_node.h"
#include "stmt_lst_node.h"

namespace spa {
class StmtLstNode;
class ProcedureNode : public IndexedNode<ProcedureNode> {
  public:
    void set_stmtlst(std::shared_ptr<StmtLstNode> stmtLst);
    std::shared_ptr<StmtLstNode> get_stmtlst() const;

  private:
    std::shared_ptr<StmtLstNode> stmt_lst_;
};
} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_AST_PROCEDURE_H_
