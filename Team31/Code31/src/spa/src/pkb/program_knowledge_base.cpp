#include "program_knowledge_base.h"

#include <algorithm>
#include <cassert>
#include <list>
#include <set>
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

bool ProgramKnowledgeBase::ExistParent(bool transitive,
                                       Index<ArgPos::kFirst> parent_stmt,
                                       Index<ArgPos::kSecond> child_stmt) {
    assert(compiled);
    int parent = parent_stmt.value;
    int child = child_stmt.value;
    if (child >= parent || parent > stmt_count_ - 1 || child > stmt_count_) {
        return false;
    }
    if (type_stmt_.GetType(parent) != StmtType::kWhile ||
        type_stmt_.GetType(parent) != StmtType::kIf) {
        return false;
    }

    if (transitive) {
        std::vector<int> parent_follower =
                stmtlst_stmt_.GetFollows(false, parent_stmt);
        return parent_follower.empty() || child_stmt.value < parent_follower[0];
    }
    int stmtlst_index = stmtlst_stmt_.GetStmtlst(child);
    return stmtlst_parent_.GetWhileStmtLst(parent) == stmtlst_index ||
           stmtlst_parent_.GetIfStmtLst(parent).then_index == stmtlst_index ||
           stmtlst_parent_.GetIfStmtLst(parent).else_index == stmtlst_index;
}
bool ProgramKnowledgeBase::ExistParent(Index<ArgPos::kFirst> parent_stmt) {
    assert(compiled);
    return parent_stmt.value < stmt_count_ &&
           (type_stmt_.GetType(parent_stmt.value) == StmtType::kIf ||
            type_stmt_.GetType(parent_stmt.value) == StmtType::kWhile);
}
bool ProgramKnowledgeBase::ExistParent(Index<ArgPos::kSecond> child_stmt) {
    assert(compiled);
    int stmtlst_index = stmtlst_stmt_.GetStmtlst(child_stmt.value);
    return child_stmt.value <= stmt_count_ &&
           (stmtlst_parent_.GetParent(stmtlst_index).type == PType::kIf ||
            stmtlst_parent_.GetParent(stmtlst_index).type == PType::kWhile);
}
bool ProgramKnowledgeBase::ExistParent() {
    assert(compiled);
    return !type_stmt_.GetStatements(StmtType::kWhile).empty() ||
           !type_stmt_.GetStatements(StmtType::kIf).empty();
}

std::set<int> ProgramKnowledgeBase::GetFollows(ArgPos return_pos,
                                               StmtType return_type) {
    auto results = return_pos == ArgPos::kFirst
                           ? stmtlst_stmt_.GetFollowedByWildcard()
                           : stmtlst_stmt_.GetFollowsWildcard();
    std::set<int> filtered_results;
    std::copy_if(results.begin(), results.end(),
                 std::inserter(filtered_results, filtered_results.end()),
                 [this, return_type](int i) {
                     return type_stmt_.GetType(i) == return_type;
                 });
    return filtered_results;
}
std::set<int> ProgramKnowledgeBase::GetFollows(bool transitive,
                                               Index<ArgPos::kFirst> first_stmt,
                                               StmtType return_type) {
    assert(compiled);
    assert(first_stmt.value > 0);
    std::vector<int> results = stmtlst_stmt_.GetFollows(transitive, first_stmt);
    if (return_type == StmtType::kAll) {
        return {results.begin(), results.end()};
    }
    std::set<int> filtered_results;
    std::copy_if(results.begin(), results.end(),
                 std::inserter(filtered_results, filtered_results.end()),
                 [this, return_type](int i) {
                     return type_stmt_.GetType(i) == return_type;
                 });
    return filtered_results;
}

std::set<int> ProgramKnowledgeBase::GetFollows(
        bool transitive, Index<ArgPos::kSecond> second_stmt,
        StmtType return_type) {
    assert(compiled);
    assert(second_stmt.value > 0);
    std::vector<int> results =
            stmtlst_stmt_.GetFollows(transitive, second_stmt);
    if (return_type == StmtType::kAll) {
        return {results.begin(), results.end()};
    }
    std::set<int> filtered_results;
    std::copy_if(results.begin(), results.end(),
                 std::inserter(filtered_results, filtered_results.end()),
                 [this, return_type](int i) {
                     return type_stmt_.GetType(i) == return_type;
                 });
    return filtered_results;
}

std::pair<std::vector<int>, std::vector<int>>
ProgramKnowledgeBase::GetFollowsPairs(bool transitive, StmtType first_type,
                                      StmtType second_type) {
    assert(compiled);
    std::pair<std::vector<int>, std::vector<int>> results =
            stmtlst_stmt_.GetFollowsPairs(transitive);
    if (first_type == StmtType::kAll && second_type == StmtType::kAll) {
        return results;
    }
    std::pair<std::vector<int>, std::vector<int>> filtered_results;
    for (int i = 0; i < results.first.size(); ++i) {
        if ((first_type == StmtType::kAll ||
             type_stmt_.GetType(results.first[i]) == first_type) &&
            (second_type == StmtType::kAll ||
             type_stmt_.GetType(results.second[i]) == second_type)) {
            filtered_results.first.emplace_back(results.first[i]);
            filtered_results.second.emplace_back(results.second[i]);
        }
    }
    return filtered_results;
}

