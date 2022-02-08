#include "program_node.h"

#include <memory>
#include <utility>

namespace spa {
bool ProgramNode::AddProcedure(std::shared_ptr<ProcedureNode> procedure) {
    procedures_.emplace_back(std::move(procedure));
    return true;
}
AbstractSyntaxTreeNode::SharedPtrVec<ProcedureNode> ProgramNode::get_procedures() const {
    return procedures_;
}
} // namespace spa
