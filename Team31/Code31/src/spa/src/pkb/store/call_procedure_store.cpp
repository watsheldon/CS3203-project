#include "call_procedure_store.h"

#include "pkb/knowledge_base.h"

namespace spa {

CallProcedureStore::CallProcedureStore(size_t stmt_count, size_t proc_count)
        : call_to_proc_(stmt_count + 1, 0), proc_to_calls_(proc_count + 1) {}
void CallProcedureStore::Set(int call_stmt, int proc_index) {
    call_to_proc_[call_stmt] = proc_index;
    proc_to_calls_[proc_index].emplace_back(call_stmt);
}
int CallProcedureStore::GetProcIndex(int call_stmt) {
    return call_to_proc_[call_stmt];
}
std::vector<int> CallProcedureStore::GetCalls(int proc_index) {
    return proc_to_calls_[proc_index];
}
}  // namespace spa
