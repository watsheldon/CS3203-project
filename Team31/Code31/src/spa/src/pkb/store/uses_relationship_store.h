#ifndef SRC_SPA_SRC_PKB_STORE_USES_RELATIONSHIP_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_USES_RELATIONSHIP_STORE_H_

#include <cstddef>
#include <vector>
#include <set>

#include "common/bitvec2d.h"
#include "index_bimap.h"
#include "pkb/secondary_structure/container_forest.h"
#include "stmtlst_parent_store.h"
#include "stmtlst_statements_store.h"
#include "type_statements_store.h"

namespace spa {
class UsesRelationshipStore {
  public:
    UsesRelationshipStore(std::size_t stmt_size, std::size_t var_size);
    void Set(int stmt_no, std::vector<int> &&var_indices);
    [[nodiscard]] const std::vector<int> &GetVarIndex(int stmt_no) const;
    [[nodiscard]] const std::set<int> &GetAllVar(int stmt_no) const;
    [[nodiscard]] const std::vector<int> &GetStmtNo(int var_index) const;
    [[nodiscard]] const std::set<int> &GetAllStmt(int var_index) const;
    void Compile(const TypeStatementsStore &type_statement_store,
                 const ContainerForest &forest,
                 const StmtlstParentStore &stmtlst_parent,
                 const StmtlstStatementsStore &stmtlst_stmt);
    [[nodiscard]] PairVec<int> GetStmtVar(StmtType stmt_type) const;
    [[nodiscard]] std::set<int> GetStmt(StmtType stmt_type) const;

  private:
    std::size_t num_stmts;
    std::size_t num_vars;
    IndexBimap<std::vector<int>> stmt_var_;
    IndexBimap<std::set<int>> complete_stmt_var_;
    PairVec<int> assign_var_pairs_;
    PairVec<int> read_var_pairs_;
    PairVec<int> print_var_pairs_;
    PairVec<int> if_var_pairs_;
    PairVec<int> while_var_pairs_;
    PairVec<int> call_var_pairs_;
    std::set<int> assign_stmts_;
    std::set<int> print_stmts_;
    std::set<int> if_stmts_;
    std::set<int> while_stmts_;
    std::set<int> all_stmts_;

    void CompileBasic(PairVec<int> &stmt_var_pair,
                      const std::vector<int> &stmt_no) const;
    void CompileContainers(const ContainerForest &forest,
                           const StmtlstParentStore &stmtlst_parent,
                           const StmtlstStatementsStore &stmtlst_stmt,
                           const TypeStatementsStore &type_statement_store);
    [[nodiscard]] PairVec<int> Combine() const;
    void AddAncestorsOnly(const PairVec<int> &basic_pairs,
                          const StmtlstStatementsStore &stmtlst_stmt,
                          const StmtlstParentStore &stmtlst_parent,
                          const ContainerForest &forest, BitVec2D &if_added,
                          BitVec2D &while_added);
    void AggregateStmts();
    void UpdateStmtVar();
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_USES_RELATIONSHIP_STORE_H_
