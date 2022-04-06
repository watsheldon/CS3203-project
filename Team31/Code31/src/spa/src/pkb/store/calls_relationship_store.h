#ifndef SRC_SPA_SRC_PKB_STORE_CALLS_RELATIONSHIP_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_CALLS_RELATIONSHIP_STORE_H_

#include "common/entity_type_enum.h"
#include "index_bimap.h"

namespace spa {
class CallsRelationshipStore {
  public:
    CallsRelationshipStore(size_t stmt_count, size_t proc_count,
                           std::vector<std::set<ProcIndex>> procs);
    void Set(StmtNo call_stmt, ProcIndex proc_index);
    [[nodiscard]] bool ExistCalls(ProcIndex first_proc,
                                  ProcIndex second_proc) const;
    [[nodiscard]] bool ExistCallsT(ProcIndex first_proc,
                                   ProcIndex second_proc) const;
    [[nodiscard]] bool ExistCalls(ProcIndex first_proc) const;
    [[nodiscard]] std::set<ProcIndex> GetCalls(ArgPos return_pos) const;
    [[nodiscard]] const std::set<ProcIndex> &GetCallerProcs(
            ProcIndex callee) const;
    [[nodiscard]] const std::set<ProcIndex> &GetCalleeProcs(
            ProcIndex caller) const;
    [[nodiscard]] const std::set<ProcIndex> &GetCallerProcsT(
            ProcIndex callee) const;
    [[nodiscard]] const std::set<ProcIndex> &GetCalleeProcsT(
            ProcIndex caller) const;
    [[nodiscard]] const std::set<ProcIndex> &GetAllCallers() const;
    [[nodiscard]] const std::set<ProcIndex> &GetAllCallees() const;
    [[nodiscard]] const PairVec<ProcIndex> &GetCallsPairs() const;
    [[nodiscard]] const PairVec<ProcIndex> &GetCallsTPairs() const;
    [[nodiscard]] const std::vector<StmtNo> &GetStmts(
            ProcIndex proc_index) const;
    [[nodiscard]] ProcIndex GetProc(StmtNo call_stmt) const;

  private:
    IndexBimap<std::vector<int>> stmt_proc_;
    IndexBimap<std::set<int>> proc_proc_;
    IndexBimap<std::set<int>> proc_proc_t_;
    std::set<ProcIndex> all_callees_;
    std::set<ProcIndex> all_callers_;
    PairVec<ProcIndex> pairs_;
    PairVec<ProcIndex> pairs_t_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_CALLS_RELATIONSHIP_STORE_H_
