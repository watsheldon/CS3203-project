#include "modifies_relationship_store.h"

#include <cassert>
#include <vector>

#include "type_statements_store.h"

namespace spa {
void ModifiesRelationshipStore::Set(int stmt_no, int var_index) {
    stmt_var_.Set(stmt_no, std::vector<int>{var_index});
}
void ModifiesRelationshipStore::AddConditionRel(
        const AuxiliaryData& data_store) {}
void ModifiesRelationshipStore::AddAllIndirectRel(
        const AuxiliaryData& data_store) {
    FillIndirectRels(indirect_stmt_types_, data_store);
}
void ModifiesRelationshipStore::AddAllDirectRel(
        const TypeStatementsStore& store) {
    FillDirectRels(direct_stmt_types_, store);
}
bool ModifiesRelationshipStore::ExistModifies(int stmt_no, int var_index) {
    assert(stmt_no != 0);
    if (stmt_no > num_stmts_) {
        return false;
    }
    auto modified_vars = GetAllVar(stmt_no);
    return var_index == 0
                   ? !modified_vars.empty()
                   : modified_vars.find(var_index) != modified_vars.end();
}
std::set<int> ModifiesRelationshipStore::GetModifies(int stmt_no) {
    return stmt_no > num_stmts_ ? std::set<int>() : GetAllVar(stmt_no);
}
std::set<int> ModifiesRelationshipStore::GetModifies(
        int var_index, StmtType type, const TypeStatementsStore& store) {
    assert(var_index != 0);
    auto stmts = GetAllStmt(var_index);
    if (type == StmtType::kAll) {
        return stmts;
    }
    for (auto it = stmts.begin(); it != stmts.end();) {
        if (store.GetType(*it) != type) {
            it = stmts.erase(it);
        } else {
            ++it;
        }
    }
    return stmts;
}
bool ModifiesRelationshipStore::ExistModifiesP(int proc_index, int var_index) {
    auto modified_vars = GetVarAccessByProc(proc_index);
    return modified_vars.find(var_index) != modified_vars.end();
}
bool ModifiesRelationshipStore::ExistModifiesP(int proc_index) {
    auto modified_vars = GetVarAccessByProc(proc_index);
    return !modified_vars.empty();
}
}  // namespace spa
