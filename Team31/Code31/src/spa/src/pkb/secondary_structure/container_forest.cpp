#include "container_forest.h"

#include <cassert>
#include <stack>

#include "pkb/store/stmtlst_parent_store.h"
#include "pkb/store/stmtlst_statements_store.h"

namespace spa {
ContainerForest::ContainerForest(
        const spa::StmtlstParentStore& stmtlst_parent_store,
        const spa::StmtlstStatementsStore& stmtlst_stmts_store,
        size_t stmtlst_count)
        : stmtlsts_(stmtlst_count + 1) {
    for (int i = 1; i < stmtlsts_.size(); ++i) {
        const auto& [type, stmt_no] = stmtlst_parent_store.GetParent(i);
        assert(type != StmtlstParentStore::kNone);
        if (type == StmtlstParentStore::kProc) continue;

        auto parent = stmtlst_stmts_store.GetStmtlst(stmt_no);
        stmtlsts_[i].SetParent(parent);
        if (stmtlsts_[parent].GetFirstChild() == 0) {
            stmtlsts_[parent].SetFirstChild(i);
            continue;
        }
        auto sibling = stmtlsts_[parent].GetFirstChild();
        while (stmtlsts_[sibling].GetNextSibling() != 0) {
            sibling = stmtlsts_[sibling].GetNextSibling();
        }
        stmtlsts_[sibling].SetNextSibling(i);
    }
}
bool ContainerForest::IsParentT(const int parent, const int child) const {
    // wildcard queries shouldn't require the use of this forest
    assert(parent != 0 && child != 0);

    auto p = stmtlsts_[child].GetParent();
    if (p == parent) return true;

    while (p != 0) {
        p = stmtlsts_[p].GetParent();
        if (p == parent) {
            return true;
        }
    }
    return false;
}
std::vector<int> ContainerForest::GetParents(const int child) const {
    assert(child != 0);
    std::vector<int> parents;
    int parent = stmtlsts_[child].GetParent();
    while (parent != 0) {
        parents.emplace_back(parent);
        parent = stmtlsts_[parent].GetParent();
    }
    return parents;
}
std::vector<int> ContainerForest::GetChildren(const int parent) const {
    assert(parent != 0);
    std::stack<int> path;
    std::vector<int> children;
    path.emplace(stmtlsts_[parent].GetFirstChild());
    while (!path.empty()) {
        auto child = path.top();
        if (child == 0) {
            path.pop();
            if (path.empty()) break;
            child = path.top();
            path.pop();
            path.emplace(stmtlsts_[child].GetNextSibling());
            continue;
        }
        children.emplace_back(child);
        path.emplace(stmtlsts_[child].GetFirstChild());
    }
    return children;
}
}  // namespace spa
