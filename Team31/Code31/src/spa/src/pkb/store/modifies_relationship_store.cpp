#include "modifies_relationship_store.h"

#include <cassert>
#include <vector>

#include "common/bitvec2d.h"
#include "pkb/secondary_structure/container_forest.h"
#include "stmtlst_parent_store.h"
#include "stmtlst_statements_store.h"
#include "type_statements_store.h"

namespace spa {
ModifiesRelationshipStore::ModifiesRelationshipStore(size_t stmt_size,
                                                     size_t var_size)
        : num_stmts(stmt_size),
          num_vars(var_size),
          stmt_var_(num_stmts, num_vars),
          complete_stmt_var_(num_stmts, num_vars) {}

void ModifiesRelationshipStore::Set(int stmt_no, int var_index) {
    stmt_var_.Set(stmt_no, std::vector<int>{var_index});
}

const std::vector<int>& ModifiesRelationshipStore::GetStmtNo(
        int var_index) const {
    return stmt_var_.GetKeys(var_index);
}

int ModifiesRelationshipStore::GetVarIndex(int stmt_no) const {
    auto var_indices = stmt_var_.GetVals(stmt_no);
    return var_indices.empty() ? 0 : var_indices[0];
}

const std::set<int>& ModifiesRelationshipStore::GetAllVar(int stmt_no) const {
    return stmt_no == 0 ? all_vars_ : complete_stmt_var_.GetVals(stmt_no);
}

const std::set<int>& ModifiesRelationshipStore::GetAllStmt(
        int var_index) const {
    return complete_stmt_var_.GetKeys(var_index);
}

void ModifiesRelationshipStore::Compile(
        const TypeStatementsStore& type_statement_store,
        const ContainerForest& forest, const StmtlstParentStore& stmtlst_parent,
        const StmtlstStatementsStore& stmtlst_stmt) {
    AggregateVars();
    CompileBasic(assign_var_pairs_,
                 type_statement_store.GetStatements(StmtType::kAssign));
    CompileBasic(read_var_pairs_,
                 type_statement_store.GetStatements(StmtType::kRead));
    CompileContainers(forest, stmtlst_parent, stmtlst_stmt,
                      type_statement_store);
}

// Add direct Modifies of variables by assign and read.
void ModifiesRelationshipStore::CompileBasic(
        PairVec<int>& stmt_var_pair, const std::vector<int>& stmt_no) const {
    auto& [stmts, vars] = stmt_var_pair;
    for (auto a : stmt_no) {
        int var_index = GetVarIndex(a);
        vars.emplace_back(var_index);
        stmts.emplace_back(a);
    }
}

// Add indirect Modifies for container statements.
void ModifiesRelationshipStore::CompileContainers(
        const ContainerForest& forest, const StmtlstParentStore& stmtlst_parent,
        const StmtlstStatementsStore& stmtlst_stmt,
        const TypeStatementsStore& type_statement_store) {
    BitVec2D if_added(num_stmts + 1, num_vars + 1);
    BitVec2D while_added(num_stmts + 1, num_vars + 1);
    AddAncestorsOnly(assign_var_pairs_, stmtlst_stmt, stmtlst_parent, forest,
                     if_added, while_added);
    AddAncestorsOnly(read_var_pairs_, stmtlst_stmt, stmtlst_parent, forest,
                     if_added, while_added);
    AddAncestorsOnly(if_var_pairs_, stmtlst_stmt, stmtlst_parent, forest,
                     if_added, while_added);
    AddAncestorsOnly(while_var_pairs_, stmtlst_stmt, stmtlst_parent, forest,
                     if_added, while_added);
    AggregateStmts();
    UpdateStmtVar();
}

// Add container statements indirect Modifies of variables in assign, read, if
// and while.
void ModifiesRelationshipStore::AddAncestorsOnly(
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

PairVec<int> ModifiesRelationshipStore::GetStmtVar(StmtType stmt_type) const {
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

PairVec<int> ModifiesRelationshipStore::Combine() const {
    auto [stmts, vars] = PairVec<int>();
    auto total_size =
            assign_var_pairs_.first.size() + read_var_pairs_.first.size() +
            if_var_pairs_.first.size() + while_var_pairs_.first.size();
    stmts.reserve(total_size), vars.reserve(total_size);
    stmts.insert(stmts.end(), assign_var_pairs_.first.begin(),
                 assign_var_pairs_.first.end());
    stmts.insert(stmts.end(), read_var_pairs_.first.begin(),
                 read_var_pairs_.first.end());
    stmts.insert(stmts.end(), if_var_pairs_.first.begin(),
                 if_var_pairs_.first.end());
    stmts.insert(stmts.end(), while_var_pairs_.first.begin(),
                 while_var_pairs_.first.end());
    vars.insert(vars.end(), assign_var_pairs_.second.begin(),
                assign_var_pairs_.second.end());
    vars.insert(vars.end(), read_var_pairs_.second.begin(),
                read_var_pairs_.second.end());
    vars.insert(vars.end(), if_var_pairs_.second.begin(),
                if_var_pairs_.second.end());
    vars.insert(vars.end(), while_var_pairs_.second.begin(),
                while_var_pairs_.second.end());
    return {stmts, vars};
}

std::set<int> ModifiesRelationshipStore::GetStmt(StmtType stmt_type) const {
    switch (stmt_type) {
        case StmtType::kAll:
            return all_stmts_;
        case StmtType::kRead:
            return read_stmts_;
        case StmtType::kPrint:
            return {};
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

void ModifiesRelationshipStore::AggregateStmts() {
    read_stmts_.insert(read_var_pairs_.first.begin(),
                       read_var_pairs_.first.end());
    all_stmts_.insert(read_stmts_.begin(), read_stmts_.end());
    assign_stmts_.insert(assign_var_pairs_.first.begin(),
                         assign_var_pairs_.first.end());
    all_stmts_.insert(assign_stmts_.begin(), assign_stmts_.end());
    if_stmts_.insert(if_var_pairs_.first.begin(), if_var_pairs_.first.end());
    all_stmts_.insert(if_stmts_.begin(), if_stmts_.end());
    while_stmts_.insert(while_var_pairs_.first.begin(),
                        while_var_pairs_.first.end());
    all_stmts_.insert(while_stmts_.begin(), while_stmts_.end());
}

void ModifiesRelationshipStore::UpdateStmtVar() {
    for (int i = 1; i < num_stmts + 1; ++i) {
        int var_index = GetVarIndex(i);
        if (var_index == 0) {
            continue;
        }
        complete_stmt_var_.Set(i, GetVarIndex(i));
    }
    auto& [if_stmts, if_vars] = if_var_pairs_;
    for (int i = 0; i < if_stmts.size(); ++i) {
        complete_stmt_var_.Set(if_stmts[i], if_vars[i]);
    }
    auto& [while_stmts, while_vars] = while_var_pairs_;
    for (int i = 0; i < while_stmts.size(); ++i) {
        complete_stmt_var_.Set(while_stmts[i], while_vars[i]);
    }
}

void ModifiesRelationshipStore::AggregateVars() {
    for (int i = 1; i <= num_stmts; ++i) {
        auto vars = stmt_var_.GetVals(i);
        all_vars_.insert(vars.begin(), vars.end());
    }
}
}  // namespace spa
