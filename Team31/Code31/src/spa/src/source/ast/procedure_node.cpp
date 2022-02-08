#include "procedure_node.h"

#include <memory>
#include <utility>

#include "stmt_lst_node.h"
#include "visitor.h"

namespace spa {
void ProcedureNode::set_stmtlst(std::shared_ptr<StmtLstNode> stmtLst) {
    stmt_lst_ = std::move(stmtLst);
}
std::shared_ptr<StmtLstNode> ProcedureNode::get_stmtlst() const {
    return stmt_lst_;
}
void ProcedureNode::Accept(AstVisitor &visitor) const {
    visitor.Visit(*this);
}
} // namespace spa
