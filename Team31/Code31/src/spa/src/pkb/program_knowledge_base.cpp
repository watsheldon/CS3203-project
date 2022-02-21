#include "program_knowledge_base.h"

#include <algorithm>
#include <cassert>
#include <list>
#include <vector>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "knowledge_base.h"
#include "secondary_structure/container_forest.h"

namespace spa {
ProgramKnowledgeBase::ProgramKnowledgeBase(BasicEntities init)
        : stmt_count_(init.assigns.size() + init.ifs.size() +
                      init.whiles.size() + init.calls.size() +
                      init.reads.size() + init.prints.size() - STMT_TYPE_COUNT),
          stmtlst_count_(init.procedures.size() + init.whiles.size() +
                         init.ifs.size() * 2 - 4),
          call_proc_(stmt_count_, init.procedures.size() - 1),
          stmtlst_parent_(init.procedures.size() - 1, stmt_count_,
                          stmtlst_count_),
          stmtlst_stmt_(stmtlst_count_, stmt_count_),
          modifies_rel_(stmt_count_, init.variables.size() - 1),
          uses_rel_(stmt_count_, init.variables.size() - 1),
          polish_notation_(stmt_count_, std::move(init.notations)),
          proc_name_(std::move(init.procedures)),
          var_name_(std::move(init.variables)),
          const_value_(std::move(init.constants)),
          type_stmt_(stmt_count_, std::move(init.reads), std::move(init.prints),
                     std::move(init.calls), std::move(init.whiles),
                     std::move(init.ifs), std::move(init.assigns)) {}

void ProgramKnowledgeBase::SetIndex(
        Index<SetEntityType::kProc> proc_index,
        Index<SetEntityType::kStmtLst> stmtlst_index) {
    assert(!compiled);
    stmtlst_parent_.Set(proc_index, stmtlst_index);
}
void ProgramKnowledgeBase::SetIndex(
        Index<SetEntityType::kStmt> while_stmt,
        Index<SetEntityType::kStmtLst> stmtlst_index) {
    assert(!compiled);
    stmtlst_parent_.Set(while_stmt, stmtlst_index);
}
void ProgramKnowledgeBase::SetIndex(Index<SetEntityType::kStmt> if_stmt,
                                    Index<SetEntityType::kStmtLst> then_index,
                                    Index<SetEntityType::kStmtLst> else_index) {
    assert(!compiled);
    stmtlst_parent_.Set(if_stmt, then_index, else_index);
}
void ProgramKnowledgeBase::SetIndex(Index<SetEntityType::kStmt> call_stmt,
                                    Index<SetEntityType::kProc> proc_index) {
    assert(!compiled);
    call_proc_.Set(call_stmt.value, proc_index.value);
}

void ProgramKnowledgeBase::SetIndex(
        Index<SetEntityType::kStmt> assign_stmt,
        Index<SetEntityType::kNotation> notation_index) {
    assert(!compiled);
    polish_notation_.Set(assign_stmt.value, notation_index.value);
}

void ProgramKnowledgeBase::SetLst(Index<SetEntityType::kStmtLst> stmtlst_index,
                                  std::vector<int> stmtlst) {
    assert(!compiled);
    stmtlst_stmt_.Set(stmtlst_index.value, std::move(stmtlst));
}

void ProgramKnowledgeBase::SetRel(Index<SetEntityType::kStmt> stmt_no,
                                  Index<SetEntityType::kVar> var_index) {
    assert(!compiled);
    modifies_rel_.Set(stmt_no.value, var_index.value);
}

void ProgramKnowledgeBase::SetRel(Index<SetEntityType::kStmt> stmt_no,
                                  std::vector<int> var_indices) {
    assert(!compiled);
    uses_rel_.Set(stmt_no.value, std::move(var_indices));
}

bool ProgramKnowledgeBase::ExistFollows(bool transitive,
                                        Index<ArgPos::kFirst> first_stmt,
                                        Index<ArgPos::kSecond> second_stmt) {
    assert(compiled);
    return stmtlst_stmt_.ExistFollows(transitive, first_stmt, second_stmt);
}
bool ProgramKnowledgeBase::ExistFollows(Index<ArgPos::kFirst> first_stmt) {
    assert(compiled);
    return stmtlst_stmt_.ExistFollows(first_stmt);
}
bool ProgramKnowledgeBase::ExistFollows(Index<ArgPos::kSecond> second_stmt) {
    assert(compiled);
    return stmtlst_stmt_.ExistFollows(second_stmt);
}
bool ProgramKnowledgeBase::ExistFollows() {
    assert(compiled);
    return stmtlst_stmt_.ExistFollows();
}

// implement after store for Parent nodes is ready
bool ProgramKnowledgeBase::ExistParent(bool transitive,
                                       Index<ArgPos::kFirst> parent_stmt,
                                       Index<ArgPos::kSecond> child_stmt) {
    assert(compiled);
    return false;
}

std::vector<int> ProgramKnowledgeBase::GetFollows(
        bool transitive, Index<ArgPos::kFirst> first_stmt,
        StmtType return_type) {
    assert(compiled);
    std::vector<int> results = stmtlst_stmt_.GetFollows(transitive, first_stmt);
    if (return_type == StmtType::kAll) {
        return results;
    }
    std::vector<int> filtered_results;
    std::copy_if(results.begin(), results.end(),
                 std::back_inserter(filtered_results),
                 [this, return_type](int i) {
                     return type_stmt_.GetType(i) == return_type;
                 });
    return filtered_results;
}

std::vector<int> ProgramKnowledgeBase::GetFollows(
        bool transitive, Index<ArgPos::kSecond> second_stmt,
        StmtType return_type) {
    assert(compiled);
    std::vector<int> results =
            stmtlst_stmt_.GetFollows(transitive, second_stmt);
    if (return_type == StmtType::kAll) {
        return results;
    }
    std::vector<int> filtered_results;
    std::copy_if(results.begin(), results.end(),
                 std::back_inserter(filtered_results),
                 [this, return_type](int i) {
                     return type_stmt_.GetType(i) == return_type;
                 });
    return filtered_results;
}

std::vector<std::pair<int, int>> ProgramKnowledgeBase::GetFollowsPairs(
        bool transitive, StmtType first_type, StmtType second_type) {
    std::vector<std::pair<int, int>> results =
            stmtlst_stmt_.GetFollowsPairs(transitive);
    if (first_type == StmtType::kAll && second_type == StmtType::kAll) {
        return results;
    }
    std::vector<std::pair<int, int>> filtered_results;
    std::copy_if(results.begin(), results.end(),
                 std::back_inserter(filtered_results),
                 [this, first_type, second_type](std::pair<int, int> p) {
                     return (first_type == StmtType::kAll ||
                             type_stmt_.GetType(p.first) == first_type) &&
                            (second_type == StmtType::kAll ||
                             type_stmt_.GetType(p.second) == second_type);
                 });
    return filtered_results;
}

// implement after store for Parent nodes is ready
std::vector<int> ProgramKnowledgeBase::GetParent(bool transitive,
                                                 Index<ArgPos::kFirst> stmt_no,
                                                 StmtType return_type) {
    assert(compiled);
    std::vector<int> results;
    return results;
}
std::vector<int> ProgramKnowledgeBase::GetParent(bool transitive,
                                                 Index<ArgPos::kSecond> stmt_no,
                                                 StmtType return_type) {
    assert(compiled);
    std::vector<int> results;
    return results;
}

bool ProgramKnowledgeBase::ExistModifies(int stmt_no, int var_index) {
    assert(compiled);
    return modifies_rel_.GetVarIndex(stmt_no) == var_index;
}

bool ProgramKnowledgeBase::ExistUses(int stmt_no, int var_index) {
    assert(compiled);
    const std::vector<int> &used_vars = uses_rel_.GetVarIndex(stmt_no);
    return std::binary_search(used_vars.begin(), used_vars.end(), var_index);
}

std::vector<int> ProgramKnowledgeBase::GetModifies(
        Index<QueryEntityType::kStmt> stmt_no,
        const std::vector<int> &filtered_var) {
    assert(compiled);
    std::vector<int> results;
    int modified_var = modifies_rel_.GetVarIndex(stmt_no.value);
    if (filtered_var.empty() ||
        std::binary_search(filtered_var.begin(), filtered_var.end(),
                           modified_var)) {
        results.emplace_back(modified_var);
    }
    return results;
}

std::vector<int> ProgramKnowledgeBase::GetModifies(
        Index<QueryEntityType::kVar> var_index,
        const std::vector<int> &filtered_stmt) {
    assert(compiled);
    auto stmts = modifies_rel_.GetStmtNo(var_index.value);
    if (filtered_stmt.empty()) {
        return stmts;
    }
    std::vector<int> results;
    std::set_intersection(stmts.begin(), stmts.end(), filtered_stmt.begin(),
                          filtered_stmt.end(), std::back_inserter(results));
    return results;
}

std::vector<int> ProgramKnowledgeBase::GetUses(
        Index<QueryEntityType::kStmt> stmt_no,
        const std::vector<int> &filtered_var) {
    assert(compiled);
    const std::vector<int> &used_vars = uses_rel_.GetVarIndex(stmt_no.value);
    if (filtered_var.empty()) {
        return used_vars;
    }
    std::vector<int> results;
    std::set_intersection(used_vars.begin(), used_vars.end(),
                          filtered_var.begin(), filtered_var.end(),
                          std::back_inserter(results));
    return results;
}

std::vector<int> ProgramKnowledgeBase::GetUses(
        Index<QueryEntityType::kVar> var_index,
        const std::vector<int> &filtered_stmt) {
    assert(compiled);
    const std::vector<int> &stmts = uses_rel_.GetStmtNo(var_index.value);
    if (filtered_stmt.empty()) {
        return stmts;
    }
    std::vector<int> results;
    std::set_intersection(stmts.begin(), stmts.end(), filtered_stmt.begin(),
                          filtered_stmt.end(), std::back_inserter(results));
    return results;
}

void ProgramKnowledgeBase::Compile() {
    assert(!compiled);
    container_forest_ = std::make_unique<ContainerForest>(
            stmtlst_parent_, stmtlst_stmt_, stmt_count_);
    compiled = true;
}

std::vector<int> ProgramKnowledgeBase::GetAllEntityIndices(QueryEntityType et) {
    assert(compiled);
    std::vector<int> results;
    switch (et) {
        case QueryEntityType::kProc:
            results.resize(proc_name_.size());
        case QueryEntityType::kVar:
            results.resize(var_name_.size());
        case QueryEntityType::kConst:
            results.resize(const_value_.size());
        case QueryEntityType::kStmt:
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

void ProgramKnowledgeBase::IndexToName(QueryEntityType et,
                                       const std::vector<int> &index_list,
                                       std::list<std::string> &names) {
    assert(compiled);
    switch (et) {
        case QueryEntityType::kProc:
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(names),
                           [this](int i) { return proc_name_.GetName(i); });

        case QueryEntityType::kVar:
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(names),
                           [this](int i) { return var_name_.GetName(i); });
        case QueryEntityType::kConst:
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
