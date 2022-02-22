#ifndef SRC_SPA_SRC_QPS_STMT_REF_H_
#define SRC_SPA_SRC_QPS_STMT_REF_H_

#include "synonym.h"

namespace spa {
struct Stmtref {
  private:
    const int stmt_no = 0;
    const Synonym *syn = nullptr;

  public:
    Stmtref(int stmt) : stmt_no(stmt) {}
    Stmtref(const Synonym *syn_ptr) : syn(syn_ptr) {}
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_STMT_REF_H_
