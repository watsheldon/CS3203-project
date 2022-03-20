#ifndef SRC_SPA_SRC_PKB_STORE_USES_RELATIONSHIP_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_USES_RELATIONSHIP_STORE_H_

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
class UsesRelationshipStore : public UsesModifiesStoreBase {
  public:
    using UsesModifiesStoreBase::UsesModifiesStoreBase;
    void Set(int stmt_no, std::vector<int>&& var_indices);
    [[nodiscard]] const std::vector<int>& GetVarIndex(int stmt_no) const;

  private:
    void AddConditionRel(const ContainerForest& forest,
                         const StmtlstParentStore& stmtlst_parent,
                         const StmtlstStatementsStore& stmtlst_stmt,
                         const TypeStatementsStore& type_statement_store,
                         PairBitmap& bitmaps) override;
    void AddAllDirectRel(const TypeStatementsStore& store) override;
    void AddAllIndirectRel(const TypeStatementsStore& type_statement_store,
                           const StmtlstStatementsStore& stmtlst_stmt,
                           const StmtlstParentStore& stmtlst_parent,
                           const ContainerForest& forest,
                           PairBitmap& bitmaps) override;

    static constexpr std::array<StmtType, 2> relevant_stmt_types_{
            {StmtType::kAssign, StmtType::kPrint}};
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_USES_RELATIONSHIP_STORE_H_
