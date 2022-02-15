#ifndef SRC_SPA_SRC_SOURCE_DESIGN_ABSTRACTION_EXTRACTOR_H_
#define SRC_SPA_SRC_SOURCE_DESIGN_ABSTRACTION_EXTRACTOR_H_

#include <memory>

#include "ast/abstract_syntax_tree.h"
#include "pkb/knowledge_base.h"
#include "pkb/program_knowledge_base.h"

namespace spa {
class IfWhileNode;

class DesignAbstractionExtractor : public AstVisitor {
    using AST = std::unique_ptr<AbstractSyntaxTree>;
    using PKB = std::unique_ptr<ProgramKnowledgeBase>;

  public:
    std::unique_ptr<KnowledgeBase> Extract(AST ast);
    void Visit(const ProgramNode &program_node) override;
    void Visit(const ProcedureNode &procedure_node) override;
    void Visit(const StmtLstNode &stmt_lst_node) override;
    void Visit(const AssignNode &assign_node) override;
    void Visit(const CallNode &call_node) override;
    void Visit(const IfNode &if_node) override;
    void Visit(const WhileNode &while_node) override;
    void Visit(const ReadNode &read_node) override;
    void Visit(const PrintNode &print_node) override;

  private:
    PKB pkb_;
    void SetUsesFromCondition(const IfWhileNode &if_while_node);
};
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_DESIGN_ABSTRACTION_EXTRACTOR_H_
