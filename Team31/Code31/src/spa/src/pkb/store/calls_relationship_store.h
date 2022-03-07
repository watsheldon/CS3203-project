#ifndef SRC_SPA_SRC_PKB_STORE_CALLS_RELATIONSHIP_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_CALLS_RELATIONSHIP_STORE_H_

#include "index_bimap.h"

namespace spa {
class CallsRelationshipStore {
  public:
    CallsRelationshipStore(size_t stmtSize, std::vector<std::set<int>> &&procs);
    void Set(int call_stmt, int proc_index);
    [[nodiscard]] const std::vector<int> &GetCallerProcs(int callee) const;
    [[nodiscard]] const std::vector<int> &GetCalleeProcs(int caller) const;
    [[nodiscard]] const std::vector<int> &GetCalls(int proc_index) const;
    [[nodiscard]] int GetProc(int call_stmt) const;

  private:
    IndexBimap<std::vector<int>> call_stmts_;
    IndexBimap<std::vector<int>> call_procs_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_CALLS_RELATIONSHIP_STORE_H_
