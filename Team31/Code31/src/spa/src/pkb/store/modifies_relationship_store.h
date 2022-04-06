#ifndef SRC_SPA_SRC_PKB_STORE_MODIFIES_RELATIONSHIP_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_MODIFIES_RELATIONSHIP_STORE_H_

#include <cstddef>
#include <set>
#include <vector>

#include "common/bitvec2d.h"
#include "index_bimap.h"
#include "pkb/secondary_structure/container_forest.h"
#include "stmtlst_parent_store.h"
#include "stmtlst_statements_store.h"
#include "type_statements_store.h"
#include "uses_modifies_store_base.h"

namespace spa {
class ModifiesRelationshipStore : public UsesModifiesStoreBase {
  public:
    using UsesModifiesStoreBase::UsesModifiesStoreBase;
    void Set(StmtNo stmt_no, VarIndex var_index) noexcept;
    [[nodiscard]] bool ExistModifies(StmtNo stmt_no,
                                     VarIndex var_index) const noexcept;
    [[nodiscard]] std::set<VarIndex> GetModifies(StmtNo stmt_no) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetModifies(
            VarIndex var_index, StmtType type,
            const TypeStatementsStore &store) const noexcept;
    [[nodiscard]] bool ExistModifiesP(ProcIndex proc_index,
                                      VarIndex var_index) const noexcept;
    [[nodiscard]] bool ExistModifiesP(ProcIndex proc_index) const noexcept;

  private:
    void AddAllDirectRel(const TypeStatementsStore &store) noexcept override;
    void AddAllIndirectRel(const AuxiliaryData &data_store) noexcept override;
    static constexpr std::array<StmtType, 2> direct_stmt_types_{
            {StmtType::kAssign, StmtType::kRead}};
    static constexpr std::array<StmtType, 4> indirect_stmt_types_{
            {StmtType::kAssign, StmtType::kRead, StmtType::kIf,
             StmtType::kWhile}};
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_MODIFIES_RELATIONSHIP_STORE_H_
