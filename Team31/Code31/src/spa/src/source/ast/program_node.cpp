#include "program_node.h"

#include <vector>

#include "procedure_node.h"
#include "visitor.h"

namespace spa {
void ProgramNode::AddProcedure(const ProcedureNode *procedure) {
    procedures_.emplace_back(procedure);
}
const std::vector<const ProcedureNode *> &ProgramNode::GetProcedures() const {
    return procedures_;
}
void ProgramNode::Accept(AstVisitor &visitor) const { visitor.Visit(*this); }
}  // namespace spa
