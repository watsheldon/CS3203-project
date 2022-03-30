#include "uses_modifies_store_base.h"

#include <cassert>
#include <numeric>

namespace spa {
UsesModifiesStoreBase::UsesModifiesStoreBase(std::size_t stmt_size,
                                             std::size_t var_size,
                                             std::size_t proc_size)
        : num_stmts_(stmt_size),
          num_vars_(var_size),
          num_procs_(proc_size),
          stmt_var_(num_stmts_, num_vars_),
          complete_stmt_var_(num_stmts_, num_vars_),
          proc_vars_(num_procs_, num_vars_) {}

bool UsesModifiesStoreBase::ExistRel(int stmt_no, int var_index) const {
    assert(stmt_no != 0);
    if (stmt_no > num_stmts_) {
        return false;
    }
    auto vars = GetAllVar(stmt_no);
    return var_index == 0 ? !vars.empty() : vars.find(var_index) != vars.end();
}
bool UsesModifiesStoreBase::ExistRelP(int proc_index) const {
    return !GetVarAccessByProc(proc_index).empty();
}
bool UsesModifiesStoreBase::ExistRelP(int proc_index, int var_index) const {
    auto vars = GetVarAccessByProc(proc_index);
    return vars.find(var_index) != vars.end();
}
std::set<int> UsesModifiesStoreBase::GetRelRelatedVars(int stmt_no) const {
    return stmt_no > num_stmts_ ? std::set<int>() : GetAllVar(stmt_no);
}
std::set<int> UsesModifiesStoreBase::GetRelRelatedVars(
        int var_index, StmtType type, const TypeStatementsStore& store) const {
    assert(var_index != 0);
    auto stmts = GetAllStmt(var_index);
    if (type == StmtType::kAll) {
        return stmts;
    }
    for (auto it = stmts.begin(); it != stmts.end();) {
        if (store.GetType(*it) != type) {
            it = stmts.erase(it);
        } else {
            ++it;
        }
    }
    return stmts;
}
const std::vector<int>& UsesModifiesStoreBase::GetStmtNo(int var_index) const {
    return stmt_var_.GetKeys(var_index);
}
const std::set<int>& UsesModifiesStoreBase::GetAllVar(int stmt_no) const {
    return stmt_no == 0 ? all_vars_ : complete_stmt_var_.GetVals(stmt_no);
}
const std::set<int>& UsesModifiesStoreBase::GetVarAccessByProc(
        int proc_index) const {
    return proc_vars_.GetVals(proc_index);
}
const std::set<int>& UsesModifiesStoreBase::GetAllStmt(int var_index) const {
    return complete_stmt_var_.GetKeys(var_index);
}
const std::set<int>& UsesModifiesStoreBase::GetAllProc(int var_index) const {
    return proc_vars_.GetKeys(var_index);
}
const std::set<int>& UsesModifiesStoreBase::GetAllProc() const {
    return all_procs_;
}
PairVec<int> UsesModifiesStoreBase::GetStmtVar(StmtType stmt_type) const {
    if (stmt_type == StmtType::kAll) {
        return GetAllRel();
    }
    return stmt_var_pairs_[StmtTypeToIndex(stmt_type)];
}
PairVec<int> UsesModifiesStoreBase::GetProcVar() const {
    return proc_var_pair_;
}
PairVec<int> UsesModifiesStoreBase::GetAllRel() const {
    auto [stmts, vars] = PairVec<int>();
    stmts.reserve(num_rels_), vars.reserve(num_rels_);

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
        const ContainerInfo& info,
        const CallsRelationshipStore& calls_rel_store) {
    PrecompileStep(type_statement_store);
    const auto& [forest, stmtlst_parent, stmtlst_stmt] = info;
    AddAllDirectRel(type_statement_store);
    AddAllContainerRel(type_statement_store, info, calls_rel_store);
    FillStmts();
    FillVars();
    FillRels();
    FillProcs();
    CalculateNumRels();
}
void UsesModifiesStoreBase::AddCallStmtVar(
        PairVec<int>& stmt_var_pair, const std::vector<int>& stmt_list,
        const CallsRelationshipStore& calls_rel_store) const {
    auto& [stmts, vars] = stmt_var_pair;
    for (const auto stmt_no : stmt_list) {
        int proc_index = calls_rel_store.GetProc(stmt_no);
        auto& var_indices = proc_vars_.GetVals(proc_index);
        auto proc_indices = calls_rel_store.GetCalleeProcsT(proc_index);
        for (auto index : proc_indices) {
            auto& indirect_var_indices = proc_vars_.GetVals(index);
            vars.insert(vars.end(), indirect_var_indices.begin(),
                        indirect_var_indices.end());
            stmts.resize(vars.size(), stmt_no);
        }
        vars.insert(vars.end(), var_indices.begin(), var_indices.end());
        stmts.resize(vars.size(), stmt_no);
    }
}
void UsesModifiesStoreBase::AddDirectRel(
        PairVec<int>& stmt_var_pair, const std::vector<int>& stmt_list) const {
    auto& [stmts, vars] = stmt_var_pair;
    for (const auto stmt_no : stmt_list) {
        auto& var_indices = stmt_var_.GetVals(stmt_no);
        vars.insert(vars.end(), var_indices.begin(), var_indices.end());
        stmts.resize(vars.size(), stmt_no);
    }
}
void UsesModifiesStoreBase::AddIndirectRel(const PairVec<int>& basic_pairs,
                                           const ContainerInfo& info,
                                           ContainerAddedTrackers& bitmaps) {
    const auto& [forest, stmtlst_parent, stmtlst_stmt] = info;
    auto& [if_added, while_added, proc_added] = bitmaps;
    auto& [basic_stmts, basic_vars] = basic_pairs;
    for (int i = 0; i < basic_stmts.size(); ++i) {
        auto s = basic_stmts[i], v = basic_vars[i];
        int stmtlst = stmtlst_stmt.GetStmtlst(s);
        auto ancestors = forest.GetAncestryTrace(stmtlst);
        int proc_index = stmtlst_parent.GetParent(ancestors.back()).index;
        ProcessProcedureAncestor(proc_index, v, proc_added);
        ancestors.pop_back();
        for (auto j : ancestors) {
            auto& [type, index] = stmtlst_parent.GetParent(j);
            auto added =
                    type == StmtlstParentStore::kIf ? if_added : while_added;
            auto& if_var_pairs_ =
                    stmt_var_pairs_[StmtTypeToIndex(StmtType::kIf)];
            auto& while_var_pairs_ =
                    stmt_var_pairs_[StmtTypeToIndex(StmtType::kWhile)];
            auto& [stmts, vars] = type == StmtlstParentStore::kIf
                                          ? if_var_pairs_
                                          : while_var_pairs_;
            if (added.At(index, v)) break;
            vars.emplace_back(v);
            stmts.emplace_back(index);
            added.Set(index, v);
        }
    }
}
void UsesModifiesStoreBase::AddAllContainerRel(
        const TypeStatementsStore& type_statement_store,
        const ContainerInfo& info,
        const CallsRelationshipStore& calls_rel_store) {
    BitVec2D if_added(num_stmts_ + 1, num_vars_ + 1);
    BitVec2D while_added(num_stmts_ + 1, num_vars_ + 1);
    BitVec2D proc_added(num_procs_ + 1, num_vars_ + 1);
    ContainerAddedTrackers bitmaps = {if_added, while_added, proc_added};
    const auto& [forest, stmtlst_parent, stmtlst_stmt] = info;
    AuxiliaryData data_store{type_statement_store, info, bitmaps};
    AddConditionRel(data_store);
    AddAllIndirectRel(data_store);
    AddCallStmtVar(stmt_var_pairs_[StmtTypeToIndex(StmtType::kCall)],
                   type_statement_store.GetStatements(StmtType::kCall),
                   calls_rel_store);
    AddIndirectRel(stmt_var_pairs_[StmtTypeToIndex(StmtType::kCall)], info,
                   bitmaps);
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
    for (int i = 1; i <= num_stmts_; ++i) {
        auto vars = stmt_var_.GetVals(i);
        all_vars_.insert(vars.begin(), vars.end());
    }
}
void UsesModifiesStoreBase::FillRels() {
    for (int i = 1; i < num_stmts_ + 1; ++i) {
        complete_stmt_var_.Set(i, stmt_var_.GetVals(i));
    }
    for (const auto stmt_type : kIndirectStmtTypes) {
        auto& relationships = stmt_var_pairs_[StmtTypeToIndex(stmt_type)];
        auto& [stmts, vars] = relationships;
        for (int i = 0; i < stmts.size(); ++i) {
            complete_stmt_var_.Set(stmts[i], vars[i]);
        }
    }
}
void UsesModifiesStoreBase::FillProcs() {
    for (int i = 1; i <= num_procs_; ++i) {
        if (!proc_vars_.GetVals(i).empty()) {
            all_procs_.emplace(i);
        }
    }
}
void UsesModifiesStoreBase::CalculateNumRels() {
    num_rels_ = std::transform_reduce(
            stmt_var_pairs_.begin(), stmt_var_pairs_.end(), 0, std::plus<>(),
            [](PairVec<int>& pair) { return pair.first.size(); });
}
void UsesModifiesStoreBase::ProcessProcedureAncestor(int proc_index,
                                                     int var_index,
                                                     BitVec2D& proc_added) {
    if (proc_added.At(proc_index, var_index)) return;
    proc_added.Set(proc_index, var_index);
    proc_vars_.Set(proc_index, var_index);
    auto& [procs, vars] = proc_var_pair_;
    vars.emplace_back(var_index);
    procs.emplace_back(proc_index);
}
void UsesModifiesStoreBase::PrecompileStep(
        const TypeStatementsStore& type_store) {}
void UsesModifiesStoreBase::AddConditionRel(
        const UsesModifiesStoreBase::AuxiliaryData& data_store) {}
}  // namespace spa
