#ifndef SRC_SPA_SRC_SOURCE_AST_PROGRAM_NODE_H_
#define SRC_SPA_SRC_SOURCE_AST_PROGRAM_NODE_H_

#include <memory>

#include "procedure_node.h"

namespace spa {
class ProgramNode : public AbstractSyntaxTreeNode {
  public:
    bool AddProcedure(std::shared_ptr<ProcedureNode> procedure);
    [[nodiscard]] SharedPtrVec <ProcedureNode> get_procedures() const;

  private:
    SharedPtrVec <ProcedureNode> procedures_;
};
} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_AST_PROGRAM_NODE_H_
