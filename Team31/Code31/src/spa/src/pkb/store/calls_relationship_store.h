#ifndef SRC_SPA_SRC_PKB_STORE_CALLS_RELATIONSHIP_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_CALLS_RELATIONSHIP_STORE_H_

#include "common/entity_type_enum.h"
#include "index_bimap.h"

namespace spa {
class CallsRelationshipStore {
  public:
    CallsRelationshipStore(size_t stmt_count, size_t proc_count,
                           std::vector<std::set<int>> procs);
    void Set(int call_stmt, int proc_index);
    [[nodiscard]] bool ExistCalls(int first_proc, int second_proc) const;
    [[nodiscard]] bool ExistCallsT(int first_proc, int second_proc) const;
    [[nodiscard]] bool ExistCalls(int first_proc) const;
    [[nodiscard]] std::set<int> GetCalls(ArgPos return_pos) const;
    [[nodiscard]] const std::set<int> &GetCallerProcs(int callee) const;
    [[nodiscard]] const std::set<int> &GetCalleeProcs(int caller) const;
    [[nodiscard]] const std::set<int> &GetCallerProcsT(int callee) const;
    [[nodiscard]] const std::set<int> &GetCalleeProcsT(int caller) const;
    [[nodiscard]] const std::set<int> &GetAllCallers() const;
    [[nodiscard]] const std::set<int> &GetAllCallees() const;
    [[nodiscard]] const PairVec<int> &GetCallsPairs() const;
    [[nodiscard]] const PairVec<int> &GetCallsTPairs() const;
    [[nodiscard]] const std::vector<int> &GetStmts(int proc_index) const;
    [[nodiscard]] int GetProc(int call_stmt) const;

  private:
    IndexBimap<std::vector<int>> stmt_proc_;
    IndexBimap<std::set<int>> proc_proc_;
    IndexBimap<std::set<int>> proc_proc_t_;
    std::set<int> all_callees_;
    std::set<int> all_callers_;
    PairVec<int> pairs_;
    PairVec<int> pairs_t_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_CALLS_RELATIONSHIP_STORE_H_
