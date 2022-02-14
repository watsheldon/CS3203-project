#include "program_knowledge_base.h"

#include <cassert>

namespace spa {
ProgramKnowledgeBase::ProgramKnowledgeBase(
        const std::shared_ptr<BasicEntities> &init)
        : stmt_count_(init->assigns.size() + init->ifs.size() +
                      init->whiles.size() + init->calls.size() +
                      init->reads.size() + init->prints.size() -
                      STMT_TYPE_COUNT),
          stmtlst_count_(init->procedures.size() + init->whiles.size() +
                         init->ifs.size() * 2 - 4),
          proc_stmtlst_(ProcedureStmtlstStore(init->procedures.size(),
                                              stmtlst_count_)),
          while_stmtlst_(WhileStmtlstStore(stmt_count_, stmtlst_count_)),
          if_stmtlst_(IfStmtlstStore(stmt_count_, stmtlst_count_)),
          stmtlst_stmt_(StmtlstStatementsStore(stmtlst_count_, stmt_count_)),
          containers_(init->whiles.size() + init->ifs.size() - 1),
          proc_name_(ProcedureNameStore(std::move(init->procedures))),
          var_name_(VariableNameStore(std::move(init->variables))),
          const_value_(ConstantValueStore(std::move(init->constants))),
          type_stmt_(stmt_count_, std::move(init->reads),
                     std::move(init->prints), std::move(init->calls),
                     std::move(init->whiles), std::move(init->ifs),
                     std::move(init->assigns)),
          modifies_rel_(stmt_count_, init->variables.size() - 1),
          uses_rel_(stmt_count_, init->variables.size() - 1) {}

void ProgramKnowledgeBase::SetIndex(Index<EntityType::kProc> proc_index,
                                    Index<EntityType::kStmtLst> stmtlst_index) {
    assert(!compiled);
    proc_stmtlst_.Set(proc_index.value, stmtlst_index.value);
}
void ProgramKnowledgeBase::SetIndex(Index<EntityType::kStmt> while_stmt,
                                    Index<EntityType::kStmtLst> stmtlst_index) {
    assert(!compiled);
    while_stmtlst_.Set(while_stmt.value, stmtlst_index.value);
}
void ProgramKnowledgeBase::SetIndex(Index<EntityType::kStmt> if_stmt,
                                    Index<EntityType::kStmtLst> then_index,
                                    Index<EntityType::kStmtLst> else_index) {
    assert(!compiled);
    if_stmtlst_.Set(if_stmt.value, then_index.value, else_index.value);
}

void ProgramKnowledgeBase::SetLst(Index<EntityType::kStmtLst> stmtlst_index,
                                  std::vector<int> stmtlst) {
    assert(!compiled);
    stmtlst_stmt_.Set(stmtlst_index.value, std::move(stmtlst));
}

void ProgramKnowledgeBase::SetRel(Index<EntityType::kStmt> stmt_no,
                                  Index<EntityType::kVar> var_index) {
    assert(!compiled);
    modifies_rel_.Set(stmt_no.value, var_index.value);
}

void ProgramKnowledgeBase::SetRel(Index<EntityType::kStmt> stmt_no,
                                  std::vector<int> var_indices) {
    assert(!compiled);
    uses_rel_.Set(stmt_no.value, std::move(var_indices));
}

void ProgramKnowledgeBase::Compile() {
    assert(!compiled);

    compiled = true;
}

std::vector<int> ProgramKnowledgeBase::GetAllEntityIndices(EntityType et) {
    assert(compiled);
    assert(et != EntityType::kStmtLst && et != EntityType::kNotation);
    std::vector<int> results;
    switch (et) {
        case EntityType::kProc:
            results.resize(proc_name_.size());
        case EntityType::kVar:
            results.resize(var_name_.size());
        case EntityType::kConst:
            results.resize(const_value_.size());
        case EntityType::kStmt:
            results.resize(stmt_count_);
        default:
            results.resize(0);
    }
    std::iota(results.begin(), results.end(), 1);
    return results;
}
std::vector<int> ProgramKnowledgeBase::GetAllEntityIndices(StmtType st) {
    assert(compiled);
    return type_stmt_.GetStatements(st);
}

void ProgramKnowledgeBase::IndexToName(EntityType et,
                                       const std::vector<int> &index_list,
                                       std::list<std::string> &names) {
    assert(compiled);
    assert(et != EntityType::kStmtLst && et != EntityType::kNotation);
    switch (et) {
        case EntityType::kProc:
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(names),
                           [this](int i) { return proc_name_.GetName(i); });

        case EntityType::kVar:
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(names),
                           [this](int i) { return var_name_.GetName(i); });
        case EntityType::kConst:
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(names),
                           [this](int i) { return const_value_.GetValue(i); });
        default:
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(names),
                           [](int i) { return std::to_string(i); });
    }
}

}  // namespace spa
