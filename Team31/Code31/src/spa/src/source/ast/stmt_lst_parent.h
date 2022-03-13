#ifndef SRC_SPA_SRC_SOURCE_AST_STMT_LST_PARENT_H_
#define SRC_SPA_SRC_SOURCE_AST_STMT_LST_PARENT_H_

#include "stmt_lst_node.h"

namespace spa {
class StmtLstParent {
  public:
    StmtLstParent() noexcept = default;
    virtual void AddStmtLst(const StmtLstNode *) noexcept = 0;
    virtual ~StmtLstParent() = 0;
};
inline StmtLstParent::~StmtLstParent() = default;
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_AST_STMT_LST_PARENT_H_
