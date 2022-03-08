#include "uses_relationship_store.h"

#include <cassert>
#include <vector>

#include "common/bitvec2d.h"
#include "pkb/secondary_structure/container_forest.h"
#include "stmtlst_parent_store.h"
#include "stmtlst_statements_store.h"
#include "type_statements_store.h"

namespace spa {
UsesRelationshipStore::UsesRelationshipStore(std::size_t stmt_size,
                                             std::size_t var_size)
        : num_stmts(stmt_size),
          num_vars(var_size),
          stmt_var_(num_stmts, num_vars),
          complete_stmt_var_(num_stmts, num_vars) {}

void UsesRelationshipStore::Set(int stmt_no, std::vector<int>&& var_indices) {
    stmt_var_.Set(stmt_no, std::forward<std::vector<int>>(var_indices));
}

const std::vector<int>& UsesRelationshipStore::GetVarIndex(int stmt_no) const {
    return stmt_var_.GetVals(stmt_no);
}

const std::set<int>& UsesRelationshipStore::GetAllVar(int stmt_no) const {
    return complete_stmt_var_.GetVals(stmt_no);
}
const std::vector<int>& UsesRelationshipStore::GetStmtNo(int var_index) const {
    return stmt_var_.GetKeys(var_index);
}
const std::set<int>& UsesRelationshipStore::GetAllStmt(int var_index) const {
    return complete_stmt_var_.GetKeys(var_index);
}
void UsesRelationshipStore::Compile(
        const TypeStatementsStore& type_statement_store,
        const ContainerForest& forest, const StmtlstParentStore& stmtlst_parent,
        const StmtlstStatementsStore& stmtlst_stmt) {
    CompileBasic(assign_var_pairs_,
                 type_statement_store.GetStatements(StmtType::kAssign));
    CompileBasic(print_var_pairs_,
                 type_statement_store.GetStatements(StmtType::kPrint));
    CompileContainers(forest, stmtlst_parent, stmtlst_stmt,
                      type_statement_store);
}
void UsesRelationshipStore::CompileBasic(
        PairVec<int>& stmt_var_pair, const std::vector<int>& stmt_no) const {
    auto& [stmts, vars] = stmt_var_pair;
    for (auto a : stmt_no) {
        auto& var_indices = GetVarIndex(a);
        vars.insert(vars.end(), var_indices.begin(), var_indices.end());
        stmts.resize(vars.size(), a);
    }
}
void UsesRelationshipStore::CompileContainers(
        const ContainerForest& forest, const StmtlstParentStore& stmtlst_parent,
        const StmtlstStatementsStore& stmtlst_stmt,
        const TypeStatementsStore& type_statement_store) {
    BitVec2D if_added(num_stmts, num_vars);
    BitVec2D while_added(num_stmts, num_vars);
    auto& [if_stmts, if_vars] = if_var_pairs_;
    for (auto i : type_statement_store.GetStatements(StmtType::kIf)) {
        auto& var_indices = GetVarIndex(i);
        auto stmtlst = stmtlst_stmt.GetStmtlst(i);
        auto ancestors = forest.GetAncestryTrace(stmtlst);
        for (auto a : ancestors) {
            auto& [type, index] = stmtlst_parent.GetParent(a);
            if (type == StmtlstParentStore::kProc) break;
            auto& [stmts, vars] = type == StmtlstParentStore::kIf
                                          ? if_var_pairs_
                                          : while_var_pairs_;
            auto& added =
                    type == StmtlstParentStore::kIf ? if_added : while_added;
            for (auto v : var_indices) {
                if (added.At(a, v)) continue;
                vars.emplace_back(v);
                added.Set(a, v);
            }
            if (stmts.size() == vars.size()) break;
            stmts.resize(vars.size(), index);
        }
        for (auto v : var_indices) {
            if (if_added.At(i, v)) continue;
            if_vars.emplace_back(v);
            if_added.Set(i, v);
        }
        if_stmts.resize(if_vars.size(), i);
    }
    auto& [while_stmts, while_vars] = while_var_pairs_;
    for (auto i : type_statement_store.GetStatements(StmtType::kWhile)) {
        auto& var_indices = GetVarIndex(i);
        auto stmtlst = stmtlst_stmt.GetStmtlst(i);
        auto ancestors = forest.GetAncestryTrace(stmtlst);
        for (auto a : ancestors) {
            auto& [type, index] = stmtlst_parent.GetParent(a);
            if (type == StmtlstParentStore::kProc) break;
            auto& [stmts, vars] = type == StmtlstParentStore::kIf
                                          ? if_var_pairs_
                                          : while_var_pairs_;
            auto& added =
                    type == StmtlstParentStore::kIf ? if_added : while_added;
            for (auto v : var_indices) {
                if (added.At(a, v)) continue;
                vars.emplace_back(v);
                added.Set(a, v);
            }
            if (stmts.size() == vars.size()) break;
            stmts.resize(vars.size(), index);
        }
        for (auto v : var_indices) {
            if (while_added.At(i, v)) continue;
            while_vars.emplace_back(v);
            while_added.Set(i, v);
        }
        while_stmts.resize(while_vars.size(), i);
    }
    AddAncestorsOnly(assign_var_pairs_, stmtlst_stmt, stmtlst_parent, forest,
                     if_added, while_added);
    AddAncestorsOnly(print_var_pairs_, stmtlst_stmt, stmtlst_parent, forest,
                     if_added, while_added);
    AggregateStmts();
    UpdateStmtVar();
}
PairVec<int> UsesRelationshipStore::GetStmtVar(StmtType stmt_type) const {
    switch (stmt_type) {
        case StmtType::kAll:
            return Combine();
        case StmtType::kRead:
            return read_var_pairs_;
        case StmtType::kPrint:
            return print_var_pairs_;
        case StmtType::kCall:
            return call_var_pairs_;
        case StmtType::kWhile:
            return while_var_pairs_;
        case StmtType::kIf:
            return if_var_pairs_;
        case StmtType::kAssign:
            return assign_var_pairs_;
        default:
            assert(false);
    }
}
PairVec<int> UsesRelationshipStore::Combine() const {
    auto [stmts, vars] = PairVec<int>();
    auto total_size =
            assign_var_pairs_.first.size() + print_var_pairs_.first.size() +
            if_var_pairs_.first.size() + while_var_pairs_.first.size();
    stmts.reserve(total_size), vars.reserve(total_size);
    stmts.insert(stmts.end(), assign_var_pairs_.first.begin(),
                 assign_var_pairs_.first.end());
    stmts.insert(stmts.end(), print_var_pairs_.first.begin(),
                 print_var_pairs_.first.end());
    stmts.insert(stmts.end(), if_var_pairs_.first.begin(),
                 if_var_pairs_.first.end());
    stmts.insert(stmts.end(), while_var_pairs_.first.begin(),
                 while_var_pairs_.first.end());
    vars.insert(vars.end(), assign_var_pairs_.second.begin(),
                assign_var_pairs_.second.end());
    vars.insert(vars.end(), print_var_pairs_.second.begin(),
                print_var_pairs_.second.end());
    vars.insert(vars.end(), if_var_pairs_.second.begin(),
                if_var_pairs_.second.end());
    vars.insert(vars.end(), while_var_pairs_.second.begin(),
                while_var_pairs_.second.end());
    return {stmts, vars};
}
void UsesRelationshipStore::AddAncestorsOnly(
        const PairVec<int>& basic_pairs,
        const StmtlstStatementsStore& stmtlst_stmt,
        const StmtlstParentStore& stmtlst_parent, const ContainerForest& forest,
        BitVec2D& if_added, BitVec2D& while_added) {
    auto& [basic_stmts, basic_vars] = basic_pairs;
    auto& [while_stmts, while_vars] = while_var_pairs_;
    auto& [if_stmts, if_vars] = if_var_pairs_;
    for (int i = 0; i < basic_stmts.size(); ++i) {
        auto s = basic_stmts[i], v = basic_vars[i];
        int stmtlst = stmtlst_stmt.GetStmtlst(s);
        auto ancestors = forest.GetAncestryTrace(stmtlst);
        for (auto j : ancestors) {
            auto& [type, index] = stmtlst_parent.GetParent(j);
            if (type == StmtlstParentStore::kProc) break;
            auto added =
                    type == StmtlstParentStore::kIf ? if_added : while_added;
            auto& [stmts, vars] = type == StmtlstParentStore::kIf
                                          ? if_var_pairs_
                                          : while_var_pairs_;
            if (added.At(index, v)) continue;
            vars.emplace_back(v);
            stmts.emplace_back(index);
            added.Set(index, v);
        }
    }
}
std::set<int> UsesRelationshipStore::GetStmt(StmtType stmt_type) const {
    switch (stmt_type) {
        case StmtType::kAll:
            return all_stmts_;
        case StmtType::kRead:
            return {};
        case StmtType::kPrint:
            return print_stmts_;
        case StmtType::kCall:
            return {};
        case StmtType::kWhile:
            return while_stmts_;
        case StmtType::kIf:
            return if_stmts_;
        case StmtType::kAssign:
            return assign_stmts_;
    }
}
void UsesRelationshipStore::AggregateStmts() {
    print_stmts_.insert(print_var_pairs_.first.begin(),
                        print_var_pairs_.first.end());
    all_stmts_.insert(print_stmts_.begin(), print_stmts_.end());
    assign_stmts_.insert(assign_var_pairs_.first.begin(),
                         assign_var_pairs_.first.end());
    all_stmts_.insert(assign_stmts_.begin(), assign_stmts_.end());
    if_stmts_.insert(if_var_pairs_.first.begin(), if_var_pairs_.first.end());
    all_stmts_.insert(if_stmts_.begin(), if_stmts_.end());
    while_stmts_.insert(while_var_pairs_.first.begin(),
                        while_var_pairs_.first.end());
    all_stmts_.insert(while_stmts_.begin(), while_stmts_.end());
}
void UsesRelationshipStore::UpdateStmtVar() {
    for (int i = 1; i < num_stmts + 1; ++i) {
        complete_stmt_var_.Add(i, GetVarIndex(i));
    }
    auto& [if_stmts, if_vars] = if_var_pairs_;
    for (int i = 0; i < if_stmts.size(); ++i) {
        complete_stmt_var_.Add(if_stmts[i], if_vars[i]);
    }
    auto& [while_stmts, while_vars] = while_var_pairs_;
    for (int i = 0; i < while_stmts.size(); ++i) {
        complete_stmt_var_.Add(while_stmts[i], while_vars[i]);
    }
}
}  // namespace spa
