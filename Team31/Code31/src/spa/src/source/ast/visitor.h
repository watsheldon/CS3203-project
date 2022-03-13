#ifndef SRC_SPA_SRC_SOURCE_AST_VISITOR_H_
#define SRC_SPA_SRC_SOURCE_AST_VISITOR_H_

namespace spa {
class ProgramNode;
class ProcedureNode;
class StmtLstNode;
class AssignNode;
class CallNode;
class IfNode;
class WhileNode;
class ReadNode;
class PrintNode;
class AstVisitor {
  public:
    virtual void Visit(const ProgramNode &program_node) noexcept = 0;
    virtual void Visit(const ProcedureNode &procedure_node) noexcept = 0;
    virtual void Visit(const StmtLstNode &stmt_lst_node) noexcept = 0;
    virtual void Visit(const AssignNode &assign_node) noexcept = 0;
    virtual void Visit(const CallNode &call_node) noexcept = 0;
    virtual void Visit(const IfNode &if_node) noexcept = 0;
    virtual void Visit(const WhileNode &while_node) noexcept = 0;
    virtual void Visit(const ReadNode &read_node) noexcept = 0;
    virtual void Visit(const PrintNode &print_node) noexcept = 0;
    virtual ~AstVisitor() noexcept = 0;
};
inline AstVisitor::~AstVisitor() noexcept = default;
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_AST_VISITOR_H_
