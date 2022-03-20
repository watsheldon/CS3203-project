#include "follows_parent_rel_store.h"

#include <utility>

#include "pkb/knowledge_base.h"
namespace spa {
FollowsParentRelStore::FollowsParentRelStore(size_t stmt_count,
                                             StorePointers ptr)
        : stmt_count_(stmt_count), ptr_(ptr) {}
bool FollowsParentRelStore::ExistFollows(
        bool transitive, Index<ArgPos::kFirst> first_stmt,
        Index<ArgPos::kSecond> second_stmt) const {
    return ptr_.stmtlst_stmt->ExistFollows(transitive, first_stmt, second_stmt);
}
bool FollowsParentRelStore::ExistFollows(
        Index<ArgPos::kFirst> first_stmt) const {
    return ptr_.stmtlst_stmt->ExistFollows(first_stmt);
}
bool FollowsParentRelStore::ExistFollows(
        Index<ArgPos::kSecond> second_stmt) const {
    return ptr_.stmtlst_stmt->ExistFollows(second_stmt);
}
bool FollowsParentRelStore::ExistFollows() const {
    return ptr_.stmtlst_stmt->ExistFollows();
}
std::set<int> FollowsParentRelStore::Extract(std::vector<int> results,
                                             StmtType return_type) const {
    if (return_type == StmtType::kAll) {
        return {results.begin(), results.end()};
    }
    std::set<int> extracted_results;
    std::copy_if(results.begin(), results.end(),
                 std::inserter(extracted_results, extracted_results.end()),
                 [this, return_type](int i) {
                     return ptr_.type_stmt->GetType(i) == return_type;
                 });
    return extracted_results;
}
PairVec<int> FollowsParentRelStore::ExtractPairs(PairVec<int> results,
                                                 StmtType first,
                                                 StmtType second) const {
    if (first == StmtType::kAll && second == StmtType::kAll) return results;
    PairVec<int> extracted_results;
    for (int i = 0; i < results.first.size(); ++i) {
        bool has_first = first == StmtType::kAll ||
                         first == ptr_.type_stmt->GetType(results.first[i]);
        bool has_second = second == StmtType::kAll ||
                          second == ptr_.type_stmt->GetType(results.second[i]);
        if (has_first && has_second) {
            extracted_results.first.emplace_back(results.first[i]);
            extracted_results.second.emplace_back(results.second[i]);
        }
    }
    return extracted_results;
}
std::set<int> FollowsParentRelStore::GetFollows(ArgPos return_pos,
                                                StmtType return_type) const {
    auto results = return_pos == ArgPos::kFirst
                           ? ptr_.stmtlst_stmt->GetFollowedByWildcard()
                           : ptr_.stmtlst_stmt->GetFollowsWildcard();
    return Extract(std::move(results), return_type);
}
std::set<int> FollowsParentRelStore::GetFollows(
        bool transitive, Index<ArgPos::kFirst> first_stmt,
        StmtType return_type) const {
    auto results = ptr_.stmtlst_stmt->GetFollows(transitive, first_stmt);
    return Extract(std::move(results), return_type);
}

std::set<int> FollowsParentRelStore::GetFollows(
        bool transitive, Index<ArgPos::kSecond> second_stmt,
        StmtType return_type) const {
    auto results = ptr_.stmtlst_stmt->GetFollows(transitive, second_stmt);
    return Extract(std::move(results), return_type);
}

PairVec<int> FollowsParentRelStore::GetFollowsPairs(
        bool transitive, StmtType first_type, StmtType second_type) const {
    PairVec<int> results = ptr_.stmtlst_stmt->GetFollowsPairs(transitive);
    return ExtractPairs(std::move(results), first_type, second_type);
}
inline bool FollowsParentRelStore::IsParent(int stmt) const {
    StmtType type = ptr_.type_stmt->GetType(stmt);
    return IsParent(type);
}
inline bool FollowsParentRelStore::IsParent(StmtType type) {
    return std::find(parent_types_.begin(), parent_types_.end(), type) !=
           parent_types_.end();
}
inline bool FollowsParentRelStore::HasParent(int stmt) const {
    int stmtlst = ptr_.stmtlst_stmt->GetStmtlst(stmt);
    PType type = ptr_.stmtlst_parent->GetParent(stmtlst).type;
    return type == PType::kWhile || type == PType::kIf;
}
bool FollowsParentRelStore::ExistParent(
        bool transitive, Index<ArgPos::kFirst> parent_stmt,
        Index<ArgPos::kSecond> child_stmt) const {
    int parent = parent_stmt.value;
    int child = child_stmt.value;
    if (child <= parent || parent >= stmt_count_ || child > stmt_count_ ||
        !IsParent(parent)) {
        return false;
    }
    if (transitive) {
        StmtType type = ptr_.type_stmt->GetType(parent);
        int last_stmt = GetContainerLastStmt(type, parent);
        return child <= last_stmt;
    }
    int stmtlst = ptr_.stmtlst_stmt->GetStmtlst(child);
    bool is_while = ptr_.stmtlst_parent->GetWhileStmtLst(parent) == stmtlst;
    bool is_then =
            ptr_.stmtlst_parent->GetIfStmtLst(parent).then_index == stmtlst;
    bool is_else =
            ptr_.stmtlst_parent->GetIfStmtLst(parent).else_index == stmtlst;
    return is_while || is_then || is_else;
}
bool FollowsParentRelStore::ExistParent(
        Index<ArgPos::kFirst> parent_stmt) const {
    int parent = parent_stmt.value;
    return parent < stmt_count_ && IsParent(parent);
}
bool FollowsParentRelStore::ExistParent(
        Index<ArgPos::kSecond> child_stmt) const {
    int child = child_stmt.value;
    return child <= stmt_count_ && HasParent(child);
}
bool FollowsParentRelStore::ExistParent() const {
    return !ptr_.type_stmt->GetStatements(StmtType::kWhile).empty() ||
           !ptr_.type_stmt->GetStatements(StmtType::kIf).empty();
}

std::set<int> FollowsParentRelStore::GetAllParents(StmtType return_type) const {
    if (!IsParent(return_type)) return {};
    std::vector<int> parents;
    if (return_type == StmtType::kAll) {
        parents = ptr_.type_stmt->GetStatements(StmtType::kWhile);
        auto if_stmts = ptr_.type_stmt->GetStatements(StmtType::kIf);
        parents.insert(parents.end(), if_stmts.begin(), if_stmts.end());
    } else {
        parents = ptr_.type_stmt->GetStatements(return_type);
    }
    return {parents.begin(), parents.end()};
}
std::set<int> FollowsParentRelStore::GetAllChildren(
        StmtType return_type) const {
    std::vector<int> stmts;
    if (return_type == StmtType::kAll) {
        stmts.resize(stmt_count_);
        std::iota(stmts.begin(), stmts.end(), 1);
    } else {
        stmts = ptr_.type_stmt->GetStatements(return_type);
    }
    std::set<int> children;
    std::copy_if(stmts.begin(), stmts.end(),
                 std::inserter(children, children.end()),
                 [this](int i) { return HasParent(i); });
    return children;
}
std::set<int> FollowsParentRelStore::GetParent(ArgPos return_pos,
                                               StmtType return_type) const {
    return (return_pos == ArgPos::kFirst) ? GetAllParents(return_type)
                                          : GetAllChildren(return_type);
}
void FollowsParentRelStore::FillChildren(int parent,
                                         std::vector<int> &results) const {
    StmtType type = ptr_.type_stmt->GetType(parent);
    if (type == StmtType::kWhile) {
        int stmtlst_index = ptr_.stmtlst_parent->GetWhileStmtLst(parent);
        results = ptr_.stmtlst_stmt->GetStatements(stmtlst_index);
    }
    int then_index = ptr_.stmtlst_parent->GetIfStmtLst(parent).then_index;
    int else_index = ptr_.stmtlst_parent->GetIfStmtLst(parent).else_index;
    results = ptr_.stmtlst_stmt->GetStatements(then_index);
    auto else_vector = ptr_.stmtlst_stmt->GetStatements(else_index);
    results.insert(results.end(), else_vector.begin(), else_vector.end());
}
void FollowsParentRelStore::FillChildrenT(int parent,
                                          std::vector<int> &results) const {
    StmtType type = ptr_.type_stmt->GetType(parent);
    int last_stmt = GetContainerLastStmt(type, parent);
    int size = last_stmt - parent;
    results.resize(size);
    std::iota(results.begin(), results.end(), parent + 1);
}
std::set<int> FollowsParentRelStore::GetParent(
        bool transitive, Index<ArgPos::kFirst> parent_stmt,
        StmtType return_type) const {
    int parent = parent_stmt.value;
    if (parent >= stmt_count_ || !IsParent(parent)) return {};
    std::vector<int> results;
    transitive ? FillChildrenT(parent, results) : FillChildren(parent, results);
    return Extract(std::move(results), return_type);
}
void FollowsParentRelStore::FillParents(int child,
                                        std::vector<int> &results) const {
    int stmtlst_index = ptr_.stmtlst_stmt->GetStmtlst(child);
    results = {ptr_.stmtlst_parent->GetParent(stmtlst_index).index};
}
void FollowsParentRelStore::FillParentsT(int child,
                                         std::vector<int> &results) const {
    int stmtlst_index = ptr_.stmtlst_stmt->GetStmtlst(child);
    std::vector<int> ancestors =
            ptr_.container_forest->GetAncestryTrace(stmtlst_index);
    for (int i : ancestors) {
        PType type = ptr_.stmtlst_parent->GetParent(i).type;
        if (type != PType::kWhile && type != PType::kIf) continue;
        int parent = ptr_.stmtlst_parent->GetParent(i).index;
        results.emplace_back(parent);
    }
}
std::set<int> FollowsParentRelStore::GetParent(
        bool transitive, Index<ArgPos::kSecond> child_stmt,
        StmtType return_type) const {
    int child = child_stmt.value;
    if (!IsParent(return_type) || child > stmt_count_ || !HasParent(child)) {
        return {};
    }
    std::vector<int> results;
    transitive ? FillParentsT(child, results) : FillParents(child, results);
    return Extract(std::move(results), return_type);
}

void FollowsParentRelStore::FillParentTPairs(PairVec<int> &results) const {
    auto &[parents, children] = results;
    for (int while_stmt : ptr_.type_stmt->GetStatements(StmtType::kWhile)) {
        int end = GetContainerLastStmt(StmtType::kWhile, while_stmt);
        for (int i = while_stmt + 1; i <= end; ++i) {
            parents.emplace_back(while_stmt);
            children.emplace_back(i);
        }
    }
    for (int if_stmt : ptr_.type_stmt->GetStatements(StmtType::kIf)) {
        int end = GetContainerLastStmt(StmtType::kIf, if_stmt);
        for (int i = if_stmt + 1; i <= end; ++i) {
            parents.emplace_back(if_stmt);
            children.emplace_back(i);
        }
    }
}
void FollowsParentRelStore::FillParentPairs(PairVec<int> &results) const {
    auto &[parents, children] = results;
    for (int while_stmt : ptr_.type_stmt->GetStatements(StmtType::kWhile)) {
        int stmtlst_index = ptr_.stmtlst_parent->GetWhileStmtLst(while_stmt);
        auto stmtlst = ptr_.stmtlst_stmt->GetStatements(stmtlst_index);
        for (int child_stmt : stmtlst) {
            parents.emplace_back(while_stmt);
            children.emplace_back(child_stmt);
        }
    }
    for (int if_stmt : ptr_.type_stmt->GetStatements(StmtType::kIf)) {
        int then_index = ptr_.stmtlst_parent->GetIfStmtLst(if_stmt).then_index;
        int else_index = ptr_.stmtlst_parent->GetIfStmtLst(if_stmt).else_index;
        auto then_stmtlst = ptr_.stmtlst_stmt->GetStatements(then_index);
        auto else_stmtlst = ptr_.stmtlst_stmt->GetStatements(else_index);
        for (int child_stmt : then_stmtlst) {
            parents.emplace_back(if_stmt);
            children.emplace_back(child_stmt);
        }
        for (int child_stmt : else_stmtlst) {
            parents.emplace_back(if_stmt);
            children.emplace_back(child_stmt);
        }
    }
}
PairVec<int> FollowsParentRelStore::GetParentPairs(bool transitive,
                                                   StmtType parent_type,
                                                   StmtType child_type) const {
    if (!IsParent(parent_type)) return {};
    PairVec<int> results;
    (transitive) ? FillParentTPairs(results) : FillParentPairs(results);
    return ExtractPairs(std::move(results), parent_type, child_type);
}

int FollowsParentRelStore::GetContainerLastStmt(StmtType type,
                                                int stmt_no) const {
    int last_stmt;
    auto followers = ptr_.stmtlst_stmt->GetFollows(
            false, Index<ArgPos::kFirst>(stmt_no));

    if (followers.size() == 1) {
        last_stmt = *followers.begin() - 1;
        return last_stmt;
    }

    int stmtlst =
            type == StmtType::kWhile
                    ? ptr_.stmtlst_parent->GetWhileStmtLst(stmt_no)
                    : ptr_.stmtlst_parent->GetIfStmtLst(stmt_no).else_index;
    int grandchild = ptr_.container_forest->GetRightmostGrandchild(stmtlst);
    auto grandchild_stmts = ptr_.stmtlst_stmt->GetStatements(grandchild);
    last_stmt = grandchild_stmts.back();
    return last_stmt;
}
}  // namespace spa
