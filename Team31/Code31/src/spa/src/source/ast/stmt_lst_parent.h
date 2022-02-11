#ifndef SRC_SPA_SRC_SOURCE_AST_STMT_LST_PARENT_H_
#define SRC_SPA_SRC_SOURCE_AST_STMT_LST_PARENT_H_

#include "stmt_lst_node.h"

namespace spa {
class StmtLstParent {
  public:
    virtual void AddStmtLst(const StmtLstNode *) = 0;
    virtual ~StmtLstParent() = 0;
};
inline StmtLstParent::~StmtLstParent() = default;
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_AST_STMT_LST_PARENT_H_
