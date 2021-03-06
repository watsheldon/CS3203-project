#include "design_abstraction_extractor.h"

#include <memory>
#include <utility>
#include <vector>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "pkb/knowledge_base.h"
#include "pkb/program_knowledge_base.h"

namespace spa {
std::unique_ptr<KnowledgeBase> DesignAbstractionExtractor::Extract(
        AST ast) noexcept {
    pkb_ = std::make_unique<ProgramKnowledgeBase>(ast->GetInitEntities());

    auto ast_root = ast->GetRoot();
    ast_root->Accept(*this);
    pkb_->Compile();
    return std::move(pkb_);
}

void DesignAbstractionExtractor::Visit(
        const ProgramNode &program_node) noexcept {
    for (auto procedure : program_node.GetProcedures()) {
        procedure->Accept(*this);
    }
}

void DesignAbstractionExtractor::Visit(
        const ProcedureNode &procedure_node) noexcept {
    // Links procedure with its statement list via their indices
    int procedure_index = procedure_node.GetIndex();
    auto stmt_lst = procedure_node.GetStmtlst();
    int stmt_lst_index = stmt_lst->GetIndex();
    pkb_->SetIndex(Index<SetEntityType::kProc>(procedure_index),
                   Index<SetEntityType::kStmtLst>(stmt_lst_index));

    stmt_lst->Accept(*this);
}

void DesignAbstractionExtractor::Visit(
        const StmtLstNode &stmt_lst_node) noexcept {
    std::vector<int> statement_indices;
    for (auto statement : stmt_lst_node.GetStatements()) {
        statement_indices.emplace_back(statement->GetIndex());
        statement->Accept(*this);
    }
    int stmt_lst_index = stmt_lst_node.GetIndex();

    // Links each StmtLst to the indices of the containing statements.
    pkb_->SetLst(Index<SetEntityType::kStmtLst>(stmt_lst_index),
                 statement_indices);
}

void DesignAbstractionExtractor::Visit(const AssignNode &assign_node) noexcept {
    int stmt_no = assign_node.GetIndex();

    // Extracts direct Modifies relationship from assignment statement.
    auto modified_var = assign_node.GetLhs();
    int var_index = modified_var->GetIndex();
    pkb_->SetRel(Index<SetEntityType::kStmt>(stmt_no),
                 Index<SetEntityType::kVar>(var_index));

    // Extracts direct Uses relationship from assignment statement.
    auto polish_notation = assign_node.GetRhs();
    std::vector<int> used_var_indices = polish_notation->GetAllVarIndices();
    pkb_->SetRel(Index<SetEntityType::kStmt>(stmt_no), used_var_indices);

    // Links statement number with polish notation index.
    int pn_index = assign_node.GetRhsIndex();
    pkb_->SetIndex(Index<SetEntityType::kStmt>(stmt_no),
                   Index<SetEntityType::kNotation>(pn_index));
}

void DesignAbstractionExtractor::Visit(const CallNode &call_node) noexcept {
    // Links call statement to the procedure it calls via their indices.
    int stmt_no = call_node.GetIndex();
    auto procedure = call_node.GetProcedure();
    int proc_index = procedure->GetIndex();
    pkb_->SetIndex(Index<SetEntityType::kStmt>(stmt_no),
                   Index<SetEntityType::kProc>(proc_index));
}

void DesignAbstractionExtractor::Visit(const IfNode &if_node) noexcept {
    IfNode::IfStmtLst if_stmt_lst = if_node.GetStmtLsts();
    auto then_lst = if_stmt_lst.then_lst;
    auto else_lst = if_stmt_lst.else_lst;

    // Links if_statement to then_stmtLst and else_stmtLst by their indices
    int then_lst_index = then_lst->GetIndex();
    int else_lst_index = else_lst->GetIndex();
    int stmt_no = if_node.GetIndex();
    pkb_->SetIndex(Index<SetEntityType::kStmt>(stmt_no),
                   Index<SetEntityType::kStmtLst>(then_lst_index),
                   Index<SetEntityType::kStmtLst>(else_lst_index));

    // Extracts direct Uses relationship from the condition in if statement.
    SetUsesFromCondition(if_node);

    then_lst->Accept(*this);
    else_lst->Accept(*this);
}

void DesignAbstractionExtractor::Visit(const WhileNode &while_node) noexcept {
    // Links while statement to its statement list by their indices.
    auto stmt_lst = while_node.GetStmtlst();
    int stmt_lst_index = stmt_lst->GetIndex();
    int stmt_no = while_node.GetIndex();
    pkb_->SetIndex(Index<SetEntityType::kStmt>(stmt_no),
                   Index<SetEntityType::kStmtLst>(stmt_lst_index));

    // Extracts direct Uses relationship from the condition in while statement.
    SetUsesFromCondition(while_node);

    stmt_lst->Accept(*this);
}

void DesignAbstractionExtractor::Visit(const ReadNode &read_node) noexcept {
    // Extracts direct Modifies relationship from read statement.
    auto modified_var = read_node.GetVariable();
    int var_index = modified_var->GetIndex();
    int stmt_no = read_node.GetIndex();
    pkb_->SetRel(Index<SetEntityType::kStmt>(stmt_no),
                 Index<SetEntityType::kVar>(var_index));
}

void DesignAbstractionExtractor::Visit(const PrintNode &print_node) noexcept {
    // Extracts direct Uses relationship from print statement.
    auto used_var = print_node.GetVariable();
    std::vector<int> used_var_index;
    used_var_index.emplace_back(used_var->GetIndex());
    int stmt_no = print_node.GetIndex();
    pkb_->SetRel(Index<SetEntityType::kStmt>(stmt_no), used_var_index);
}

void DesignAbstractionExtractor::SetUsesFromCondition(
        const IfWhileNode &if_while_node) noexcept {
    int stmt_no = if_while_node.GetIndex();
    auto condition = if_while_node.GetCondition();
    std::vector<int> used_var_indices;
    for (auto variable : condition->GetVariables()) {
        used_var_indices.emplace_back(variable->GetIndex());
    }
    pkb_->SetRel(Index<SetEntityType::kStmt>(stmt_no), used_var_indices);
}
}  // namespace spa