std::set<int> ProgramKnowledgeBase::GetAllParents(StmtType return_type) {
    std::vector<int> results;
    if (return_type == StmtType::kAll) {
        results = type_stmt_.GetStatements(StmtType::kWhile);
        std::vector<int> if_stmts = type_stmt_.GetStatements(StmtType::kIf);
        results.insert(results.end(), if_stmts.begin(), if_stmts.end());
    } else {
        results = type_stmt_.GetStatements(return_type);
    }
    return {results.begin(), results.end()};
}
std::set<int> ProgramKnowledgeBase::GetAllChildren(StmtType return_type) {
    if (return_type == StmtType::kAll) {
        std::vector<int> results;
        for (auto &while_stmt : type_stmt_.GetStatements(StmtType::kWhile)) {
            int stmtlst_index = stmtlst_parent_.GetWhileStmtLst(while_stmt);
            std::vector<int> stmtlst =
                    stmtlst_stmt_.GetStatements(stmtlst_index);
            results.insert(results.end(), stmtlst.begin(), stmtlst.end());
        }
        for (auto &if_stmt : type_stmt_.GetStatements(StmtType::kIf)) {
            int then_index = stmtlst_parent_.GetIfStmtLst(if_stmt).then_index;
            int else_index = stmtlst_parent_.GetIfStmtLst(if_stmt).else_index;
            std::vector<int> then_stmtlst =
                    stmtlst_stmt_.GetStatements(then_index);
            std::vector<int> else_stmtlst =
                    stmtlst_stmt_.GetStatements(else_index);
            results.insert(results.end(), then_stmtlst.begin(),
                           then_stmtlst.end());
            results.insert(results.end(), else_stmtlst.begin(),
                           else_stmtlst.end());
        }
        return {results.begin(), results.end()};
    }
    std::vector<int> results = type_stmt_.GetStatements(return_type);
    std::vector<int> filtered_results;
    for (auto &i : results) {
        int stmtlst_index = stmtlst_stmt_.GetStmtlst(i);
        PType type = stmtlst_parent_.GetParent(stmtlst_index).type;
        if (type != PType::kIf && type != PType::kWhile) continue;
        filtered_results.emplace_back(i);
    }
    return {filtered_results.begin(), filtered_results.end()};
}
std::set<int> ProgramKnowledgeBase::GetParent(ArgPos return_pos,
                                              StmtType return_type) {
    if (return_pos == ArgPos::kFirst) {
        return GetAllParents(return_type);
    }
    return GetAllChildren(return_type);
}
void ProgramKnowledgeBase::GetNonTransitiveParentFirst(
        StmtType parent_type, int parent, std::vector<int> &results) const {
    if (parent_type == StmtType::kWhile) {
        int stmtlst_index = stmtlst_parent_.GetWhileStmtLst(parent);
        results = stmtlst_stmt_.GetStatements(stmtlst_index);
    }
    int then_index = stmtlst_parent_.GetIfStmtLst(parent).then_index;
    int else_index = stmtlst_parent_.GetIfStmtLst(parent).else_index;
    results = stmtlst_stmt_.GetStatements(then_index);
    std::vector<int> else_vector = stmtlst_stmt_.GetStatements(else_index);
    results.insert(results.end(), else_vector.begin(), else_vector.end());
}
void ProgramKnowledgeBase::GetTransitiveParentFirst(
        std::vector<int> parent_follower, int parent,
        std::vector<int> &results) const {
    int size;
    if (parent_follower.empty()) {
        size = static_cast<int>(stmt_count_) - parent;
    } else {
        size = parent_follower[0] - parent - 1;
    }
    results.resize(size);
    std::iota(results.begin(), results.end(), parent + 1);
}
std::set<int> ProgramKnowledgeBase::GetParent(bool transitive,
                                              Index<ArgPos::kFirst> parent_stmt,
                                              StmtType return_type) {
    assert(compiled);
    assert(parent_stmt.value > 0);
    if (parent_stmt.value > stmt_count_ - 1) {
        return {};
    }
    StmtType parent_type = type_stmt_.GetType(parent_stmt.value);
    if (parent_type != StmtType::kWhile && parent_type != StmtType::kIf) {
        return {};
    }

    std::vector<int> results;
    if (transitive) {
        GetTransitiveParentFirst(stmtlst_stmt_.GetFollows(false, parent_stmt),
                                 parent_stmt.value, results);
    } else {
        GetNonTransitiveParentFirst(parent_type, parent_stmt.value, results);
    }
    if (return_type == StmtType::kAll) {
        return {results.begin(), results.end()};
    }
    std::vector<int> filtered_results;
    std::copy_if(results.begin(), results.end(),
                 std::back_inserter(filtered_results),
                 [this, return_type](int i) {
                     return type_stmt_.GetType(i) == return_type;
                 });
    return {filtered_results.begin(), filtered_results.end()};
}
std::set<int> ProgramKnowledgeBase::GetParent(bool transitive,
                                              Index<ArgPos::kSecond> child_stmt,
                                              StmtType return_type) {
    assert(compiled);
    assert(child_stmt.value > 0);
    assert(return_type == StmtType::kAll || return_type == StmtType::kWhile ||
           return_type == StmtType::kIf);
    if (child_stmt.value > stmt_count_) {
        return {};
    }

    int stmtlst_index = stmtlst_stmt_.GetStmtlst(child_stmt.value);
    PType type = stmtlst_parent_.GetParent(stmtlst_index).type;
    if (type != PType::kIf && type != PType::kWhile) {
        return {};
    }
    std::vector<int> results;
    int direct_parent = stmtlst_parent_.GetParent(stmtlst_index).index;
    if (transitive) {
        std::vector<int> stmtlsts =
                container_forest_->GetParents(stmtlst_index);
        for (auto &i : stmtlsts) {
            if (stmtlst_parent_.GetParent(i).type != PType::kWhile &&
                stmtlst_parent_.GetParent(i).type != PType::kIf)
                continue;
            int indirect_parent = stmtlst_parent_.GetParent(i).index;
            results.emplace_back(indirect_parent);
        }
        results.emplace_back(direct_parent);
    } else {
        results = {direct_parent};
    }

    if (return_type == StmtType::kAll) {
        return {results.begin(), results.end()};
    }
    std::vector<int> filtered_results;
    std::copy_if(results.begin(), results.end(),
                 std::back_inserter(filtered_results),
                 [this, return_type](int i) {
                     return type_stmt_.GetType(i) == return_type;
                 });
    return {filtered_results.begin(), filtered_results.end()};
}

