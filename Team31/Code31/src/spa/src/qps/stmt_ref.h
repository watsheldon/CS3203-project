#ifndef SPA_SRC_SPA_SRC_QPS_STMT_REF_H_
#define SPA_SRC_SPA_SRC_QPS_STMT_REF_H_

#include "declaration_cl.h"
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

#endif  // SPA_SRC_SPA_SRC_QPS_STMT_REF_H_
