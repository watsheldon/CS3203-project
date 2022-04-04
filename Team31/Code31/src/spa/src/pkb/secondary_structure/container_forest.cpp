#include "container_forest.h"

#include <cassert>
#include <stack>

#include "common/aliases.h"
#include "pkb/store/stmtlst_parent_store.h"
#include "pkb/store/stmtlst_statements_store.h"

namespace spa {
ContainerForest::ContainerForest(
        const spa::StmtlstParentStore& stmtlst_parent_store,
        const spa::StmtlstStatementsStore& stmtlst_stmts_store,
        size_t stmtlst_count) noexcept
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
bool ContainerForest::IsParentT(const int parent,
                                const int child) const noexcept {
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
std::vector<int> ContainerForest::GetAncestryTrace(
        const int child) const noexcept {
    assert(child != 0);
    std::vector<int> ancestry = {child};
    int parent = stmtlsts_[child].GetParent();
    while (parent != 0) {
        ancestry.emplace_back(parent);
        parent = stmtlsts_[parent].GetParent();
    }
    return ancestry;
}
StmtLstIndex ContainerForest::GetRoot(StmtLstIndex child) const noexcept {
    StmtLstIndex father = stmtlsts_[child].GetParent();
    while (father != 0) {
        child = father;
        father = stmtlsts_[child].GetParent();
    }
    return child;
}
bool ContainerForest::SameProcedure(StmtLstIndex a,
                                    StmtLstIndex b) const noexcept {
    assert(a != 0 && b != 0);
    return GetRoot(a) == GetRoot(b);
}
std::vector<int> ContainerForest::GetChildren(const int parent) const noexcept {
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
int ContainerForest::GetRightmostGrandchild(
        const int grandparent) const noexcept {
    assert(grandparent != 0);
    auto grandchild = grandparent;
    while (stmtlsts_[grandchild].GetFirstChild()) {
        grandchild = stmtlsts_[grandchild].GetFirstChild();
        while (stmtlsts_[grandchild].GetNextSibling()) {
            grandchild = stmtlsts_[grandchild].GetNextSibling();
        }
    }
    return grandchild;
}
}  // namespace spa
