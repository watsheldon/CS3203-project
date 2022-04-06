#include "modifies_relationship_store.h"

#include <vector>

#include "type_statements_store.h"

namespace spa {
void ModifiesRelationshipStore::Set(int stmt_no, int var_index) noexcept {
    stmt_var_.Set(stmt_no, std::vector<int>{var_index});
}
void ModifiesRelationshipStore::AddAllIndirectRel(
        const AuxiliaryData& data_store) noexcept {
    FillIndirectRels(indirect_stmt_types_, data_store);
}
void ModifiesRelationshipStore::AddAllDirectRel(
        const TypeStatementsStore& store) noexcept {
    FillDirectRels(direct_stmt_types_, store);
}
bool ModifiesRelationshipStore::ExistModifies(int stmt_no,
                                              int var_index) const noexcept {
    return ExistRel(stmt_no, var_index);
}
std::set<int> ModifiesRelationshipStore::GetModifies(
        int stmt_no) const noexcept {
    return GetRelRelatedVars(stmt_no);
}
std::set<int> ModifiesRelationshipStore::GetModifies(
        int var_index, StmtType type,
        const TypeStatementsStore& store) const noexcept {
    return GetRelRelatedVars(var_index, type, store);
}
bool ModifiesRelationshipStore::ExistModifiesP(int proc_index,
                                               int var_index) const noexcept {
    return ExistRelP(proc_index, var_index);
}
bool ModifiesRelationshipStore::ExistModifiesP(int proc_index) const noexcept {
    return ExistRelP(proc_index);
}
}  // namespace spa
