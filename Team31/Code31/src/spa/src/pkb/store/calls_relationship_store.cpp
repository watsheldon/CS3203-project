#include "calls_relationship_store.h"

namespace spa {
CallsRelationshipStore::CallsRelationshipStore(
        size_t stmt_count, std::vector<std::set<int>>&& procs)
        : call_stmts_(IndexBimap<std::vector<int>>(stmt_count, procs.size())),
          call_procs_(procs.size(), procs.size()) {
    for (int i = 1; i < procs.size(); i++) {
        std::vector<int> v(procs[i].begin(), procs[i].end());
        call_procs_.Set(i, std::move(v));
    }
}
void CallsRelationshipStore::Set(int call_stmt, int proc_index) {
    call_stmts_.Set(call_stmt, std::move(std::vector<int>{proc_index}));
}
const std::vector<int>& CallsRelationshipStore::GetCallerProcs(
        int callee) const {
    return call_procs_.GetKeys(callee);
}
const std::vector<int>& CallsRelationshipStore::GetCalleeProcs(
        int caller) const {
    return call_procs_.GetVals(caller);
}
const std::vector<int>& CallsRelationshipStore::GetCalls(int proc_index) const {
    return call_stmts_.GetKeys(proc_index);
}
int CallsRelationshipStore::GetProc(int call_stmt) const {
    return call_stmts_.GetVals(call_stmt)[0];
}

}  // namespace spa
