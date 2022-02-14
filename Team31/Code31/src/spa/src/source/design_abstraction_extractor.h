#ifndef SRC_SPA_SRC_SOURCE_DESIGN_ABSTRACTION_EXTRACTOR_H_
#define SRC_SPA_SRC_SOURCE_DESIGN_ABSTRACTION_EXTRACTOR_H_

#include <memory>

#include "ast/abstract_syntax_tree.h"
#include "pkb/knowledge_base.h"

namespace spa {
class DesignAbstractionExtractor : public AstVisitor {
    using AST = std::unique_ptr<AbstractSyntaxTree>;

  public:
    std::unique_ptr<KnowledgeBase> Extract(AST ast);
    void Visit(const ProgramNode &program_node);
    void Visit(const ProcedureNode &procedure_node);
    void Visit(const StmtLstNode &stmt_lst_node);
    void Visit(const AssignNode &assign_node);
    void Visit(const CallNode &call_node);
    void Visit(const IfNode &if_node);
    void Visit(const WhileNode &while_node);
    void Visit(const ReadNode &read_node);
    void Visit(const PrintNode &write_node);
};
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_DESIGN_ABSTRACTION_EXTRACTOR_H_
