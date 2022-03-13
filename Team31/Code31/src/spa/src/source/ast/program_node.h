#ifndef SRC_SPA_SRC_SOURCE_AST_PROGRAM_NODE_H_
#define SRC_SPA_SRC_SOURCE_AST_PROGRAM_NODE_H_

#include <vector>

#include "procedure_node.h"
#include "visitable.h"
#include "visitor.h"

namespace spa {
class ProgramNode : public AbstractSyntaxTreeNode, public Visitable {
  public:
    ProgramNode() noexcept = default;
    void AddProcedure(const ProcedureNode *procedure) noexcept;
    [[nodiscard]] const std::vector<const ProcedureNode *> &GetProcedures()
            const noexcept;
    void Accept(AstVisitor &visitor) const noexcept override;

  private:
    std::vector<const ProcedureNode *> procedures_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_AST_PROGRAM_NODE_H_
