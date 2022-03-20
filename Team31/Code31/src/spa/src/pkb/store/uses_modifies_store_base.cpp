#include "uses_modifies_store_base.h"

#include <cassert>

namespace spa {
UsesModifiesStoreBase::UsesModifiesStoreBase(std::size_t stmt_size,
                                             std::size_t var_size)
        : num_stmts(stmt_size),
          num_vars(var_size),
          stmt_var_(num_stmts, num_vars),
          complete_stmt_var_(num_stmts, num_vars) {}

const std::vector<int>& UsesModifiesStoreBase::GetStmtNo(int var_index) const {
    return stmt_var_.GetKeys(var_index);
}

const std::set<int>& UsesModifiesStoreBase::GetAllVar(int stmt_no) const {
    return stmt_no == 0 ? all_vars_ : complete_stmt_var_.GetVals(stmt_no);
}

const std::set<int>& UsesModifiesStoreBase::GetAllStmt(int var_index) const {
    return complete_stmt_var_.GetKeys(var_index);
}

PairVec<int> UsesModifiesStoreBase::GetStmtVar(StmtType stmt_type) const {
    if (stmt_type == StmtType::kAll) {
        return GetAllRel();
    }
    return stmt_var_pairs_[static_cast<int>(stmt_type) - 1];
}

PairVec<int> UsesModifiesStoreBase::GetAllRel() const {
    auto [stmts, vars] = PairVec<int>();
    stmts.reserve(num_rels), vars.reserve(num_rels);

    for (const auto& stmt_var_pair : stmt_var_pairs_) {
        stmts.insert(stmts.end(), stmt_var_pair.first.begin(),
                     stmt_var_pair.first.end());
        vars.insert(vars.end(), stmt_var_pair.second.begin(),
                    stmt_var_pair.second.end());
    }
    return {stmts, vars};
}

std::set<int> UsesModifiesStoreBase::GetStmt(StmtType stmt_type) const {
    return stmts_arr_[static_cast<int>(stmt_type)];
}

void UsesModifiesStoreBase::Compile(
        const TypeStatementsStore& type_statement_store,
        const ContainerForest& forest, const StmtlstParentStore& stmtlst_parent,
        const StmtlstStatementsStore& stmtlst_stmt) {
    AddAllDirectRel(type_statement_store);
    AddAllContainerRel(forest, stmtlst_parent, stmtlst_stmt,
                       type_statement_store);
    FillStmts();
    FillVars();
    FillRels();
    CalculateNumRels();
}

void UsesModifiesStoreBase::AddDirectRel(
        PairVec<int>& stmt_var_pair, const std::vector<int>& stmt_no) const {
    auto& [stmts, vars] = stmt_var_pair;
    for (auto a : stmt_no) {
        auto& var_indices = stmt_var_.GetVals(a);
        vars.insert(vars.end(), var_indices.begin(), var_indices.end());
        stmts.resize(vars.size(), a);
    }
}

void UsesModifiesStoreBase::AddIndirectRel(
        const PairVec<int>& basic_pairs,
        const StmtlstStatementsStore& stmtlst_stmt,
        const StmtlstParentStore& stmtlst_parent, const ContainerForest& forest,
        BitVec2D& if_added, BitVec2D& while_added) {
    auto& [basic_stmts, basic_vars] = basic_pairs;
    for (int i = 0; i < basic_stmts.size(); ++i) {
        auto s = basic_stmts[i], v = basic_vars[i];
        int stmtlst = stmtlst_stmt.GetStmtlst(s);
        auto ancestors = forest.GetAncestryTrace(stmtlst);
        for (auto j : ancestors) {
            auto& [type, index] = stmtlst_parent.GetParent(j);
            if (type == StmtlstParentStore::kProc) break;
            auto added =
                    type == StmtlstParentStore::kIf ? if_added : while_added;
            auto& if_var_pairs_ =
                    stmt_var_pairs_[static_cast<int>(StmtType::kIf) - 1];
            auto& while_var_pairs_ =
                    stmt_var_pairs_[static_cast<int>(StmtType::kWhile) - 1];
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

void UsesModifiesStoreBase::AddAllContainerRel(
        const ContainerForest& forest, const StmtlstParentStore& stmtlst_parent,
        const StmtlstStatementsStore& stmtlst_stmt,
        const TypeStatementsStore& type_statement_store) {
    BitVec2D if_added(num_stmts + 1, num_vars + 1);
    BitVec2D while_added(num_stmts + 1, num_vars + 1);

    AddConditionRel(forest, stmtlst_parent, stmtlst_stmt, type_statement_store,
                    if_added, while_added);

    AddAllIndirectRel(type_statement_store, stmtlst_stmt, stmtlst_parent,
                      forest, if_added, while_added);
}

void UsesModifiesStoreBase::FillStmts() {
    auto& all_stmts = stmts_arr_[static_cast<int>(StmtType::kAll)];
    for (int i = 1; i < stmts_arr_.size(); ++i) {
        auto& stmt_var_pair = stmt_var_pairs_[i - 1];
        auto& stmts = stmts_arr_[i];
        stmts.insert(stmt_var_pair.first.begin(), stmt_var_pair.first.end());
        all_stmts.insert(stmts.begin(), stmts.end());
    }
}

void UsesModifiesStoreBase::FillVars() {
    for (int i = 1; i <= num_stmts; ++i) {
        auto vars = stmt_var_.GetVals(i);
        all_vars_.insert(vars.begin(), vars.end());
    }
}

void UsesModifiesStoreBase::FillRels() {
    for (int i = 1; i < num_stmts + 1; ++i) {
        complete_stmt_var_.Set(i, stmt_var_.GetVals(i));
    }

    std::array<StmtType, 2> container_types{{StmtType::kIf, StmtType::kWhile}};
    for (const auto stmt_type : container_types) {
        auto& relationships = stmt_var_pairs_[static_cast<int>(stmt_type) - 1];
        auto& [stmts, vars] = relationships;
        for (int i = 0; i < stmts.size(); ++i) {
            complete_stmt_var_.Set(stmts[i], vars[i]);
        }
    }
}

void UsesModifiesStoreBase::CalculateNumRels() {
    std::size_t total_size = 0;
    for (const auto& stmt_var_pair : stmt_var_pairs_) {
        total_size += stmt_var_pair.first.size();
    }
    num_rels = total_size;
}
}  // namespace spa
