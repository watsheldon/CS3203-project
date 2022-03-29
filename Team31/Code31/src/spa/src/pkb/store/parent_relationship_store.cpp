#include "parent_relationship_store.h"

#include <algorithm>
#include <iterator>
#include <numeric>
#include <set>
#include <utility>
#include <vector>

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "common/index.h"

namespace spa {
ParentRelationshipStore::ParentRelationshipStore(std::size_t stmt_count,
                                                 StoreRefs refs) noexcept
        : stmt_count_(stmt_count),
          type_stmt_(refs.type_stmt),
          stmtlst_parent_(refs.stmtlst_parent),
          stmtlst_stmt_(refs.stmtlst_stmt) {}
bool ParentRelationshipStore::ExistParent(
        Index<ArgPos::kFirst> parent_stmt,
        Index<ArgPos::kSecond> child_stmt) const noexcept {
    StmtNo parent = parent_stmt.value;
    StmtNo child = child_stmt.value;
    if (child <= parent || parent >= stmt_count_ || child > stmt_count_ ||
        !IsParent(parent)) {
        return false;
    }
    StmtNo stmtlst = stmtlst_stmt_.GetStmtlst(child);
    bool is_while = stmtlst_parent_.GetWhileStmtLst(parent) == stmtlst;
    bool is_then = stmtlst_parent_.GetIfStmtLst(parent).then_index == stmtlst;
    bool is_else = stmtlst_parent_.GetIfStmtLst(parent).else_index == stmtlst;
    return is_while || is_then || is_else;
}
bool ParentRelationshipStore::ExistParentT(
        Index<ArgPos::kFirst> parent_stmt,
        Index<ArgPos::kSecond> child_stmt) const noexcept {
    StmtNo parent = parent_stmt.value;
    StmtNo child = child_stmt.value;
    if (child <= parent || parent >= stmt_count_ || child > stmt_count_ ||
        !IsParent(parent)) {
        return false;
    }
    StmtType type = type_stmt_.GetType(parent);
    StmtNo last_stmt = GetContainerLastStmt(type, parent);
    return child <= last_stmt;
}
bool ParentRelationshipStore::ExistParent(
        Index<ArgPos::kFirst> parent_stmt) const noexcept {
    StmtNo parent = parent_stmt.value;
    return parent < stmt_count_ && IsParent(parent);
}
bool ParentRelationshipStore::ExistParent(
        Index<ArgPos::kSecond> child_stmt) const noexcept {
    StmtNo child = child_stmt.value;
    return child <= stmt_count_ && HasParent(child);
}
bool ParentRelationshipStore::ExistParent() const noexcept {
    return !type_stmt_.GetStatements(StmtType::kWhile).empty() ||
           !type_stmt_.GetStatements(StmtType::kIf).empty();
}
std::set<StmtNo> ParentRelationshipStore::GetParent(
        ArgPos return_pos, StmtType return_type) const noexcept {
    return (return_pos == ArgPos::kFirst) ? GetAllParents(return_type)
                                          : GetAllChildren(return_type);
}
std::set<StmtNo> ParentRelationshipStore::GetParent(
        Index<ArgPos::kFirst> parent_stmt,
        StmtType return_type) const noexcept {
    StmtNo parent = parent_stmt.value;
    if (parent >= stmt_count_ || !IsParent(parent)) return {};
    std::vector<StmtNo> results;
    FillChildren(parent, results);
    return Extract(std::move(results), return_type);
}
std::set<StmtNo> ParentRelationshipStore::GetParentT(
        Index<ArgPos::kFirst> parent_stmt,
        StmtType return_type) const noexcept {
    StmtNo parent = parent_stmt.value;
    if (parent >= stmt_count_ || !IsParent(parent)) return {};
    std::vector<StmtNo> results;
    FillChildrenT(parent, results);
    return Extract(std::move(results), return_type);
}
std::set<StmtNo> ParentRelationshipStore::GetParent(
        Index<ArgPos::kSecond> child_stmt,
        StmtType return_type) const noexcept {
    StmtNo child = child_stmt.value;
    if (!IsParent(return_type) || child > stmt_count_ || !HasParent(child)) {
        return {};
    }
    std::vector<StmtNo> results;
    FillParents(child, results);
    return Extract(std::move(results), return_type);
}
std::set<StmtNo> ParentRelationshipStore::GetParentT(
        Index<ArgPos::kSecond> child_stmt,
        StmtType return_type) const noexcept {
    StmtNo child = child_stmt.value;
    if (!IsParent(return_type) || child > stmt_count_ || !HasParent(child)) {
        return {};
    }
    std::vector<StmtNo> results;
    FillParentsT(child, results);
    return Extract(std::move(results), return_type);
}
PairVec<StmtNo> ParentRelationshipStore::GetParentPairs(
        StmtType parent_type, StmtType child_type) const noexcept {
    if (!IsParent(parent_type)) return {};
    PairVec<StmtNo> results;
    FillParentPairs(results);
    return ExtractPairs(std::move(results), parent_type, child_type);
}
PairVec<StmtNo> ParentRelationshipStore::GetParentPairsT(
        StmtType parent_type, StmtType child_type) const noexcept {
    if (!IsParent(parent_type)) return {};
    PairVec<StmtNo> results;
    FillParentTPairs(results);
    return ExtractPairs(std::move(results), parent_type, child_type);
}
std::set<StmtNo> ParentRelationshipStore::Extract(
        std::vector<StmtNo> results, StmtType return_type) const noexcept {
    if (return_type == StmtType::kAll) {
        return {results.begin(), results.end()};
    }
    std::set<StmtNo> extracted_results;
    std::copy_if(results.begin(), results.end(),
                 std::inserter(extracted_results, extracted_results.end()),
                 [this, return_type](StmtNo i) {
                     return type_stmt_.GetType(i) == return_type;
                 });
    return extracted_results;
}
inline bool ParentRelationshipStore::IsParent(StmtNo stmt) const noexcept {
    StmtType type = type_stmt_.GetType(stmt);
    return IsParent(type);
}
inline bool ParentRelationshipStore::IsParent(StmtType type) noexcept {
    return std::find(parent_types_.begin(), parent_types_.end(), type) !=
           parent_types_.end();
}
inline bool ParentRelationshipStore::HasParent(StmtNo stmt) const noexcept {
    StmtNo stmtlst = stmtlst_stmt_.GetStmtlst(stmt);
    StmtlstParentStore::ParentType type =
            stmtlst_parent_.GetParent(stmtlst).type;
    return type == StmtlstParentStore::ParentType::kWhile ||
           type == StmtlstParentStore::ParentType::kIf;
}
std::set<StmtNo> ParentRelationshipStore::GetAllParents(
        StmtType return_type) const noexcept {
    if (!IsParent(return_type)) return {};
    std::vector<StmtNo> parents;
    if (return_type == StmtType::kAll) {
        parents = type_stmt_.GetStatements(StmtType::kWhile);
        auto if_stmts = type_stmt_.GetStatements(StmtType::kIf);
        parents.insert(parents.end(), if_stmts.begin(), if_stmts.end());
    } else {
        parents = type_stmt_.GetStatements(return_type);
    }
    return {parents.begin(), parents.end()};
}
std::set<StmtNo> ParentRelationshipStore::GetAllChildren(
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
void ParentRelationshipStore::FillChildren(
        StmtNo parent, std::vector<StmtNo> &results) const noexcept {
    StmtType type = type_stmt_.GetType(parent);
    if (type == StmtType::kWhile) {
        StmtNo stmtlst_index = stmtlst_parent_.GetWhileStmtLst(parent);
        results = stmtlst_stmt_.GetStatements(stmtlst_index);
    }
    StmtNo then_index = stmtlst_parent_.GetIfStmtLst(parent).then_index;
    StmtNo else_index = stmtlst_parent_.GetIfStmtLst(parent).else_index;
    results = stmtlst_stmt_.GetStatements(then_index);
    auto else_vector = stmtlst_stmt_.GetStatements(else_index);
    results.insert(results.end(), else_vector.begin(), else_vector.end());
}
void ParentRelationshipStore::FillChildrenT(
        StmtNo parent, std::vector<StmtNo> &results) const noexcept {
    StmtType type = type_stmt_.GetType(parent);
    StmtNo last_stmt = GetContainerLastStmt(type, parent);
    StmtNo size = last_stmt - parent;
    results.resize(size);
    std::iota(results.begin(), results.end(), parent + 1);
}
void ParentRelationshipStore::FillParents(
        StmtNo child, std::vector<StmtNo> &results) const noexcept {
    StmtNo stmtlst_index = stmtlst_stmt_.GetStmtlst(child);
    results = {stmtlst_parent_.GetParent(stmtlst_index).index};
}
void ParentRelationshipStore::FillParentsT(
        StmtNo child, std::vector<StmtNo> &results) const noexcept {
    StmtNo stmtlst_index = stmtlst_stmt_.GetStmtlst(child);
    std::vector<StmtNo> ancestors =
            container_forest_->GetAncestryTrace(stmtlst_index);
    for (StmtNo i : ancestors) {
        StmtlstParentStore::ParentType type = stmtlst_parent_.GetParent(i).type;
        if (type != StmtlstParentStore::ParentType::kWhile &&
            type != StmtlstParentStore::ParentType::kIf)
            continue;
        StmtNo parent = stmtlst_parent_.GetParent(i).index;
        results.emplace_back(parent);
    }
}
void ParentRelationshipStore::FillParentTPairs(
        PairVec<StmtNo> &results) const noexcept {
    auto &[parents, children] = results;
    for (StmtNo while_stmt : type_stmt_.GetStatements(StmtType::kWhile)) {
        StmtNo end = GetContainerLastStmt(StmtType::kWhile, while_stmt);
        for (StmtNo i = while_stmt + 1; i <= end; ++i) {
            parents.emplace_back(while_stmt);
            children.emplace_back(i);
        }
    }
    for (StmtNo if_stmt : type_stmt_.GetStatements(StmtType::kIf)) {
        StmtNo end = GetContainerLastStmt(StmtType::kIf, if_stmt);
        for (StmtNo i = if_stmt + 1; i <= end; ++i) {
            parents.emplace_back(if_stmt);
            children.emplace_back(i);
        }
    }
}
void ParentRelationshipStore::FillParentPairs(
        PairVec<StmtNo> &results) const noexcept {
    auto &[parents, children] = results;
    for (StmtNo while_stmt : type_stmt_.GetStatements(StmtType::kWhile)) {
        StmtNo stmtlst_index = stmtlst_parent_.GetWhileStmtLst(while_stmt);
        auto stmtlst = stmtlst_stmt_.GetStatements(stmtlst_index);
        for (StmtNo child_stmt : stmtlst) {
            parents.emplace_back(while_stmt);
            children.emplace_back(child_stmt);
        }
    }
    for (StmtNo if_stmt : type_stmt_.GetStatements(StmtType::kIf)) {
        StmtNo then_index = stmtlst_parent_.GetIfStmtLst(if_stmt).then_index;
        StmtNo else_index = stmtlst_parent_.GetIfStmtLst(if_stmt).else_index;
        auto then_stmtlst = stmtlst_stmt_.GetStatements(then_index);
        auto else_stmtlst = stmtlst_stmt_.GetStatements(else_index);
        for (StmtNo child_stmt : then_stmtlst) {
            parents.emplace_back(if_stmt);
            children.emplace_back(child_stmt);
        }
        for (StmtNo child_stmt : else_stmtlst) {
            parents.emplace_back(if_stmt);
            children.emplace_back(child_stmt);
        }
    }
}
PairVec<StmtNo> ParentRelationshipStore::ExtractPairs(
        PairVec<StmtNo> results, StmtType first,
        StmtType second) const noexcept {
    if (first == StmtType::kAll && second == StmtType::kAll) return results;
    PairVec<StmtNo> extracted_results;
    for (StmtNo i = 0; i < results.first.size(); ++i) {
        bool has_first = first == StmtType::kAll ||
                         first == type_stmt_.GetType(results.first[i]);
        bool has_second = second == StmtType::kAll ||
                          second == type_stmt_.GetType(results.second[i]);
        if (has_first && has_second) {
            extracted_results.first.emplace_back(results.first[i]);
            extracted_results.second.emplace_back(results.second[i]);
        }
    }
    return extracted_results;
}
StmtNo ParentRelationshipStore::GetContainerLastStmt(
        StmtType type, StmtNo stmt_no) const noexcept {
    StmtNo last_stmt;
    auto followers = stmtlst_stmt_.GetFollows(Index<ArgPos::kFirst>(stmt_no));
    if (followers.size() == 1) {
        last_stmt = *followers.begin() - 1;
        return last_stmt;
    }
    StmtNo stmtlst = type == StmtType::kWhile
                             ? stmtlst_parent_.GetWhileStmtLst(stmt_no)
                             : stmtlst_parent_.GetIfStmtLst(stmt_no).else_index;
    auto child_stmts = stmtlst_stmt_.GetStatements(stmtlst);
    const StmtNo last_child = child_stmts.back();
    if (!IsParent(last_child)) {
        return last_child;
    }
    StmtNo grandchild = container_forest_->GetRightmostGrandchild(stmtlst);
    auto grandchild_stmts = stmtlst_stmt_.GetStatements(grandchild);
    last_stmt = grandchild_stmts.back();
    return last_stmt;
}
}  // namespace spa
