#include "calls_relationship_store.h"

#include <deque>

namespace spa {
CallsRelationshipStore::CallsRelationshipStore(
        size_t stmt_count, size_t proc_count,
        std::vector<std::set<ProcIndex>> procs)
        : stmt_proc_(stmt_count, proc_count),
          proc_proc_(proc_count, std::move(procs)),
          proc_proc_t_(proc_count, proc_count) {
    for (int k = 1; k <= proc_count; ++k) {
        if (!proc_proc_.GetKeys(k).empty()) all_callees_.insert(k);
        if (proc_proc_.GetVals(k).empty()) continue;
        all_callers_.insert(k);
        std::deque<int> queue;
        queue.insert(end(queue), begin(proc_proc_.GetVals(k)),
                     end(proc_proc_.GetVals(k)));

        while (!queue.empty()) {
            int v = queue.front();
            proc_proc_t_.Set(k, v);
            queue.pop_front();
            queue.insert(end(queue), begin(proc_proc_.GetVals(v)),
                         end(proc_proc_.GetVals(v)));
        }
    }
    for (auto k : all_callers_) {
        for (auto v : proc_proc_.GetVals(k)) {
            pairs_.first.emplace_back(k);
            pairs_.second.emplace_back(v);
        }
        for (auto v : proc_proc_t_.GetVals(k)) {
            pairs_t_.first.emplace_back(k);
            pairs_t_.second.emplace_back(v);
        }
    }
}
void CallsRelationshipStore::Set(StmtNo call_stmt, ProcIndex proc_index) {
    stmt_proc_.Set(call_stmt, std::move(std::vector<ProcIndex>{proc_index}));
}
bool CallsRelationshipStore::ExistCalls(ProcIndex first_proc,
                                        ProcIndex second_proc) const {
    const auto& callees = GetCalleeProcs(first_proc);
    return callees.find(second_proc) != callees.end();
}
bool CallsRelationshipStore::ExistCallsT(ProcIndex first_proc,
                                         ProcIndex second_proc) const {
    const auto& callees = GetCalleeProcsT(first_proc);
    return callees.find(second_proc) != callees.end();
}
bool CallsRelationshipStore::ExistCalls(ProcIndex first_proc) const {
    return !GetCalleeProcs(first_proc).empty();
}
std::set<ProcIndex> CallsRelationshipStore::GetCalls(ArgPos return_pos) const {
    return return_pos == ArgPos::kFirst ? GetAllCallers() : GetAllCallees();
}
const std::set<ProcIndex>& CallsRelationshipStore::GetCallerProcs(
        ProcIndex callee) const {
    return proc_proc_.GetKeys(callee);
}
const std::set<ProcIndex>& CallsRelationshipStore::GetCalleeProcs(
        ProcIndex caller) const {
    return proc_proc_.GetVals(caller);
}
const std::set<ProcIndex>& CallsRelationshipStore::GetCallerProcsT(
        ProcIndex callee) const {
    return proc_proc_t_.GetKeys(callee);
}
const std::set<ProcIndex>& CallsRelationshipStore::GetCalleeProcsT(
        ProcIndex caller) const {
    return proc_proc_t_.GetVals(caller);
}
const std::set<ProcIndex>& CallsRelationshipStore::GetAllCallers() const {
    return all_callers_;
}
const std::set<ProcIndex>& CallsRelationshipStore::GetAllCallees() const {
    return all_callees_;
}
const PairVec<ProcIndex>& CallsRelationshipStore::GetCallsPairs() const {
    return pairs_;
}
const PairVec<ProcIndex>& CallsRelationshipStore::GetCallsTPairs() const {
    return pairs_t_;
}
const std::vector<StmtNo>& CallsRelationshipStore::GetStmts(
        ProcIndex proc_index) const {
    return stmt_proc_.GetKeys(proc_index);
}
ProcIndex CallsRelationshipStore::GetProc(StmtNo call_stmt) const {
    return stmt_proc_.GetVals(call_stmt)[0];
}

}  // namespace spa
