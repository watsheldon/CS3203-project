#include "program_node.h"

#include <memory>
#include <utility>

#include "visitor.h"

namespace spa {
bool ProgramNode::AddProcedure(std::shared_ptr<ProcedureNode> procedure) {
    procedures_.emplace_back(std::move(procedure));
    return true;
}
AbstractSyntaxTreeNode::SharedPtrVec<ProcedureNode>
ProgramNode::get_procedures() const {
    return procedures_;
}
void ProgramNode::Accept(AstVisitor &visitor) const { visitor.Visit(*this); }
}  // namespace spa
