#include "program_knowledge_base.h"

#include <cassert>

namespace spa {
ProgramKnowledgeBase::ProgramKnowledgeBase(BasicEntities init)
        : stmt_count_(init.assigns.size() + init.ifs.size() +
                      init.whiles.size() + init.calls.size() +
                      init.reads.size() + init.prints.size() - STMT_TYPE_COUNT),
          stmtlst_count_(init.procedures.size() + init.whiles.size() +
                         init.ifs.size() * 2 - 4),
          proc_stmtlst_(init.procedures.size(), stmtlst_count_),
          while_stmtlst_(stmt_count_, stmtlst_count_),
          if_stmtlst_(stmt_count_, stmtlst_count_),
          stmtlst_stmt_(stmtlst_count_, stmt_count_),
          containers_(init.whiles.size() + init.ifs.size() - 1),
          proc_name_(std::move(init.procedures)),
          var_name_(std::move(init.variables)),
          const_value_(std::move(init.constants)),
          type_stmt_(stmt_count_, std::move(init.reads), std::move(init.prints),
                     std::move(init.calls), std::move(init.whiles),
                     std::move(init.ifs), std::move(init.assigns)),
          modifies_rel_(stmt_count_, init.variables.size() - 1),
          uses_rel_(stmt_count_, init.variables.size() - 1) {}

void ProgramKnowledgeBase::SetIndex(
        Index<SetEntityType::kProc> proc_index,
        Index<SetEntityType::kStmtLst> stmtlst_index) {
    assert(!compiled);
    proc_stmtlst_.Set(proc_index.value, stmtlst_index.value);
}
void ProgramKnowledgeBase::SetIndex(
        Index<SetEntityType::kStmt> while_stmt,
        Index<SetEntityType::kStmtLst> stmtlst_index) {
    assert(!compiled);
    while_stmtlst_.Set(while_stmt.value, stmtlst_index.value);
}
void ProgramKnowledgeBase::SetIndex(Index<SetEntityType::kStmt> if_stmt,
                                    Index<SetEntityType::kStmtLst> then_index,
                                    Index<SetEntityType::kStmtLst> else_index) {
    assert(!compiled);
    if_stmtlst_.Set(if_stmt.value, then_index.value, else_index.value);
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

bool ProgramKnowledgeBase::existModifies(Index<QueryEntityType::kStmt> stmt_no,
                                         Index<QueryEntityType::kVar> var_index) {
    assert(!compiled);
    return modifies_rel_.GetVarIndex(stmt_no.value) == var_index.value;
}

bool ProgramKnowledgeBase::existUses(Index<QueryEntityType::kStmt> stmt_no,
                                     Index<QueryEntityType::kVar> var_index) {
    assert(!compiled);
    return binarySearch(uses_rel_.GetVarIndex(stmt_no.value), 
                        0,
                        uses_rel_.GetVarIndex(stmt_no.value).size() - 1,
                        var_index.value);
}

int ProgramKnowledgeBase::getModifies(Index<QueryEntityType::kStmt> stmt_no,
                                      std::vector<int> filtered_var) {
    assert(!compiled);
    if (binarySearch(filtered_var,
        0,
        filtered_var.size() - 1,
        modifies_rel_.GetVarIndex(stmt_no.value))) {
        return modifies_rel_.GetVarIndex(stmt_no.value);
        }

    return 0;
}

std::vector<int> ProgramKnowledgeBase::getModifies(Index<QueryEntityType::kVar> var_index,
                                                   std::vector<int> filtered_stmt) {
    assert(!compiled);
    std::vector<int> results;

    std::set_intersection(modifies_rel_.GetStmtNo(var_index.value).begin(),
                          modifies_rel_.GetStmtNo(var_index.value).end(),
                          filtered_stmt.begin(), 
                          filtered_stmt.end(),
                          std::back_inserter(results));
    
    return results;
}

std::vector<int> ProgramKnowledgeBase::getUses(Index<QueryEntityType::kStmt> stmt_no,
                                               std::vector<int> filtered_var) {
    assert(!compiled);
    std::vector<int> results;

    std::set_intersection(uses_rel_.GetVarIndex(stmt_no.value).begin(),
                          uses_rel_.GetVarIndex(stmt_no.value).end(),
                          filtered_var.begin(),
                          filtered_var.end(),
                          std::back_inserter(results));
    return results;
}

std::vector<int> ProgramKnowledgeBase::getUses(Index<QueryEntityType::kVar> var_index,
                                                std::vector<int> filtered_stmt) {
    assert(!compiled);
    std::vector<int> results;

    std::set_intersection(uses_rel_.GetStmtNo(var_index.value).begin(),
                          uses_rel_.GetStmtNo(var_index.value).end(),
                          filtered_stmt.begin(),
                          filtered_stmt.end(),
                          std::back_inserter(results));

    return results;
}

void ProgramKnowledgeBase::Compile() {
    assert(!compiled);

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

bool ProgramKnowledgeBase::binarySearch(std::vector<int> vec, int left, int right, int x) {
    if (right >= left) {
        int mid = left + (right - left) / 2;

        if (vec[mid] == x) {
            return true;
        }
        if (vec[mid] > x) {
            return binarySearch(vec, left, mid - 1, x);
        }

        return binarySearch(vec, mid + 1, right, x);
    }
    return false;
}

}  // namespace spa
