#include "design_abstraction_extractor.h"

#include <cassert>
#include <memory>
#include <set>
#include <utility>
#include <vector>

#include "common/entity_type_enum.h"
#include "pkb/knowledge_base.h"
#include "pkb/program_knowledge_base.h"

namespace spa {
std::unique_ptr<KnowledgeBase> DesignAbstractionExtractor::Extract(AST ast) {
    auto ast_root = ast->GetRoot();
    assert(ast_root != nullptr);
    ast_root->Accept(*this);

    pkb_ = std::make_unique<ProgramKnowledgeBase>(ast->getInitEntities());
    pkb_->Compile();
    return pkb_;
}

void DesignAbstractionExtractor::Visit(const ProgramNode &program_node) {
    for (auto procedure : program_node.GetProcedures()) {
        procedure->Accept(*this);
    }
}

void DesignAbstractionExtractor::Visit(const ProcedureNode &procedure_node) {
    // Links procedure with its statement list via their indices
    StmtLstNode *stmt_lst = procedure_node.GetStmtlst();
    int procedure_index = procedure_node.GetIndex();
    int stmt_lst_index = stmt_lst->GetIndex();
    pkb_->SetIndex(Index<SetEntityType::kProc>(procedure_index),
                   Index<SetEntityType::kStmtLst>(stmt_lst_index));

    stmt_lst->Accept(*this);
}

void DesignAbstractionExtractor::Visit(const StmtLstNode &stmt_lst_node) {
    int stmt_lst_index = stmt_lst_node.GetIndex();
    std::vector<int> statement_indices;
    for (auto statement : stmt_lst_node.GetStatements()) {
        statement_indices.emplace_back(statement->GetIndex());
        statement->Accept(*this);
    }

    // Links each StmtLst to the indices of the containing statements.
    pkb_->SetLst(Index<SetEntityType::kStmtLst>(stmt_lst_index),
                 statement_indices);
}

void DesignAbstractionExtractor::Visit(const AssignNode &assign_node) {
    int stmt_no = assign_node.GetIndex();

    // Extracts direct Modifies relationship from assignment statement.
    VariableNode *modified_var = assign_node.GetLhs();
    int var_index = modified_var->GetIndex();
    pkb_->SetRel(Index<SetEntityType::kStmt>(stmt_no),
                 Index<SetEntityType::kVar>(var_index));

    // Extracts direct Uses relationship from assignment statement.
    std::vector<int> used_var_indices =
            assign_node.GetRhs()->GetAllVarIndices();
    pkb_->SetRel(Index<SetEntityType::kStmt>(stmt_no), used_var_indices);
}

void DesignAbstractionExtractor::Visit(const CallNode &call_node) {
    // Links call statement to the procedure it calls via their indices.
    int stmt_no = call_node.GetIndex();
    auto procedure = call_node.GetProcedure();
    int procedure_index = procedure->GetIndex();
    pkb_->SetIndex(Index<SetEntityType::kStmt>(stmt_no),
                   Index<SetEntityType::kProc>(proc_index));
}

void DesignAbstractionExtractor::Visit(const IfNode &if_node) {
    int stmt_no = if_node.GetIndex();
    IfNode::IfStmtLst if_stmt_lst = if_node.GetStmtLsts();
    StmtLstNode *then_lst = if_stmt_lst.then_lst;
    StmtLstNode *else_lst = if_stmt_lst.else_lst;

    // Links if_statement to then_stmtLst and else_stmtLst by their indices
    int then_lst_index = then_lst->GetIndex();
    int else_lst_index = else_lst->GetIndex();
    pkb_->SetIndex(Index<SetEntityType::kStmt>(stmt_no),
                   Index<SetEntityType::kStmtLst>(then_lst_index),
                   Index<SetEntityType::kStmtLst>(else_lst_index));

    // Extracts direct Uses relationship from the condition in if statement.
    ConditionNode *condition = if_node.GetCondition();
    std::vector<int> used_var_indices;
    for (auto variable : condition->GetVariables()) {
        used_var_indices.emplace_back(variable->GetIndex());
    }
    pkb_->SetRel(Index<SetEntityType::kStmt>(stmt_no), used_var_indices);

    then_lst->Accept(*this);
    else_lst->Accept(*this);
}

void DesignAbstractionExtractor::Visit(const WhileNode &while_node) {
    int stmt_no = while_node.GetIndex();

    // Links while statement to its statement list by their indices.
    StmtLstNode *stmt_lst = while_node.GetStmtlst();
    int stmt_lst_index = stmt_lst->GetIndex();
    pkb_->SetIndex(Index<SetEntityType::kStmt>(stmt_no),
                   Index<SetEntityType::kStmtLst>(stmt_lst_index));

    // Extracts direct Uses relationship from the condition in while statement.
    ConditionNode *condition = while_node.GetCondition();
    std::vector<int> used_var_indices;
    for (auto variable : condition->GetVariables()) {
        used_var_indices.emplace_back(variable->GetIndex());
    }
    pkb_->SetRel(Index<SetEntityType::kStmt>(stmt_no), used_var_indices);

    stmt_lst->Accept(*this);
}

void DesignAbstractionExtractor::Visit(const ReadNode &read_node) {
    int stmt_no = read_node.GetIndex();

    // Extracts direct Modifies relationship from read statement.
    VariableNode *modified_var = read_node.GetVariable();
    int var_index = modified_var->GetIndex();
    pkb_->SetRel(Index<SetEntityType::kStmt>(stmt_no),
                 Index<SetEntityType::kVar>(var_index));
}

void DesignAbstractionExtractor::Visit(const PrintNode &print_node) {
    int stmt_no = print_node.GetIndex();

    // Extracts direct Uses relationship from print statement.
    VariableNode *used_var = print_node.GetVariable();
    std::vector<int> used_var_index;
    used_var_index.emplace_back(used_var->GetIndex());
    pkb_->SetRel(Index<SetEntityType::kStmt>(stmt_no), used_var_index);
}
}  // namespace spa
