#include "design_abstraction_extractor.h"

#include <memory>
#include <utility>

#include "common/entity_type_enum.h"
#include "pkb/knowledge_base.h"
#include "pkb/program_knowledge_base.h"

namespace spa {
std::unique_ptr<KnowledgeBase> DesignAbstractionExtractor::Extract(AST ast) {
    pkb_ = std::make_unique<ProgramKnowledgeBase>(ast->getInitEntities());
    ast_ = std::move(ast);
    pkb_->Compile();
    return pkb_;
}

void DesignAbstractionExtractor::Visit(const ProgramNode &program_node) {
    for (auto procedure : program_node.GetProcedures()) {
        procedure->Accept(*this);
    }
}

void DesignAbstractionExtractor::Visit(const ProcedureNode &procedure_node) {
    StmtLstNode *stmt_lst = procedure_node.GetStmtlst();
    stmt_lst->Accept(*this);
}

void DesignAbstractionExtractor::Visit(const StmtLstNode &stmt_lst_node) {
    for (auto statement : stmt_lst_node.GetStatements()) {
        statement->Accept(*this);
    }
}

void DesignAbstractionExtractor::Visit(const AssignNode &assign_node) {
    /**
     * Extracts direct Modifies relationship from assignment statement.
     */
    VariableNode *modified_var = assign_node.GetLhs();
    int stmt_no = assign_node.GetIndex();
    int var_index = modified_var->GetIndex();
    pkb_->SetRel(Index<SetEntityType::kStmt>(stmt_no),
                 Index<SetEntityType::kVar>(var_index));
}

void DesignAbstractionExtractor::Visit(const CallNode &call_node) {}

void DesignAbstractionExtractor::Visit(const IfNode &if_node) {
    IfNode::IfStmtLst if_stmt_lst = if_node.GetStmtLsts();
    if_stmt_lst.then_lst->Accept(*this);
    if_stmt_lst.else_lst->Accept(*this);
}

void DesignAbstractionExtractor::Visit(const WhileNode &while_node) {
    StmtLstNode *stmt_lst = while_node.GetStmtlst();
    stmt_lst->Accept(*this);
}

void DesignAbstractionExtractor::Visit(const ReadNode &read_node) {
    /**
     * Extracts direct Modifies relationship from read statement.
     */
    VariableNode *modified_var = read_node.GetVariable();
    int var_index = modified_var->GetIndex();
    int stmt_no = read_node.GetIndex();
    pkb_->SetRel(Index<SetEntityType::kStmt>(stmt_no),
                 Index<SetEntityType::kVar>(var_index));
}

void DesignAbstractionExtractor::Visit(const PrintNode &write_node) {}
}  // namespace spa
