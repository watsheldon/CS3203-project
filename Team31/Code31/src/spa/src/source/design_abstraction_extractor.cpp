#include "design_abstraction_extractor.h"

#include <memory>

#include "pkb/knowledge_base.h"
#include "pkb/program_knowledge_base.h"

namespace spa {
std::unique_ptr<KnowledgeBase> DesignAbstractionExtractor::Extract(AST ast) {
    auto pkb = std::make_unique<ProgramKnowledgeBase>(ast->getInitEntities());
    pkb->Compile();
    return pkb;
}

void DesignAbstractionExtractor::Visit(const ProgramNode &program_node) {}
void DesignAbstractionExtractor::Visit(const ProcedureNode &procedure_node) {}
void DesignAbstractionExtractor::Visit(const StmtLstNode &stmt_lst_node) {}
void DesignAbstractionExtractor::Visit(const AssignNode &assign_node) {}
void DesignAbstractionExtractor::Visit(const CallNode &call_node) {}
void DesignAbstractionExtractor::Visit(const IfNode &if_node) {}
void DesignAbstractionExtractor::Visit(const WhileNode &while_node) {}
void DesignAbstractionExtractor::Visit(const ReadNode &read_node) {}
void DesignAbstractionExtractor::Visit(const PrintNode &write_node) {}
}  // namespace spa