void ProgramKnowledgeBase::GetTransitiveParentPairs(
        std::pair<std::vector<int>, std::vector<int>> &results) {
    for (auto &while_stmt : type_stmt_.GetStatements(StmtType::kWhile)) {
        std::vector<int> follower = stmtlst_stmt_.GetFollows(
                false, Index<ArgPos::kFirst>(while_stmt));
        int end;
        (follower.empty()) ? (end = static_cast<int>(stmt_count_) + 1)
                           : (end = follower[0]);
        for (int i = while_stmt + 1; i < end; ++i) {
            results.first.emplace_back(while_stmt);
            results.second.emplace_back(i);
        }
    }
    for (auto &if_stmt : type_stmt_.GetStatements(StmtType::kIf)) {
        std::vector<int> follower =
                stmtlst_stmt_.GetFollows(false, Index<ArgPos::kFirst>(if_stmt));
        int end;
        (follower.empty()) ? (end = static_cast<int>(stmt_count_) + 1)
                           : (end = follower[0]);
        for (int i = if_stmt + 1; i < end; ++i) {
            results.first.emplace_back(if_stmt);
            results.second.emplace_back(i);
        }
    }
}
void ProgramKnowledgeBase::GetNonTransitiveParentPairs(
        std::pair<std::vector<int>, std::vector<int>> &results) {
    for (auto &while_stmt : type_stmt_.GetStatements(StmtType::kWhile)) {
        int stmtlst_index = stmtlst_parent_.GetWhileStmtLst(while_stmt);
        std::vector<int> stmtlst = stmtlst_stmt_.GetStatements(stmtlst_index);
        for (auto &child_stmt : stmtlst) {
            results.first.emplace_back(while_stmt);
            results.second.emplace_back(child_stmt);
        }
    }
    for (auto &if_stmt : type_stmt_.GetStatements(StmtType::kIf)) {
        int then_index = stmtlst_parent_.GetIfStmtLst(if_stmt).then_index;
        int else_index = stmtlst_parent_.GetIfStmtLst(if_stmt).else_index;
        std::vector<int> then_stmtlst = stmtlst_stmt_.GetStatements(then_index);
        std::vector<int> else_stmtlst = stmtlst_stmt_.GetStatements(else_index);
        for (auto &child_stmt : then_stmtlst) {
            results.first.emplace_back(if_stmt);
            results.second.emplace_back(child_stmt);
        }
        for (auto &child_stmt : else_stmtlst) {
            results.first.emplace_back(if_stmt);
            results.second.emplace_back(child_stmt);
        }
    }
}
std::pair<std::vector<int>, std::vector<int>>
ProgramKnowledgeBase::GetParentPairs(bool transitive, StmtType parent_type,
                                     StmtType child_type) {
    std::pair<std::vector<int>, std::vector<int>> results;
    if (transitive) {
        GetTransitiveParentPairs(results);
    } else {
        GetNonTransitiveParentPairs(results);
    }
    if (parent_type == StmtType::kAll && child_type == StmtType::kAll) {
        return results;
    }
    std::pair<std::vector<int>, std::vector<int>> filtered_results;
    for (int i = 0; i < results.first.size(); ++i) {
        if ((parent_type == StmtType::kAll ||
             type_stmt_.GetType(results.first[i]) == parent_type) &&
            (child_type == StmtType::kAll ||
             type_stmt_.GetType(results.second[i]) == child_type)) {
            filtered_results.first.emplace_back(results.first[i]);
            filtered_results.second.emplace_back(results.second[i]);
        }
    }
    return filtered_results;
}

bool ProgramKnowledgeBase::ExistModifies(int stmt_no, int var_index) {
    assert(compiled);
    assert(stmt_no != 0);
    StmtType type = type_stmt_.GetType(stmt_no);

    if (type == StmtType::kPrint || type == StmtType::kCall) {
        return false;
    }

    if (type == StmtType::kAssign || type == StmtType::kRead) {
        return var_index == 0 ||
               var_index == modifies_rel_.GetVarIndex(stmt_no);
    }

    auto first_stmt = stmt_no + 1;
    int last_stmt;

    auto follow_vec =
            GetFollows(false, Index<ArgPos::kFirst>(stmt_no), StmtType::kAll);

    if (follow_vec.size() == 1) {
        last_stmt = *follow_vec.begin();
    } else {
        int stmtlst =
                type == StmtType::kWhile
                        ? stmtlst_parent_.GetWhileStmtLst(stmt_no)
                        : stmtlst_parent_.GetIfStmtLst(stmt_no).else_index;
        int grandchild = container_forest_->GetRightmostGrandchild(stmtlst);
        auto grandchild_stmt = stmtlst_stmt_.GetStatements(grandchild);
        last_stmt = grandchild_stmt.back();
    }

    if (var_index == 0) {
        const auto &assign_stmts = type_stmt_.GetStatements(StmtType::kAssign);
        auto first_assign = std::lower_bound(assign_stmts.begin(),
                                             assign_stmts.end(), first_stmt);
        if (first_assign != assign_stmts.end() && *first_assign <= last_stmt) {
            return true;
        }

        const auto &read_stmts = type_stmt_.GetStatements(StmtType::kRead);
        auto first_read = std::lower_bound(read_stmts.begin(), read_stmts.end(),
                                           first_stmt);
        if (first_read != read_stmts.end() && *first_read <= last_stmt) {
            return true;
        }

        return false;
    }

    auto modifying_stmt = modifies_rel_.GetStmtNo(var_index);
    return std::any_of(modifying_stmt.begin(), modifying_stmt.end(),
                       [first_stmt, last_stmt](auto i) {
                           return first_stmt <= i && i <= last_stmt;
                       });
}

