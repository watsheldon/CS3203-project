#include "modifies_relationship_store.h"

#include <vector>
#include <cassert>

#include "type_statements_store.h"

namespace spa {
void ModifiesRelationshipStore::Set(StmtNo stmt_no,
                                    VarIndex var_index) noexcept {
    stmt_var_.Set(stmt_no, std::vector<VarIndex>{var_index});
}
void ModifiesRelationshipStore::AddAllIndirectRel(
        const AuxiliaryData& data_store) noexcept {
    FillIndirectRels(indirect_stmt_types_, data_store);
}
void ModifiesRelationshipStore::AddAllDirectRel(
        const TypeStatementsStore& store) noexcept {
    FillDirectRels(direct_stmt_types_, store);
}
bool ModifiesRelationshipStore::ExistModifies(
        StmtNo stmt_no, VarIndex var_index) const noexcept {
    return ExistRel(stmt_no, var_index);
}
std::set<VarIndex> ModifiesRelationshipStore::GetModifies(
        StmtNo stmt_no) const noexcept {
    return GetRelRelatedVars(stmt_no);
}
std::set<StmtNo> ModifiesRelationshipStore::GetModifies(
        VarIndex var_index, StmtType type,
        const TypeStatementsStore& store) const noexcept {
    return GetRelRelatedVars(var_index, type, store);
}
bool ModifiesRelationshipStore::ExistModifiesP(
        ProcIndex proc_index, VarIndex var_index) const noexcept {
    return ExistRelP(proc_index, var_index);
}
bool ModifiesRelationshipStore::ExistModifiesP(
        ProcIndex proc_index) const noexcept {
    return ExistRelP(proc_index);
}
VarIndex ModifiesRelationshipStore::GetModifiesSingleVar(
        StmtNo stmt_no) const noexcept {
    const auto& modifies = complete_stmt_var_.GetVals(stmt_no);
    assert(!modifies.empty());
    return *modifies.begin();
}
}  // namespace spa
