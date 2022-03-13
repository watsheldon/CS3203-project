#include "program_node.h"

#include <vector>

#include "procedure_node.h"
#include "visitor.h"

namespace spa {
void ProgramNode::AddProcedure(const ProcedureNode *procedure) noexcept {
    procedures_.emplace_back(procedure);
}
const std::vector<const ProcedureNode *> &ProgramNode::GetProcedures()
        const noexcept {
    return procedures_;
}
void ProgramNode::Accept(AstVisitor &visitor) const noexcept {
    visitor.Visit(*this);
}
}  // namespace spa