bool ProgramKnowledgeBase::ExistUses(int stmt_no, int var_index) {
    assert(compiled);
    assert(stmt_no != 0);

    StmtType type = type_stmt_.GetType(stmt_no);

    if (type == StmtType::kRead || type == StmtType::kCall) {
        return false;
    }

    if (type == StmtType::kAssign || type == StmtType::kPrint) {
        std::vector<int> var_indices;
        var_indices = uses_rel_.GetVarIndex(stmt_no);
        if (var_index == 0) {
            return var_indices.size() > 0;
        }

        return std::binary_search(var_indices.begin(), var_indices.end(),
                                  var_index);
    }

    auto first_stmt = stmt_no + 1;
    int last_stmt;

    auto follow_vec =
            GetFollows(false, Index<ArgPos::kFirst>(stmt_no), StmtType::kAll);

    if (follow_vec.size() == 1) {
        last_stmt = *follow_vec.begin();
    } else {
        int stmtlst =
                type == StmtType::kWhile
                        ? stmtlst_parent_.GetWhileStmtLst(stmt_no)
                        : stmtlst_parent_.GetIfStmtLst(stmt_no).else_index;
        int grandchild = container_forest_->GetRightmostGrandchild(stmt_no);
        auto grandchild_stmt = stmtlst_stmt_.GetStatements(grandchild);
        last_stmt = grandchild_stmt.back();
    }

    if (var_index == 0) {
        const auto &assign_stmts = type_stmt_.GetStatements(StmtType::kAssign);
        auto first_assign = std::lower_bound(assign_stmts.begin(),
                                             assign_stmts.end(), first_stmt);
        if (first_assign != assign_stmts.end() && *first_assign <= last_stmt) {
            return true;
        }

        const auto &print_stmts = type_stmt_.GetStatements(StmtType::kPrint);
        auto first_print = std::lower_bound(print_stmts.begin(),
                                            print_stmts.end(), first_stmt);
        if (first_print != print_stmts.end() && *first_print <= last_stmt) {
            return true;
        }

        return false;
    }

    auto using_stmt = uses_rel_.GetStmtNo(var_index);
    return std::any_of(using_stmt.begin(), using_stmt.end(),
                       [first_stmt, last_stmt](auto i) {
                           return first_stmt <= i && i <= last_stmt;
                       });
}

std::set<int> ProgramKnowledgeBase::GetModifies(
        Index<QueryEntityType::kStmt> stmt_no) {
    assert(compiled);
    assert(stmt_no.value != 0);

    StmtType type = type_stmt_.GetType(stmt_no.value);

    if (type == StmtType::kPrint || type == StmtType::kCall) {
        return {};
    }

    if (type == StmtType::kAssign || type == StmtType::kRead) {
        return {modifies_rel_.GetVarIndex(stmt_no.value)};
    }

    auto first_stmt = stmt_no.value + 1;
    int last_stmt;

    auto follow_vec = GetFollows(false, Index<ArgPos::kFirst>(stmt_no.value),
                                 StmtType::kAll);

    if (follow_vec.size() == 1) {
        last_stmt = *follow_vec.begin();
    } else {
        int stmtlst = type == StmtType::kWhile
                              ? stmtlst_parent_.GetWhileStmtLst(stmt_no.value)
                              : stmtlst_parent_.GetIfStmtLst(stmt_no.value)
                                        .else_index;
        int grandchild = container_forest_->GetRightmostGrandchild(stmtlst);
        auto grandchild_stmt = stmtlst_stmt_.GetStatements(grandchild);
        last_stmt = grandchild_stmt.back();
    }

    const auto &assign_stmts = type_stmt_.GetStatements(StmtType::kAssign);
    auto first_assign = std::lower_bound(assign_stmts.begin(),
                                         assign_stmts.end(), first_stmt);

    auto second_assign =
            std::upper_bound(first_assign, assign_stmts.end(), last_stmt);

    std::set<int> results;

    for (auto i = first_assign; i != second_assign; i++) {
        results.emplace(modifies_rel_.GetVarIndex(*i));
    }

    const auto &read_stmts = type_stmt_.GetStatements(StmtType::kRead);
    auto first_read =
            std::lower_bound(read_stmts.begin(), read_stmts.end(), first_stmt);
    auto second_read =
            std::upper_bound(first_read, read_stmts.end(), last_stmt);

    for (auto i = first_read; i != second_read; i++) {
        results.emplace(modifies_rel_.GetVarIndex(*i));
    }

    return results;
}

std::set<int> ProgramKnowledgeBase::GetModifies(
        Index<QueryEntityType::kVar> var_index, StmtType type) {
    assert(compiled);
    assert(var_index.value != 0);

    if (type == StmtType::kPrint || type == StmtType::kCall) {
        return {};
    }

    auto direct_modifying_stmt = modifies_rel_.GetStmtNo(var_index.value);

    if (type == StmtType::kAssign) {
        std::set<int> assign_stmt;
        std::copy_if(direct_modifying_stmt.begin(), direct_modifying_stmt.end(),
                     std::inserter(assign_stmt, assign_stmt.begin()),
                     [this](int i) {
                         return type_stmt_.GetType(i) == StmtType::kAssign;
                     });
        return assign_stmt;
    }

    if (type == StmtType::kRead) {
        std::set<int> read_stmt;
        std::copy_if(direct_modifying_stmt.begin(), direct_modifying_stmt.end(),
                     std::inserter(read_stmt, read_stmt.begin()),
                     [this](int i) {
                         return type_stmt_.GetType(i) == StmtType::kRead;
                     });
        return read_stmt;
    }

    std::set<int> container_stmt;
    std::vector<int> parents;

    for (auto &i : direct_modifying_stmt) {
        int stmtlst = stmtlst_stmt_.GetStmtlst(i);
        parents = container_forest_->GetParents(stmtlst);
        parents.emplace_back(stmtlst);

        for (int i = 0; i < parents.size(); i++) {
            auto stmt = stmtlst_parent_.GetParent(i);
            if (stmt.type != PType::kWhile && stmt.type != PType::kIf) {
                continue;
            }
            container_stmt.emplace(stmt.index);
        }
    }

    if (type == StmtType::kAll) {
        container_stmt.insert(direct_modifying_stmt.begin(),
                              direct_modifying_stmt.end());
        return container_stmt;
    }

    if (type == StmtType::kIf) {
        std::set<int> if_stmt;
        std::copy_if(container_stmt.begin(), container_stmt.end(),
                     std::inserter(if_stmt, if_stmt.begin()), [this](int i) {
                         return type_stmt_.GetType(i) == StmtType::kIf;
                     });
        return if_stmt;
    }

    assert(type == StmtType::kWhile);

    std::set<int> while_stmt;
    std::copy_if(container_stmt.begin(), container_stmt.end(),
                 std::inserter(while_stmt, while_stmt.begin()), [this](int i) {
                     return type_stmt_.GetType(i) == StmtType::kWhile;
                 });

    return while_stmt;
}

