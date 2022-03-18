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
#include "uses_modifies.h"

namespace spa {
class UsesRelationshipStore : public UsesModifies {
  public:
    UsesRelationshipStore(std::size_t stmt_size, std::size_t var_size);
    void Set(int stmt_no, std::vector<int> &&var_indices);
    [[nodiscard]] const std::vector<int> &GetVarIndex(int stmt_no) const;
    void Compile(const TypeStatementsStore &type_statement_store,
                 const ContainerForest &forest,
                 const StmtlstParentStore &stmtlst_parent,
                 const StmtlstStatementsStore &stmtlst_stmt) override;

  private:
    void CompileContainers(
            const ContainerForest &forest,
            const StmtlstParentStore &stmtlst_parent,
            const StmtlstStatementsStore &stmtlst_stmt,
            const TypeStatementsStore &type_statement_store) override;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_USES_RELATIONSHIP_STORE_H_
