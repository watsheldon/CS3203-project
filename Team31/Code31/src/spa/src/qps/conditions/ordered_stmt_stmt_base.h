#ifndef SRC_SPA_SRC_QPS_CONDITIONS_ORDERED_STMT_STMT_BASE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_ORDERED_STMT_STMT_BASE_H_

#include "stmt_stmt_base.h"

namespace spa {
class OrderedStmtStmtBase : public StmtStmtBase {
  public:
    using StmtStmtBase::StmtStmtBase;
    ~OrderedStmtStmtBase() override = default;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_CONDITIONS_ORDERED_STMT_STMT_BASE_H_