std::set<int> ProgramKnowledgeBase::GetModifies(StmtType type) {
    assert(compiled);

    if (type == StmtType::kPrint || type == StmtType::kCall) {
        return {};
    }

    if (type == StmtType::kAssign) {
        auto assign_stmt = type_stmt_.GetStatements(StmtType::kAssign);
        return {assign_stmt.begin(), assign_stmt.end()};
    }

    if (type == StmtType::kRead) {
        auto read_stmt = type_stmt_.GetStatements(StmtType::kRead);
        return {read_stmt.begin(), read_stmt.end()};
    }

    std::set<int> container_stmt;
    std::vector<int> parents, direct_modifying_stmt;

    direct_modifying_stmt = type_stmt_.GetStatements(StmtType::kAssign);
    direct_modifying_stmt.insert(
            direct_modifying_stmt.end(),
            type_stmt_.GetStatements(StmtType::kRead).begin(),
            type_stmt_.GetStatements(StmtType::kRead).end());

    for (auto &i : direct_modifying_stmt) {
        int stmtlst = stmtlst_stmt_.GetStmtlst(i);
        parents = container_forest_->GetParents(stmtlst);
        parents.emplace_back(stmtlst);

        for (int i = 0; i < parents.size(); i++) {
            auto stmt = stmtlst_parent_.GetParent(i);
            if (stmt.type != PType::kWhile && stmt.type != PType::kIf) {
                continue;
            }
            container_stmt.emplace(stmt.index);
        }
    }

    if (type == StmtType::kAll) {
        container_stmt.insert(direct_modifying_stmt.begin(),
                              direct_modifying_stmt.end());
        return container_stmt;
    }

    if (type == StmtType::kIf) {
        std::set<int> if_stmt;
        std::copy_if(container_stmt.begin(), container_stmt.end(),
                     std::inserter(if_stmt, if_stmt.begin()), [this](int i) {
                         return type_stmt_.GetType(i) == StmtType::kIf;
                     });
        return if_stmt;
    }

    assert(type == StmtType::kWhile);
    std::set<int> while_stmt;
    std::copy_if(container_stmt.begin(), container_stmt.end(),
                 std::inserter(while_stmt, while_stmt.begin()), [this](int i) {
                     return type_stmt_.GetType(i) == StmtType::kWhile;
                 });
    return while_stmt;
}

std::pair<std::vector<int>, std::vector<int>>
ProgramKnowledgeBase::GetModifiesStmtVar(StmtType type) {
    assert(compiled);

    if (type == StmtType::kPrint || type == StmtType::kCall) {
        return {{}, {}};
    }

    if (type == StmtType::kAssign) {
        std::vector<int> assign_stmt =
                type_stmt_.GetStatements(StmtType::kAssign);
        std::vector<int> assign_var;

        for (auto &i : assign_stmt) {
            assign_var.emplace_back(modifies_rel_.GetVarIndex(i));
        }

        return {assign_stmt, assign_var};
    }

    if (type == StmtType::kRead) {
        std::vector<int> read_stmt = type_stmt_.GetStatements(StmtType::kRead);
        std::vector<int> read_var;

        for (auto &i : read_stmt) {
            read_var.emplace_back(modifies_rel_.GetVarIndex(i));
        }

        return {read_stmt, read_var};
    }

    std::set<int> container_stmt;
    std::vector<int> parents, direct_modifying_stmt;

    direct_modifying_stmt = type_stmt_.GetStatements(StmtType::kAssign);
    direct_modifying_stmt.insert(
            direct_modifying_stmt.end(),
            type_stmt_.GetStatements(StmtType::kRead).begin(),
            type_stmt_.GetStatements(StmtType::kRead).end());

    if (type == StmtType::kIf) {
        std::vector<int> if_stmt;
        std::vector<int> if_var;

        for (auto &i : direct_modifying_stmt) {
            int var_index = modifies_rel_.GetVarIndex(i);
            int stmtlst = stmtlst_stmt_.GetStmtlst(i);
            parents = container_forest_->GetParents(stmtlst);
            for (auto &j : parents) {
                auto stmt = stmtlst_parent_.GetParent(j);
                if (stmt.type == StmtlstParentStore::kIf) {
                    if_stmt.emplace_back(stmt.index);
                    if_var.emplace_back(var_index);
                }
            }
        }
        return {if_stmt, if_var};
    }

    if (type == StmtType::kWhile) {
        std::vector<int> while_stmt;
        std::vector<int> while_var;

        for (auto &i : direct_modifying_stmt) {
            int var_index = modifies_rel_.GetVarIndex(i);
            int stmtlst = stmtlst_stmt_.GetStmtlst(i);
            parents = container_forest_->GetParents(stmtlst);
            for (auto &j : parents) {
                auto stmt = stmtlst_parent_.GetParent(j);
                if (stmt.type == StmtlstParentStore::kWhile) {
                    while_stmt.emplace_back(stmt.index);
                    while_var.emplace_back(var_index);
                }
            }
        }
        return {while_stmt, while_var};
    }

    assert(type == StmtType::kAll);

    std::vector<int> all_stmt;
    std::vector<int> all_var;

    for (auto &i : direct_modifying_stmt) {
        all_stmt.emplace_back(i);
        int var_index = modifies_rel_.GetVarIndex(i);
        all_var.emplace_back(var_index);
        int stmtlst = stmtlst_stmt_.GetStmtlst(i);
        parents = container_forest_->GetParents(stmtlst);
        parents.emplace_back(stmtlst);

        for (auto &j : parents) {
            auto stmt = stmtlst_parent_.GetParent(j);
            if (stmt.type != PType::kWhile && stmt.type != PType::kIf) {
                continue;
            }
            all_stmt.emplace_back(stmt.index);
            all_var.emplace_back(var_index);
        }
    }
    return {all_stmt, all_var};
}

