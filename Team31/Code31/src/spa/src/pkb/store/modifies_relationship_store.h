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
    ModifiesRelationshipStore(std::size_t stmt_size, std::size_t var_size);
    void Set(int stmt_no, int var_index);
    [[nodiscard]] int GetVarIndex(int stmt_no) const;
    void Compile(const TypeStatementsStore &type_statement_store,
                 const ContainerForest &forest,
                 const StmtlstParentStore &stmtlst_parent,
                 const StmtlstStatementsStore &stmtlst_stmt) override;

  private:
    void AddContainerRel(
            const ContainerForest &forest,
            const StmtlstParentStore &stmtlst_parent,
            const StmtlstStatementsStore &stmtlst_stmt,
            const TypeStatementsStore &type_statement_store) override;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_MODIFIES_RELATIONSHIP_STORE_H_
