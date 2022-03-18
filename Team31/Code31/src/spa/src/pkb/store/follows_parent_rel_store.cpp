#include "follows_parent_rel_store.h"

#include <utility>

#include "pkb/knowledge_base.h"
namespace spa {
FollowsParentRelStore::FollowsParentRelStore(size_t stmt_count)
        : stmt_count_(stmt_count) {}
void FollowsParentRelStore::Compile(
        std::unique_ptr<TypeStatementsStore> type_stmt,
        std::unique_ptr<ContainerForest> container_forest,
        std::unique_ptr<StmtlstParentStore> stmtlst_parent,
        std::unique_ptr<StmtlstStatementsStore> stmtlst_stmt) {
    type_stmt_ = std::move(type_stmt);
    container_forest_ = std::move(container_forest);
    stmtlst_parent_ = std::move(stmtlst_parent);
    stmtlst_stmt_ = std::move(stmtlst_stmt);
}
bool FollowsParentRelStore::ExistFollows(bool transitive,
                                         Index<ArgPos::kFirst> first_stmt,
                                         Index<ArgPos::kSecond> second_stmt) {
    return stmtlst_stmt_->ExistFollows(transitive, first_stmt, second_stmt);
}
bool FollowsParentRelStore::ExistFollows(Index<ArgPos::kFirst> first_stmt) {
    return stmtlst_stmt_->ExistFollows(first_stmt);
}
bool FollowsParentRelStore::ExistFollows(Index<ArgPos::kSecond> second_stmt) {
    return stmtlst_stmt_->ExistFollows(second_stmt);
}
bool FollowsParentRelStore::ExistFollows() {
    return stmtlst_stmt_->ExistFollows();
}
std::set<int> FollowsParentRelStore::filter(std::vector<int> results,
                                            StmtType return_type) {
    if (return_type == StmtType::kAll) {
        return {results.begin(), results.end()};
    }
    std::set<int> filtered_results;
    std::copy_if(results.begin(), results.end(),
                 std::inserter(filtered_results, filtered_results.end()),
                 [this, return_type](int i) {
                     return type_stmt_->GetType(i) == return_type;
                 });
    return filtered_results;
}
std::set<int> FollowsParentRelStore::GetFollows(ArgPos return_pos,
                                                StmtType return_type) {
    auto results = return_pos == ArgPos::kFirst
                           ? stmtlst_stmt_->GetFollowedByWildcard()
                           : stmtlst_stmt_->GetFollowsWildcard();
    return filter(std::move(results), return_type);
}
std::set<int> FollowsParentRelStore::GetFollows(
        bool transitive, Index<ArgPos::kFirst> first_stmt,
        StmtType return_type) {
    auto results = stmtlst_stmt_->GetFollows(transitive, first_stmt);
    return filter(std::move(results), return_type);
}

std::set<int> FollowsParentRelStore::GetFollows(
        bool transitive, Index<ArgPos::kSecond> second_stmt,
        StmtType return_type) {
    auto results = stmtlst_stmt_->GetFollows(transitive, second_stmt);
    return filter(std::move(results), return_type);
}

PairVec<int> FollowsParentRelStore::GetFollowsPairs(bool transitive,
                                                    StmtType first_type,
                                                    StmtType second_type) {
    PairVec<int> results = stmtlst_stmt_->GetFollowsPairs(transitive);
    if (first_type == StmtType::kAll && second_type == StmtType::kAll) {
        return results;
    }
    PairVec<int> filtered_results;
    for (int i = 0; i < results.first.size(); ++i) {
        if ((first_type == StmtType::kAll ||
             type_stmt_->GetType(results.first[i]) == first_type) &&
            (second_type == StmtType::kAll ||
             type_stmt_->GetType(results.second[i]) == second_type)) {
            filtered_results.first.emplace_back(results.first[i]);
            filtered_results.second.emplace_back(results.second[i]);
        }
    }
    return filtered_results;
}

bool FollowsParentRelStore::ExistParent(bool transitive,
                                        Index<ArgPos::kFirst> parent_stmt,
                                        Index<ArgPos::kSecond> child_stmt) {
    int parent = parent_stmt.value;
    int child = child_stmt.value;
    if (child <= parent || parent > stmt_count_ - 1 || child > stmt_count_) {
        return false;
    }
    StmtType parent_type = type_stmt_->GetType(parent);
    if (parent_type != StmtType::kWhile && parent_type != StmtType::kIf) {
        return false;
    }

    if (transitive) {
        int last_stmt = GetContainerLastStmt(parent_type, parent);
        return child_stmt.value <= last_stmt;
    }
    int stmtlst_index = stmtlst_stmt_->GetStmtlst(child);
    return stmtlst_parent_->GetWhileStmtLst(parent) == stmtlst_index ||
           stmtlst_parent_->GetIfStmtLst(parent).then_index == stmtlst_index ||
           stmtlst_parent_->GetIfStmtLst(parent).else_index == stmtlst_index;
}
bool FollowsParentRelStore::ExistParent(Index<ArgPos::kFirst> parent_stmt) {
    return parent_stmt.value < stmt_count_ &&
           (type_stmt_->GetType(parent_stmt.value) == StmtType::kIf ||
            type_stmt_->GetType(parent_stmt.value) == StmtType::kWhile);
}
bool FollowsParentRelStore::ExistParent(Index<ArgPos::kSecond> child_stmt) {
    int stmtlst_index = stmtlst_stmt_->GetStmtlst(child_stmt.value);
    return child_stmt.value <= stmt_count_ &&
           (stmtlst_parent_->GetParent(stmtlst_index).type == PType::kIf ||
            stmtlst_parent_->GetParent(stmtlst_index).type == PType::kWhile);
}
bool FollowsParentRelStore::ExistParent() {
    return !type_stmt_->GetStatements(StmtType::kWhile).empty() ||
           !type_stmt_->GetStatements(StmtType::kIf).empty();
}

std::set<int> FollowsParentRelStore::GetAllParents(StmtType return_type) {
    if (return_type != StmtType::kAll && return_type != StmtType::kWhile &&
        return_type != StmtType::kIf) {
        return {};
    }
    std::vector<int> results;
    if (return_type == StmtType::kAll) {
        results = type_stmt_->GetStatements(StmtType::kWhile);
        auto if_stmts = type_stmt_->GetStatements(StmtType::kIf);
        results.insert(results.end(), if_stmts.begin(), if_stmts.end());
    } else {
        results = type_stmt_->GetStatements(return_type);
    }
    return {results.begin(), results.end()};
}
std::set<int> FollowsParentRelStore::GetAllChildren(StmtType return_type) {
    if (return_type == StmtType::kAll) {
        std::vector<int> results;
        for (auto &while_stmt : type_stmt_->GetStatements(StmtType::kWhile)) {
            int stmtlst_index = stmtlst_parent_->GetWhileStmtLst(while_stmt);
            auto stmtlst = stmtlst_stmt_->GetStatements(stmtlst_index);
            results.insert(results.end(), stmtlst.begin(), stmtlst.end());
        }
        for (auto &if_stmt : type_stmt_->GetStatements(StmtType::kIf)) {
            int then_index = stmtlst_parent_->GetIfStmtLst(if_stmt).then_index;
            int else_index = stmtlst_parent_->GetIfStmtLst(if_stmt).else_index;
            auto then_stmtlst = stmtlst_stmt_->GetStatements(then_index);
            auto else_stmtlst = stmtlst_stmt_->GetStatements(else_index);
            results.insert(results.end(), then_stmtlst.begin(),
                           then_stmtlst.end());
            results.insert(results.end(), else_stmtlst.begin(),
                           else_stmtlst.end());
        }
        return {results.begin(), results.end()};
    }
    std::vector<int> results = type_stmt_->GetStatements(return_type);
    std::vector<int> filtered_results;
    for (auto &i : results) {
        int stmtlst_index = stmtlst_stmt_->GetStmtlst(i);
        PType type = stmtlst_parent_->GetParent(stmtlst_index).type;
        if (type != PType::kIf && type != PType::kWhile) continue;
        filtered_results.emplace_back(i);
    }
    return {filtered_results.begin(), filtered_results.end()};
}
std::set<int> FollowsParentRelStore::GetParent(ArgPos return_pos,
                                               StmtType return_type) {
    if (return_pos == ArgPos::kFirst) {
        return GetAllParents(return_type);
    }
    return GetAllChildren(return_type);
}
void FollowsParentRelStore::GetNonTransitiveParentFirst(
        StmtType parent_type, int parent, std::vector<int> &results) const {
    if (parent_type == StmtType::kWhile) {
        int stmtlst_index = stmtlst_parent_->GetWhileStmtLst(parent);
        results = stmtlst_stmt_->GetStatements(stmtlst_index);
    }
    int then_index = stmtlst_parent_->GetIfStmtLst(parent).then_index;
    int else_index = stmtlst_parent_->GetIfStmtLst(parent).else_index;
    results = stmtlst_stmt_->GetStatements(then_index);
    auto else_vector = stmtlst_stmt_->GetStatements(else_index);
    results.insert(results.end(), else_vector.begin(), else_vector.end());
}
void FollowsParentRelStore::GetTransitiveParentFirst(
        StmtType parent_type, int parent, std::vector<int> &results) {
    int last_stmt = GetContainerLastStmt(parent_type, parent);
    int size = last_stmt - parent;
    results.resize(size);
    std::iota(results.begin(), results.end(), parent + 1);
}
std::set<int> FollowsParentRelStore::GetParent(
        bool transitive, Index<ArgPos::kFirst> parent_stmt,
        StmtType return_type) {
    if (parent_stmt.value > stmt_count_ - 1) {
        return {};
    }
    StmtType parent_type = type_stmt_->GetType(parent_stmt.value);
    if (parent_type != StmtType::kWhile && parent_type != StmtType::kIf) {
        return {};
    }

    std::vector<int> results;
    if (transitive) {
        GetTransitiveParentFirst(parent_type, parent_stmt.value, results);
    } else {
        GetNonTransitiveParentFirst(parent_type, parent_stmt.value, results);
    }
    return filter(std::move(results), return_type);
}
std::set<int> FollowsParentRelStore::GetParent(
        bool transitive, Index<ArgPos::kSecond> child_stmt,
        StmtType return_type) {
    if (return_type != StmtType::kAll && return_type != StmtType::kWhile &&
        return_type != StmtType::kIf) {
        return {};
    }
    if (child_stmt.value > stmt_count_) {
        return {};
    }

    int stmtlst_index = stmtlst_stmt_->GetStmtlst(child_stmt.value);
    PType type = stmtlst_parent_->GetParent(stmtlst_index).type;
    if (type != PType::kIf && type != PType::kWhile) {
        return {};
    }
    std::vector<int> results;
    if (transitive) {
        std::vector<int> ancestors =
                container_forest_->GetAncestryTrace(stmtlst_index);
        for (int i : ancestors) {
            if (stmtlst_parent_->GetParent(i).type != PType::kWhile &&
                stmtlst_parent_->GetParent(i).type != PType::kIf)
                continue;
            int parent = stmtlst_parent_->GetParent(i).index;
            results.emplace_back(parent);
        }
    } else {
        results = {stmtlst_parent_->GetParent(stmtlst_index).index};
    }

    return filter(std::move(results), return_type);
}

void FollowsParentRelStore::GetTransitiveParentPairs(
        std::pair<std::vector<int>, std::vector<int>> &results) {
    for (auto &while_stmt : type_stmt_->GetStatements(StmtType::kWhile)) {
        int end = GetContainerLastStmt(StmtType::kWhile, while_stmt);
        for (int i = while_stmt + 1; i <= end; ++i) {
            results.first.emplace_back(while_stmt);
            results.second.emplace_back(i);
        }
    }
    for (auto &if_stmt : type_stmt_->GetStatements(StmtType::kIf)) {
        int end = GetContainerLastStmt(StmtType::kIf, if_stmt);
        for (int i = if_stmt + 1; i <= end; ++i) {
            results.first.emplace_back(if_stmt);
            results.second.emplace_back(i);
        }
    }
}
void FollowsParentRelStore::GetNonTransitiveParentPairs(
        std::pair<std::vector<int>, std::vector<int>> &results) {
    for (auto &while_stmt : type_stmt_->GetStatements(StmtType::kWhile)) {
        int stmtlst_index = stmtlst_parent_->GetWhileStmtLst(while_stmt);
        auto stmtlst = stmtlst_stmt_->GetStatements(stmtlst_index);
        for (auto &child_stmt : stmtlst) {
            results.first.emplace_back(while_stmt);
            results.second.emplace_back(child_stmt);
        }
    }
    for (auto &if_stmt : type_stmt_->GetStatements(StmtType::kIf)) {
        int then_index = stmtlst_parent_->GetIfStmtLst(if_stmt).then_index;
        int else_index = stmtlst_parent_->GetIfStmtLst(if_stmt).else_index;
        auto then_stmtlst = stmtlst_stmt_->GetStatements(then_index);
        auto else_stmtlst = stmtlst_stmt_->GetStatements(else_index);
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
PairVec<int> FollowsParentRelStore::GetParentPairs(bool transitive,
                                                   StmtType parent_type,
                                                   StmtType child_type) {
    if (parent_type != StmtType::kAll && parent_type != StmtType::kWhile &&
        parent_type != StmtType::kIf) {
        return {};
    }
    PairVec<int> results;
    if (transitive) {
        GetTransitiveParentPairs(results);
    } else {
        GetNonTransitiveParentPairs(results);
    }
    if (parent_type == StmtType::kAll && child_type == StmtType::kAll) {
        return results;
    }
    PairVec<int> filtered_results;
    for (int i = 0; i < results.first.size(); ++i) {
        if ((parent_type == StmtType::kAll ||
             type_stmt_->GetType(results.first[i]) == parent_type) &&
            (child_type == StmtType::kAll ||
             type_stmt_->GetType(results.second[i]) == child_type)) {
            filtered_results.first.emplace_back(results.first[i]);
            filtered_results.second.emplace_back(results.second[i]);
        }
    }
    return filtered_results;
}

int FollowsParentRelStore::GetContainerLastStmt(StmtType type, int stmt_no) {
    int last_stmt;
    auto follow_vec =
            stmtlst_stmt_->GetFollows(false, Index<ArgPos::kFirst>(stmt_no));

    if (follow_vec.size() == 1) {
        last_stmt = *follow_vec.begin() - 1;
        return last_stmt;
    }

    int stmtlst = type == StmtType::kWhile
                          ? stmtlst_parent_->GetWhileStmtLst(stmt_no)
                          : stmtlst_parent_->GetIfStmtLst(stmt_no).else_index;
    int grandchild = container_forest_->GetRightmostGrandchild(stmtlst);
    auto grandchild_stmt = stmtlst_stmt_->GetStatements(grandchild);
    last_stmt = grandchild_stmt.back();

    return last_stmt;
}
}  // namespace spa