std::set<int> ProgramKnowledgeBase::GetUses(
        Index<QueryEntityType::kStmt> stmt_no) {
    assert(compiled);
    assert(stmt_no.value != 0);

    StmtType type = type_stmt_.GetType(stmt_no.value);

    if (type == StmtType::kRead || type == StmtType::kCall) {
        return {};
    }

    if (type == StmtType::kAssign || type == StmtType::kPrint) {
        return {uses_rel_.GetVarIndex(stmt_no.value).begin(),
                uses_rel_.GetVarIndex(stmt_no.value).end()};
    }

    auto first_stmt = stmt_no.value + 1;
    int last_stmt;

    auto follow_vec = GetFollows(false, Index<ArgPos::kFirst>(stmt_no.value),
                                 StmtType::kAll);

    if (follow_vec.size() == 1) {
        last_stmt = *follow_vec.begin();
    } else {
        int stmtlst = type == StmtType::kWhile
                              ? stmtlst_parent_.GetWhileStmtLst(stmt_no.value)
                              : stmtlst_parent_.GetIfStmtLst(stmt_no.value)
                                        .else_index;
        int grandchild = container_forest_->GetRightmostGrandchild(stmtlst);
        auto grandchild_stmt = stmtlst_stmt_.GetStatements(grandchild);
        last_stmt = grandchild_stmt.back();
    }

    const auto &assign_stmts = type_stmt_.GetStatements(StmtType::kAssign);
    auto first_assign = std::lower_bound(assign_stmts.begin(),
                                         assign_stmts.end(), first_stmt);

    auto second_assign =
            std::upper_bound(first_assign, assign_stmts.end(), last_stmt);

    std::set<int> results;

    for (auto i = first_assign; i != second_assign; i++) {
        results.insert(uses_rel_.GetVarIndex(*i).begin(),
                       uses_rel_.GetVarIndex(*i).end());
    }

    const auto &print_stmts = type_stmt_.GetStatements(StmtType::kPrint);
    auto first_print = std::lower_bound(print_stmts.begin(), print_stmts.end(),
                                        first_stmt);
    auto second_print =
            std::upper_bound(first_print, print_stmts.end(), last_stmt);

    for (auto i = first_print; i != second_print; i++) {
        results.insert(uses_rel_.GetVarIndex(*i).begin(),
                       uses_rel_.GetVarIndex(*i).end());
    }
    return results;
}

std::set<int> ProgramKnowledgeBase::GetUses(
        Index<QueryEntityType::kVar> var_index, StmtType type) {
    assert(compiled);
    assert(var_index.value != 0);

    if (type == StmtType::kPrint || type == StmtType::kCall) {
        return {};
    }

    auto direct_uses_stmt = uses_rel_.GetStmtNo(var_index.value);

    if (type == StmtType::kAssign) {
        std::set<int> assign_stmt;
        std::copy_if(direct_uses_stmt.begin(), direct_uses_stmt.end(),
                     std::inserter(assign_stmt, assign_stmt.begin()),
                     [this](int i) {
                         return type_stmt_.GetType(i) == StmtType::kAssign;
                     });
        return assign_stmt;
    }

    if (type == StmtType::kPrint) {
        std::set<int> print_stmt;
        std::copy_if(direct_uses_stmt.begin(), direct_uses_stmt.end(),
                     std::inserter(print_stmt, print_stmt.begin()),
                     [this](int i) {
                         return type_stmt_.GetType(i) == StmtType::kPrint;
                     });
        return print_stmt;
    }

    std::set<int> container_stmt;
    std::vector<int> parents;

    for (auto &i : direct_uses_stmt) {
        int stmtlst = stmtlst_stmt_.GetStmtlst(i);
        parents = container_forest_->GetParents(stmtlst);
        parents.emplace_back(stmtlst);

        for (int i = 0; i < parents.size(); i++) {
            auto stmt = stmtlst_parent_.GetParent(i);
            if (stmt.type != PType::kWhile && stmt.type != PType::kIf) {
                continue;
            }
            container_stmt.emplace(stmt.index);
        }
    }

    if (type == StmtType::kAll) {
        container_stmt.insert(direct_uses_stmt.begin(), direct_uses_stmt.end());
        return container_stmt;
    }

    if (type == StmtType::kIf) {
        std::set<int> if_stmt;
        std::copy_if(container_stmt.begin(), container_stmt.end(),
                     std::inserter(if_stmt, if_stmt.begin()), [this](int i) {
                         return type_stmt_.GetType(i) == StmtType::kIf;
                     });
        return if_stmt;
    }

    assert(type == StmtType::kWhile);

    std::set<int> while_stmt;
    std::copy_if(container_stmt.begin(), container_stmt.end(),
                 std::inserter(while_stmt, while_stmt.begin()), [this](int i) {
                     return type_stmt_.GetType(i) == StmtType::kWhile;
                 });
    return while_stmt;
}

