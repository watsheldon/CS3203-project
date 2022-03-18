#include "uses_modifies.h"

#include <cassert>

namespace spa {
UsesModifies::UsesModifies(std::size_t stmt_size, std::size_t var_size)
        : num_stmts(stmt_size),
          num_vars(var_size),
          stmt_var_(num_stmts, num_vars),
          complete_stmt_var_(num_stmts, num_vars) {}

const std::vector<int>& UsesModifies::GetStmtNo(int var_index) const {
    return stmt_var_.GetKeys(var_index);
}

const std::set<int>& UsesModifies::GetAllVar(int stmt_no) const {
    return stmt_no == 0 ? all_vars_ : complete_stmt_var_.GetVals(stmt_no);
}

const std::set<int>& UsesModifies::GetAllStmt(int var_index) const {
    return complete_stmt_var_.GetKeys(var_index);
}

PairVec<int> UsesModifies::GetStmtVar(StmtType stmt_type) const {
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

PairVec<int> UsesModifies::Combine() const {
    auto [stmts, vars] = PairVec<int>();
    auto total_size =
            if_var_pairs_.first.size() + while_var_pairs_.first.size() +
            assign_var_pairs_.first.size() + read_var_pairs_.first.size() +
            print_var_pairs_.first.size() + call_var_pairs_.first.size();
    stmts.reserve(total_size), vars.reserve(total_size);

    stmts.insert(stmts.end(), if_var_pairs_.first.begin(),
                 if_var_pairs_.first.end());
    stmts.insert(stmts.end(), while_var_pairs_.first.begin(),
                 while_var_pairs_.first.end());
    stmts.insert(stmts.end(), assign_var_pairs_.first.begin(),
                 assign_var_pairs_.first.end());
    stmts.insert(stmts.end(), read_var_pairs_.first.begin(),
                 read_var_pairs_.first.end());
    stmts.insert(stmts.end(), print_var_pairs_.first.begin(),
                 print_var_pairs_.first.end());
    stmts.insert(stmts.end(), call_var_pairs_.first.begin(),
                 call_var_pairs_.first.end());

    vars.insert(vars.end(), if_var_pairs_.second.begin(),
                if_var_pairs_.second.end());
    vars.insert(vars.end(), while_var_pairs_.second.begin(),
                while_var_pairs_.second.end());
    vars.insert(vars.end(), assign_var_pairs_.second.begin(),
                assign_var_pairs_.second.end());
    vars.insert(vars.end(), read_var_pairs_.second.begin(),
                read_var_pairs_.second.end());
    vars.insert(vars.end(), print_var_pairs_.second.begin(),
                print_var_pairs_.second.end());
    vars.insert(vars.end(), call_var_pairs_.second.begin(),
                call_var_pairs_.second.end());

    return {stmts, vars};
}

std::set<int> UsesModifies::GetStmt(StmtType stmt_type) const {
    switch (stmt_type) {
        case StmtType::kAll:
            return all_stmts_;
        case StmtType::kRead:
            return read_stmts_;
        case StmtType::kPrint:
            return print_stmts_;
        case StmtType::kCall:
            return call_stmts_;
        case StmtType::kWhile:
            return while_stmts_;
        case StmtType::kIf:
            return if_stmts_;
        case StmtType::kAssign:
            return assign_stmts_;
    }
}

void UsesModifies::CompileBasic(PairVec<int>& stmt_var_pair,
                                const std::vector<int>& stmt_no) const {
    auto& [stmts, vars] = stmt_var_pair;
    for (auto a : stmt_no) {
        auto& var_indices = stmt_var_.GetVals(a);
        vars.insert(vars.end(), var_indices.begin(), var_indices.end());
        stmts.resize(vars.size(), a);
    }
}

void UsesModifies::AddAncestorsOnly(const PairVec<int>& basic_pairs,
                                    const StmtlstStatementsStore& stmtlst_stmt,
                                    const StmtlstParentStore& stmtlst_parent,
                                    const ContainerForest& forest,
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

void UsesModifies::AggregateStmts() {
    read_stmts_.insert(read_var_pairs_.first.begin(),
                       read_var_pairs_.first.end());
    all_stmts_.insert(read_stmts_.begin(), read_stmts_.end());
    print_stmts_.insert(print_var_pairs_.first.begin(),
                        print_var_pairs_.first.end());
    all_stmts_.insert(print_stmts_.begin(), print_stmts_.end());
    call_stmts_.insert(call_var_pairs_.first.begin(),
                       call_var_pairs_.first.end());
    all_stmts_.insert(call_stmts_.begin(), call_stmts_.end());
    assign_stmts_.insert(assign_var_pairs_.first.begin(),
                         assign_var_pairs_.first.end());
    all_stmts_.insert(assign_stmts_.begin(), assign_stmts_.end());
    if_stmts_.insert(if_var_pairs_.first.begin(), if_var_pairs_.first.end());
    all_stmts_.insert(if_stmts_.begin(), if_stmts_.end());
    while_stmts_.insert(while_var_pairs_.first.begin(),
                        while_var_pairs_.first.end());
    all_stmts_.insert(while_stmts_.begin(), while_stmts_.end());
}

void UsesModifies::AggregateVars() {
    for (int i = 1; i <= num_stmts; ++i) {
        auto vars = stmt_var_.GetVals(i);
        all_vars_.insert(vars.begin(), vars.end());
    }
}

void UsesModifies::UpdateStmtVar() {
    for (int i = 1; i < num_stmts + 1; ++i) {
        complete_stmt_var_.Set(i, stmt_var_.GetVals(i));
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
}  // namespace spa
