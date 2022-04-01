#include "parent_relationship_store.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <numeric>
#include <set>
#include <vector>

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "common/index.h"

namespace spa {
ParentRelationshipStore::ParentRelationshipStore(std::size_t stmt_count,
                                                 StoreRefs refs) noexcept
        : FollowsParentRelationshipBase(refs), stmt_count_(stmt_count) {}
bool ParentRelationshipStore::IsNonTransitive(
        Index<ArgPos::kFirst> parent_stmt,
        Index<ArgPos::kSecond> child_stmt) const noexcept {
    if (child_stmt <= parent_stmt || parent_stmt >= stmt_count_ ||
        child_stmt > stmt_count_) {
        return false;
    }
    auto parent_type = type_stmt_.GetType(parent_stmt);
    if (!IsParentType(parent_type)) return false;
    int child_stmtlst = stmtlst_stmt_.GetStmtlst(child_stmt);
    if (parent_type == StmtType::kWhile) {
        return stmtlst_parent_.GetWhileStmtLst(parent_stmt) == child_stmtlst;
    }
    auto [then_idx, else_idx] = stmtlst_parent_.GetIfStmtLst(parent_stmt);
    return child_stmtlst == then_idx || child_stmtlst == else_idx;
}
bool ParentRelationshipStore::IsTransitive(
        Index<ArgPos::kFirst> parent_stmt,
        Index<ArgPos::kSecond> child_stmt) const noexcept {
    StmtNo parent = parent_stmt.value;
    StmtNo child = child_stmt.value;
    if (child <= parent || parent >= stmt_count_ || child > stmt_count_) {
        return false;
    }
    StmtType type = type_stmt_.GetType(parent);
    if (!IsParentType(type)) return false;
    StmtNo last_stmt = GetContainerLastStmt(type, parent);
    return child <= last_stmt;
}
bool ParentRelationshipStore::HasSecondValues(
        Index<ArgPos::kFirst> parent_stmt) const noexcept {
    return parent_stmt < stmt_count_ && IsParent(parent_stmt);
}
bool ParentRelationshipStore::HasFirstValues(
        Index<ArgPos::kSecond> child_stmt) const noexcept {
    return child_stmt <= stmt_count_ && HasParent(child_stmt);
}
bool ParentRelationshipStore::ExistRelationship() const noexcept {
    return !type_stmt_.GetStatements(StmtType::kWhile).empty() ||
           !type_stmt_.GetStatements(StmtType::kIf).empty();
}
std::set<StmtNo> ParentRelationshipStore::GetFirstArg(
        StmtType return_type) const noexcept {
    if (!IsParentType(return_type)) return {};
    if (return_type != StmtType::kAll) {
        const auto &parent_stmts = type_stmt_.GetStatements(return_type);
        return {parent_stmts.begin(), parent_stmts.end()};
    }
    const auto &while_stmts = type_stmt_.GetStatements(StmtType::kWhile);
    const auto &if_stmts = type_stmt_.GetStatements(StmtType::kIf);
    std::set<StmtNo> parents(while_stmts.begin(), while_stmts.end());
    parents.insert(if_stmts.begin(), if_stmts.end());
    return parents;
}
std::set<StmtNo> ParentRelationshipStore::GetSecondArg(
        StmtType return_type) const noexcept {
    std::vector<StmtNo> stmts;
    if (return_type == StmtType::kAll) {
        stmts.resize(stmt_count_);
        std::iota(stmts.begin(), stmts.end(), 1);
    } else {
        stmts = type_stmt_.GetStatements(return_type);
    }
    std::set<StmtNo> children;
    std::copy_if(stmts.begin(), stmts.end(),
                 std::inserter(children, children.end()),
                 [this](StmtNo i) { return HasParent(i); });
    return children;
}
std::set<StmtNo> ParentRelationshipStore::GetSecondArg(
        Index<ArgPos::kFirst> stmt_no, StmtType return_type) const noexcept {
    StmtNo parent = stmt_no.value;
    if (parent >= stmt_count_ || !IsParent(parent)) return {};
    StmtType type = type_stmt_.GetType(parent);
    if (type == StmtType::kWhile) {
        StmtNo stmtlst_index = stmtlst_parent_.GetWhileStmtLst(parent);
        return Filter(stmtlst_stmt_.GetStatements(stmtlst_index), return_type);
    }
    auto [then_idx, else_idx] = stmtlst_parent_.GetIfStmtLst(parent);
    auto then_set = Filter(stmtlst_stmt_.GetStatements(then_idx), return_type);
    auto else_set = Filter(stmtlst_stmt_.GetStatements(else_idx), return_type);
    then_set.insert(else_set.begin(), else_set.end());
    return then_set;
}
std::set<StmtNo> ParentRelationshipStore::GetSecondArgT(
        Index<ArgPos::kFirst> stmt_no, StmtType return_type) const noexcept {
    StmtNo parent = stmt_no.value;
    if (parent >= stmt_count_ || !IsParent(parent)) return {};
    std::vector<StmtNo> results;
    StmtType type = type_stmt_.GetType(parent);
    StmtNo last_stmt = GetContainerLastStmt(type, parent);
    StmtNo num_children = last_stmt - parent;
    results.resize(num_children);
    std::iota(results.begin(), results.end(), parent + 1);
    return Filter(results, return_type);
}
std::set<StmtNo> ParentRelationshipStore::GetFirstArg(
        Index<ArgPos::kSecond> stmt_no, StmtType return_type) const noexcept {
    StmtNo child = stmt_no.value;
    if (!IsParentType(return_type) || child > stmt_count_) return {};
    int stmtlst_index = stmtlst_stmt_.GetStmtlst(child);
    auto parent = stmtlst_parent_.GetParent(stmtlst_index);
    switch (parent.type) {
        case StmtlstParentStore::kNone:
        case StmtlstParentStore::kProc:
            return {};
        case StmtlstParentStore::kIf:
        case StmtlstParentStore::kWhile:
            return Filter(parent.index, return_type);
    }
}
std::set<StmtNo> ParentRelationshipStore::GetFirstArgT(
        Index<ArgPos::kSecond> stmt_no, StmtType return_type) const noexcept {
    StmtNo child = stmt_no.value;
    if (!IsParentType(return_type) || child > stmt_count_ ||
        !HasParent(child)) {
        return {};
    }
    std::vector<StmtNo> results;
    int stmtlst_index = stmtlst_stmt_.GetStmtlst(child);
    auto ancestors = container_forest_->GetAncestryTrace(stmtlst_index);
    ancestors.pop_back();
    for (int a : ancestors) {
        auto type = stmtlst_parent_.GetParent(a).type;
        assert(type == StmtlstParentStore::kWhile ||
               type == StmtlstParentStore::kIf);
        StmtNo parent = stmtlst_parent_.GetParent(a).index;
        results.emplace_back(parent);
    }
    return Filter(results, return_type);
}
PairVec<StmtNo> ParentRelationshipStore::GetBothArgs(
        StmtType parent_type, StmtType child_type) const noexcept {
    if (!IsParentType(parent_type)) return {};
    PairVec<StmtNo> results;
    auto &[parents, children] = results;
    if (parent_type != StmtType::kIf) {
        for (StmtNo while_stmt : type_stmt_.GetStatements(StmtType::kWhile)) {
            int stmtlst_index = stmtlst_parent_.GetWhileStmtLst(while_stmt);
            AddAllChildren(stmtlst_index, children, child_type);
            parents.resize(children.size(), while_stmt);
        }
    }
    if (parent_type != StmtType::kWhile) {
        for (StmtNo if_stmt : type_stmt_.GetStatements(StmtType::kIf)) {
            int then_index = stmtlst_parent_.GetIfStmtLst(if_stmt).then_index;
            AddAllChildren(then_index, children, child_type);
            int else_index = stmtlst_parent_.GetIfStmtLst(if_stmt).else_index;
            AddAllChildren(else_index, children, child_type);
            parents.resize(children.size(), if_stmt);
        }
    }
    return results;
}
void ParentRelationshipStore::AddAllTransitivePairs(
        StmtType parent_type, StmtType child_type,
        PairVec<StmtNo> &results) const noexcept {
    assert(parent_type == StmtType::kWhile || parent_type == StmtType::kIf);
    auto &[parents, children] = results;
    for (StmtNo stmt : type_stmt_.GetStatements(parent_type)) {
        StmtNo end = GetContainerLastStmt(parent_type, stmt);
        AddAllTransitiveChildren(children, child_type, stmt, end);
        parents.resize(children.size(), stmt);
    }
}
void ParentRelationshipStore::AddAllTransitiveChildren(
        std::vector<StmtNo> &children, StmtType child_type, StmtNo parent_stmt,
        StmtNo last_stmt) const {
    int num_children = last_stmt - parent_stmt;
    if (child_type == StmtType::kAll) {
        children.resize(children.size() + num_children);
        std::iota(children.end() - num_children, children.end(),
                  parent_stmt + 1);
        return;
    }
    children.reserve(children.size() + num_children);
    for (StmtNo i = parent_stmt + 1; i <= last_stmt; ++i)
        if (type_stmt_.GetType(i) == child_type) children.emplace_back(i);
}
PairVec<StmtNo> ParentRelationshipStore::GetBothArgsT(
        StmtType parent_type, StmtType child_type) const noexcept {
    if (!IsParentType(parent_type)) return {};
    PairVec<StmtNo> results;
    auto &[parents, children] = results;
    if (parent_type != StmtType::kIf)
        AddAllTransitivePairs(StmtType::kWhile, child_type, results);
    if (parent_type != StmtType::kWhile)
        AddAllTransitivePairs(StmtType::kIf, child_type, results);
    return results;
}
inline bool ParentRelationshipStore::IsParent(StmtNo stmt) const noexcept {
    StmtType type = type_stmt_.GetType(stmt);
    return IsParentType(type);
}
constexpr bool ParentRelationshipStore::IsParentType(StmtType type) noexcept {
    return std::find(kParentTypes.begin(), kParentTypes.end(), type) !=
           kParentTypes.end();
}
inline bool ParentRelationshipStore::HasParent(StmtNo stmt) const noexcept {
    int stmtlst = stmtlst_stmt_.GetStmtlst(stmt);
    auto type = stmtlst_parent_.GetParent(stmtlst).type;
    return type == StmtlstParentStore::ParentType::kWhile ||
           type == StmtlstParentStore::ParentType::kIf;
}
void ParentRelationshipStore::AddAllChildren(
        int stmtlst_index, std::vector<StmtNo> &children,
        StmtType child_type) const noexcept {
    const auto &stmtlst = stmtlst_stmt_.GetStatements(stmtlst_index);
    children.reserve(children.size() + stmtlst.size());
    if (child_type == StmtType::kAll) {
        children.insert(children.end(), stmtlst.begin(), stmtlst.end());
        return;
    }
    std::copy_if(stmtlst.begin(), stmtlst.end(), std::back_inserter(children),
                 [this, child_type](StmtNo stmt_no) {
                     return type_stmt_.GetType(stmt_no) == child_type;
                 });
}
StmtNo ParentRelationshipStore::GetContainerLastStmt(
        StmtType type, StmtNo stmt_no) const noexcept {
    StmtNo last_stmt;
    auto follower =
            stmtlst_stmt_.GetFollowsSecondArg(Index<ArgPos::kFirst>(stmt_no));
    if (follower != 0) return follower - 1;
    StmtNo stmtlst = type == StmtType::kWhile
                             ? stmtlst_parent_.GetWhileStmtLst(stmt_no)
                             : stmtlst_parent_.GetIfStmtLst(stmt_no).else_index;
    auto child_stmts = stmtlst_stmt_.GetStatements(stmtlst);
    const StmtNo last_child = child_stmts.back();
    if (!IsParent(last_child)) return last_child;
    StmtNo grandchild = container_forest_->GetRightmostGrandchild(stmtlst);
    auto grandchild_stmts = stmtlst_stmt_.GetStatements(grandchild);
    last_stmt = grandchild_stmts.back();
    return last_stmt;
}
}  // namespace spa