std::set<int> ProgramKnowledgeBase::GetUses(StmtType type) {
    assert(compiled);

    if (type == StmtType::kRead || type == StmtType::kCall) {
        return {};
    }

    if (type == StmtType::kAssign) {
        std::set<int> assign_stmt;
        std::copy_if(
                type_stmt_.GetStatements(StmtType::kAssign).begin(),
                type_stmt_.GetStatements(StmtType::kAssign).end(),
                std::inserter(assign_stmt, assign_stmt.begin()),
                [this](int i) { return uses_rel_.GetVarIndex(i).size() > 0; });
        return assign_stmt;
    }

    if (type == StmtType::kPrint) {
        return {type_stmt_.GetStatements(StmtType::kPrint).begin(),
                type_stmt_.GetStatements(StmtType::kPrint).end()};
    }

    std::set<int> container_stmt;
    std::vector<int> parents, direct_modifying_stmt;

    direct_modifying_stmt = type_stmt_.GetStatements(StmtType::kAssign);
    direct_modifying_stmt.insert(
            direct_modifying_stmt.end(),
            type_stmt_.GetStatements(StmtType::kPrint).begin(),
            type_stmt_.GetStatements(StmtType::kPrint).end());

    for (auto &i : direct_modifying_stmt) {
        int stmtlst = stmtlst_stmt_.GetStmtlst(i);
        parents = container_forest_->GetParents(stmtlst);
        parents.emplace_back(stmtlst);

        for (int i = 0; i < parents.size(); i++) {
            auto stmt = stmtlst_parent_.GetParent(i);
            if (stmt.type != PType::kWhile && stmt.type != PType::kIf) {
                continue;
            }
            container_stmt.emplace(stmt.index);
        }
    }

    if (type == StmtType::kAll) {
        container_stmt.insert(direct_modifying_stmt.begin(),
                              direct_modifying_stmt.end());
        return container_stmt;
    }

    if (type == StmtType::kIf) {
        std::set<int> if_stmt;
        std::copy_if(container_stmt.begin(), container_stmt.end(),
                     std::inserter(if_stmt, if_stmt.begin()), [this](int i) {
                         return type_stmt_.GetType(i) == StmtType::kIf;
                     });
        return if_stmt;
    }

    assert(type == StmtType::kWhile);
    std::set<int> while_stmt;
    std::copy_if(container_stmt.begin(), container_stmt.end(),
                 std::inserter(while_stmt, while_stmt.begin()), [this](int i) {
                     return type_stmt_.GetType(i) == StmtType::kWhile;
                 });
    return while_stmt;
}

std::pair<std::vector<int>, std::vector<int>>
ProgramKnowledgeBase::GetUsesStmtVar(StmtType type) {
    if (type == StmtType::kRead || type == StmtType::kCall) {
        return {{}, {}};
    }

    if (type == StmtType::kAssign) {
        std::vector<int> assign_stmt =
                type_stmt_.GetStatements(StmtType::kAssign);
        std::vector<int> assign_var;
        std::vector<int> assign_stmt_pair;

        for (auto &i : assign_stmt) {
            assign_var.insert(assign_var.end(),
                              uses_rel_.GetVarIndex(i).begin(),
                              uses_rel_.GetVarIndex(i).end());
            for (int i = 0; i < uses_rel_.GetVarIndex(i).size(); i++) {
                assign_stmt_pair.emplace_back(i);
            }
        }
        return {assign_stmt_pair, assign_var};
    }

    if (type == StmtType::kPrint) {
        std::vector<int> print_stmt =
                type_stmt_.GetStatements(StmtType::kPrint);
        std::vector<int> print_var;

        for (auto &i : print_stmt) {
            print_var.emplace_back(uses_rel_.GetVarIndex(i)[0]);
        }

        return {print_stmt, print_var};
    }

    std::set<int> container_stmt;
    std::vector<int> parents, direct_modifying_stmt;

    direct_modifying_stmt = type_stmt_.GetStatements(StmtType::kAssign);
    direct_modifying_stmt.insert(
            direct_modifying_stmt.end(),
            type_stmt_.GetStatements(StmtType::kPrint).begin(),
            type_stmt_.GetStatements(StmtType::kPrint).end());

    if (type == StmtType::kIf) {
        std::vector<int> if_stmt;
        std::vector<int> if_var;

        for (auto &i : direct_modifying_stmt) {
            auto var_indices = uses_rel_.GetVarIndex(i);
            int stmtlst = stmtlst_stmt_.GetStmtlst(i);
            parents = container_forest_->GetParents(stmtlst);
            parents.emplace_back(stmtlst);

            for (auto &j : parents) {
                auto stmt = stmtlst_parent_.GetParent(j);
                if (stmt.type == StmtlstParentStore::kIf) {
                    if_var.insert(if_var.end(), var_indices.begin(),
                                  var_indices.end());

                    for (int i = 0; i < var_indices.size(); i++) {
                        if_stmt.emplace_back(stmt.index);
                    }
                }
            }
        }
        return {if_stmt, if_var};
    }

    if (type == StmtType::kWhile) {
        std::vector<int> while_var;
        std::vector<int> while_stmt;

        for (auto &i : direct_modifying_stmt) {
            auto var_indices = uses_rel_.GetVarIndex(i);
            int stmtlst = stmtlst_stmt_.GetStmtlst(i);
            parents = container_forest_->GetParents(stmtlst);
            parents.emplace_back(stmtlst);

            for (auto &j : parents) {
                auto stmt = stmtlst_parent_.GetParent(j);
                if (stmt.type == StmtlstParentStore::kWhile) {
                    while_var.insert(while_var.end(), var_indices.begin(),
                                     var_indices.end());

                    for (int i = 0; i < var_indices.size(); i++) {
                        while_stmt.emplace_back(stmt.index);
                    }
                }
            }
        }
        return {while_stmt, while_var};
    }

    assert(type == StmtType::kAll);
    std::vector<int> all_stmt;
    std::vector<int> all_var;

    for (auto &i : direct_modifying_stmt) {
        auto var_indices = uses_rel_.GetVarIndex(i);
        all_var.insert(all_var.end(), var_indices.begin(), var_indices.end());
        for (int i = 0; i < var_indices.size(); i++) {
            all_stmt.emplace_back(i);
        }

        int stmtlst = stmtlst_stmt_.GetStmtlst(i);
        parents = container_forest_->GetParents(stmtlst);
        parents.emplace_back(stmtlst);

        for (auto &j : parents) {
            auto stmt = stmtlst_parent_.GetParent(j);
            if (stmt.type != PType::kWhile && stmt.type != PType::kIf) {
                continue;
            }
            all_var.insert(all_var.end(), var_indices.begin(),
                           var_indices.end());
            for (int i = 0; i < var_indices.size(); i++) {
                all_stmt.emplace_back(stmt.index);
            }
        }
    }
    return {all_stmt, all_var};
}

