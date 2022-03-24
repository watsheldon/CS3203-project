#include "uses_modifies_store_base.h"

#include <cassert>

namespace spa {
UsesModifiesStoreBase::UsesModifiesStoreBase(std::size_t stmt_size,
                                             std::size_t var_size,
                                             std::size_t proc_size)
        : num_stmts(stmt_size),
          num_vars(var_size),
          num_procs(proc_size),
          stmt_var_(num_stmts, num_vars),
          complete_stmt_var_(num_stmts, num_vars),
          proc_vars_(num_procs, num_vars) {}

const std::vector<int>& UsesModifiesStoreBase::GetStmtNo(int var_index) const {
    return stmt_var_.GetKeys(var_index);
}
const std::set<int>& UsesModifiesStoreBase::GetAllVar(int stmt_no) const {
    return stmt_no == 0 ? all_vars_ : complete_stmt_var_.GetVals(stmt_no);
}
const std::set<int>& UsesModifiesStoreBase::GetAllVarProc(
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
    return stmt_var_pairs_[ConvertStmtType(stmt_type)];
}
PairVec<int> UsesModifiesStoreBase::GetProcVar() const {
    return proc_var_pair_;
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
        const ContainerInfo& info,
        const CallsRelationshipStore& calls_rel_store) {
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
        for (auto proc_index : proc_indices) {
            auto& indirect_var_indices = proc_vars_.GetVals(proc_index);
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
                                           ContainerBitmap& bitmaps) {
    const auto& [forest, stmtlst_parent, stmtlst_stmt] = info;
    auto& [if_added, while_added, proc_added] = bitmaps;
    auto& [basic_stmts, basic_vars] = basic_pairs;
    for (int i = 0; i < basic_stmts.size(); ++i) {
        auto s = basic_stmts[i], v = basic_vars[i];
        int stmtlst = stmtlst_stmt.GetStmtlst(s);
        auto ancestors = forest.GetAncestryTrace(stmtlst);
        auto& [type, proc_index] = stmtlst_parent.GetParent(ancestors.back());
        ProcessProcedureAncestor(proc_index, v, proc_added);
        ancestors.pop_back();
        for (auto j : ancestors) {
            auto& [type, index] = stmtlst_parent.GetParent(j);
            auto added =
                    type == StmtlstParentStore::kIf ? if_added : while_added;
            auto& if_var_pairs_ =
                    stmt_var_pairs_[ConvertStmtType(StmtType::kIf)];
            auto& while_var_pairs_ =
                    stmt_var_pairs_[ConvertStmtType(StmtType::kWhile)];
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
    BitVec2D if_added(num_stmts + 1, num_vars + 1);
    BitVec2D while_added(num_stmts + 1, num_vars + 1);
    BitVec2D proc_added(num_procs + 1, num_vars + 1);
    ContainerBitmap bitmaps = {if_added, while_added, proc_added};
    const auto& [forest, stmtlst_parent, stmtlst_stmt] = info;

    AddConditionRel(type_statement_store, info, bitmaps);
    AddAllIndirectRel(type_statement_store, info, bitmaps);
    AddCallStmtVar(stmt_var_pairs_[ConvertStmtType(StmtType::kCall)],
                   type_statement_store.GetStatements(StmtType::kCall),
                   calls_rel_store);
    AddIndirectRel(stmt_var_pairs_[ConvertStmtType(StmtType::kCall)], info,
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
    for (int i = 1; i <= num_stmts; ++i) {
        auto vars = stmt_var_.GetVals(i);
        all_vars_.insert(vars.begin(), vars.end());
    }
}
void UsesModifiesStoreBase::FillRels() {
    for (int i = 1; i < num_stmts + 1; ++i) {
        complete_stmt_var_.Set(i, stmt_var_.GetVals(i));
    }
    for (const auto stmt_type : KIndirect_stmt_types_) {
        auto& relationships = stmt_var_pairs_[ConvertStmtType(stmt_type)];
        auto& [stmts, vars] = relationships;
        for (int i = 0; i < stmts.size(); ++i) {
            complete_stmt_var_.Set(stmts[i], vars[i]);
        }
    }
}
void UsesModifiesStoreBase::FillProcs() {
    for (int i = 1; i <= num_procs; ++i) {
        if (!proc_vars_.GetVals(i).empty()) {
            all_procs_.emplace(i);
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
