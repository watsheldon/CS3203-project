#ifndef SRC_SPA_SRC_PKB_STORE_USES_MODIFIES_STORE_BASE_H_
#define SRC_SPA_SRC_PKB_STORE_USES_MODIFIES_STORE_BASE_H_

#include <array>
#include <cstddef>
#include <set>
#include <vector>

#include "common/aliases.h"
#include "common/bitvec2d.h"
#include "index_bimap.h"
#include "pkb/secondary_structure/container_forest.h"
#include "stmtlst_parent_store.h"
#include "stmtlst_statements_store.h"
#include "type_statements_store.h"

namespace spa {
class UsesModifiesStoreBase {
  public:
    UsesModifiesStoreBase(std::size_t stmt_size, std::size_t var_size);
    [[nodiscard]] const std::vector<int> &GetStmtNo(int var_index) const;
    [[nodiscard]] const std::set<int> &GetAllVar(int stmt_no) const;
    [[nodiscard]] const std::set<int> &GetAllStmt(int var_index) const;
    [[nodiscard]] PairVec<int> GetStmtVar(StmtType stmt_type) const;
    [[nodiscard]] std::set<int> GetStmt(StmtType stmt_type) const;
    void Compile(const TypeStatementsStore &type_statement_store,
                 const ContainerForest &forest,
                 const StmtlstParentStore &stmtlst_parent,
                 const StmtlstStatementsStore &stmtlst_stmt);

  protected:
    [[nodiscard]] PairVec<int> GetAllRel() const;
    void AddDirectRel(PairVec<int> &stmt_var_pair,
                      const std::vector<int> &stmt_no) const;
    void AddIndirectRel(const PairVec<int> &basic_pairs,
                        const StmtlstStatementsStore &stmtlst_stmt,
                        const StmtlstParentStore &stmtlst_parent,
                        const ContainerForest &forest, BitVec2D &if_added,
                        BitVec2D &while_added);
    void AddAllContainerRel(const ContainerForest &forest,
                            const StmtlstParentStore &stmtlst_parent,
                            const StmtlstStatementsStore &stmtlst_stmt,
                            const TypeStatementsStore &type_statement_store);
    void FillStmts();
    void FillVars();
    void FillRels();
    void CalculateNumRels();

    virtual void AddAllDirectRel(const TypeStatementsStore &store) = 0;
    virtual void AddAllIndirectRel(
            const TypeStatementsStore &type_statement_store,
            const StmtlstStatementsStore &stmtlst_stmt,
            const StmtlstParentStore &stmtlst_parent,
            const ContainerForest &forest, BitVec2D &if_added,
            BitVec2D &while_added) = 0;
    virtual void AddConditionRel(
            const ContainerForest &forest,
            const StmtlstParentStore &stmtlst_parent,
            const StmtlstStatementsStore &stmtlst_stmt,
            const TypeStatementsStore &type_statement_store, BitVec2D &if_added,
            BitVec2D &while_added) = 0;

    template <std::size_t n>
    void FillDirectRels(std::array<StmtType, n> direct_stmt_types,
                        const TypeStatementsStore &type_statement_store) {
        for (const auto stmt_type : direct_stmt_types) {
            auto &relationships =
                    stmt_var_pairs_[static_cast<int>(stmt_type) - 1];
            AddDirectRel(relationships,
                         type_statement_store.GetStatements(stmt_type));
        }
    }
    template <std::size_t n>
    void FillIndirectRels(std::array<StmtType, n> indirect_stmt_types,
                          const TypeStatementsStore &type_statement_store,
                          const StmtlstStatementsStore &stmtlst_stmt,
                          const StmtlstParentStore &stmtlst_parent,
                          const ContainerForest &forest, BitVec2D &if_added,
                          BitVec2D &while_added) {
        for (const auto stmt_type : indirect_stmt_types) {
            auto &relationships =
                    stmt_var_pairs_[static_cast<int>(stmt_type) - 1];
            AddIndirectRel(relationships, stmtlst_stmt, stmtlst_parent, forest,
                           if_added, while_added);
        }
    }

    std::size_t num_stmts;
    std::size_t num_vars;
    std::size_t num_rels;
    IndexBimap<std::vector<int>> stmt_var_;
    IndexBimap<std::set<int>> complete_stmt_var_;
    std::set<int> all_vars_;
    // Read, Print, Call, While, If, Assign
    std::array<PairVec<int>, 6> stmt_var_pairs_;
    // All, Read, Print, Call, While, If, Assign
    std::array<std::set<int>, 7> stmts_arr_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_USES_MODIFIES_STORE_BASE_H_