// ( _, " "), (_, _" "_)
std::set<int> ProgramKnowledgeBase::GetPattern(std::vector<QueryToken> tokens,
                                               bool partial_match) {
    assert(compiled);
    if (ContainsUnseenVarConst(tokens)) {
        return {};
    }

    std::vector<int> assign_stmt;
    assign_stmt = type_stmt_.GetStatements(StmtType::kAssign);
    PN converted_pn = (ConvertFromQueryTokens(tokens));

    if (partial_match) {
        std::set<int> partial_match_stmt;

        for (auto &i : assign_stmt) {
            const PN &pn = polish_notation_.GetNotation(
                    polish_notation_.GetPolishIndex(i));
            if (pn.Contains(converted_pn)) {
                partial_match_stmt.emplace(i);
            }
        }
        return partial_match_stmt;
    }

    assert(partial_match == false);
    std::set<int> full_match_stmt;

    for (auto &i : assign_stmt) {
        const PN &pn = polish_notation_.GetNotation(
                polish_notation_.GetPolishIndex(i));
        if (pn == converted_pn) {
            full_match_stmt.emplace(i);
        }
    }

    return full_match_stmt;
}

// (" ", _)
std::set<int> ProgramKnowledgeBase::GetPattern(QueryToken token) {
    assert(compiled);
    int var_index = var_name_.GetIndex(token.value);
    if (var_index == 0) {
        return {};
    }

    return GetModifies(Index<QueryEntityType::kVar>(var_index),
                       StmtType::kAssign);
}

// (" ", " ") , (" ", _" "_)
std::set<int> ProgramKnowledgeBase::GetPattern(
        QueryToken first_token, std::vector<QueryToken> second_tokens,
        bool partial_match) {
    assert(compiled);
    if (ContainsUnseenVarConst(second_tokens)) {
        return {};
    }

    int var_index = var_name_.GetIndex(first_token.value);
    if (var_index == 0) {
        return {};
    }

    std::set<int> assign_stmt;
    std::set<int> filtered_assign_stmt;
    assign_stmt = GetPattern(second_tokens, partial_match);

    for (auto &i : assign_stmt) {
        if (ExistModifies(i, var_index)) {
            filtered_assign_stmt.emplace(i);
        }
    }
    return filtered_assign_stmt;
}
// (v, " ") , (v, _" "_)
std::pair<std::vector<int>, std::vector<int>>
ProgramKnowledgeBase::GetPatternPair(std::vector<QueryToken> tokens,
                                     bool partial_match) {
    assert(compiled);
    if (ContainsUnseenVarConst(tokens)) {
        return {};
    }

    std::set<int> assign_stmt_set;
    std::vector<int> assign_var;
    std::vector<int> assign_stmt;
    assign_stmt_set = GetPattern(tokens, partial_match);

    for (auto &i : assign_stmt_set) {
        auto index = Index<QueryEntityType::kStmt>(i);
        assign_var.emplace_back(*GetModifies(index).begin());
    }
    return {assign_stmt, assign_var};
}

// (v, _)
std::pair<std::vector<int>, std::vector<int>>
ProgramKnowledgeBase::GetPatternPair() {
    return GetModifiesStmtVar(StmtType::kAssign);
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

std::pair<PolishNotation, bool> ProgramKnowledgeBase::ConvertFromQueryTokens(
        const std::vector<QueryToken> &tokens) {
    std::vector<PolishNotationNode> expr;
    bool contains_unseen = false;
    for (const auto &token : tokens) {
        switch (token.type) {
            case QueryTokenType::WORD: {
                int var_index = var_name_.GetIndex(token.value);
                if (var_index == 0) {
                    contains_unseen = true;
                    break;
                }
                PolishNotationNode node(ExprNodeType::kVariable, var_index);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::INTEGER: {
                int const_index = const_value_.GetIndex(token.value);
                if (const_index = 0) {
                    contains_unseen = true;
                    break;
                }
                PolishNotationNode node(ExprNodeType::kConstant, const_index);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::PLUS: {
                PolishNotationNode node(OperatorType::kPlus);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::MINUS: {
                PolishNotationNode node(OperatorType::kMinus);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::TIMES: {
                PolishNotationNode node(OperatorType::kTimes);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::DIVIDE: {
                PolishNotationNode node(OperatorType::kDivide);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::MODULO: {
                PolishNotationNode node(OperatorType::kModulo);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::LEFTBRACKET: {
                PolishNotationNode node(ExprNodeType::kBracketL);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::RIGHTBRACKET: {
                PolishNotationNode node(ExprNodeType::kBracketR);
                expr.emplace_back(node);
                break;
            }
            default:
                assert(false);
        }
        if (contains_unseen) {
            break;
        }
    }
    return std::make_pair(PolishNotation(expr), contains_unseen);
}

}  // namespace spa
