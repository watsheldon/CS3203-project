#ifndef SRC_SPA_SRC_PKB_STORE_USES_MODIFIES_STORE_BASE_H_
#define SRC_SPA_SRC_PKB_STORE_USES_MODIFIES_STORE_BASE_H_

#include <array>
#include <cstddef>
#include <set>
#include <vector>

#include "calls_relationship_store.h"
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
    struct ContainerBitmap {
        BitVec2D &if_added;
        BitVec2D &while_added;
        BitVec2D &proc_added;
    };
    struct ContainerInfo {
        const ContainerForest &forest;
        const StmtlstParentStore &stmtlst_parent;
        const StmtlstStatementsStore &stmtlst_stmt;
    };

    UsesModifiesStoreBase(std::size_t stmt_size, std::size_t var_size,
                          std::size_t proc_size);
    [[nodiscard]] const std::vector<int> &GetStmtNo(int var_index) const;
    [[nodiscard]] const std::set<int> &GetAllVar(int stmt_no) const;
    [[nodiscard]] const std::set<int> &GetAllVarProc(int proc_index) const;
    [[nodiscard]] const std::set<int> &GetAllStmt(int var_index) const;
    [[nodiscard]] const std::set<int> &GetAllProc(int var_index) const;
    [[nodiscard]] const std::set<int> &GetAllProc() const;
    [[nodiscard]] PairVec<int> GetStmtVar(StmtType stmt_type) const;
    [[nodiscard]] PairVec<int> GetProcVar() const;
    [[nodiscard]] std::set<int> GetStmt(StmtType stmt_type) const;
    void Compile(const TypeStatementsStore &type_statement_store,
                 const ContainerInfo &info,
                 const CallsRelationshipStore &calls_rel_store);

  protected:
    [[nodiscard]] PairVec<int> GetAllRel() const;
    void AddDirectRel(PairVec<int> &stmt_var_pair,
                      const std::vector<int> &stmt_no) const;
    void AddIndirectRel(const PairVec<int> &basic_pairs,
                        const ContainerInfo &info, ContainerBitmap &bitmaps);
    void AddAllContainerRel(const TypeStatementsStore &type_statement_store,
                            const ContainerInfo &info,
                            const CallsRelationshipStore &calls_rel_store);
    void AddCallStmtVar(PairVec<int> &stmt_var_pair,
                        const std::vector<int> &stmt_no,
                        const CallsRelationshipStore &calls_rel_store) const;
    void ProcessProcedureAncestor(int proc_index, int var_index,
                                  BitVec2D &proc_added) {
        if (proc_added.At(proc_index, var_index)) return;
        proc_added.Set(proc_index, var_index);
        proc_vars_.Set(proc_index, var_index);
        auto &[procs, vars] = proc_var_pair_;
        vars.emplace_back(var_index);
        procs.emplace_back(proc_index);
    }
    void FillStmts();
    void FillVars();
    void FillRels();
    void FillProcs();
    void CalculateNumRels();

    virtual void AddAllDirectRel(const TypeStatementsStore &store) = 0;
    virtual void AddAllIndirectRel(
            const TypeStatementsStore &type_statement_store,
            const ContainerInfo &info, ContainerBitmap &bitmaps) = 0;
    virtual void AddConditionRel(
            const TypeStatementsStore &type_statement_store,
            const ContainerInfo &info, ContainerBitmap &bitmaps) = 0;

    template <std::size_t n>
    void FillDirectRels(std::array<StmtType, n> direct_stmt_types,
                        const TypeStatementsStore &type_statement_store) {
        for (const auto stmt_type : direct_stmt_types) {
            auto &relationships = stmt_var_pairs_[ConvertStmtType(stmt_type)];
            AddDirectRel(relationships,
                         type_statement_store.GetStatements(stmt_type));
        }
    }
    template <std::size_t n>
    void FillIndirectRels(std::array<StmtType, n> indirect_stmt_types,
                          const TypeStatementsStore &type_statement_store,
                          const ContainerInfo &info, ContainerBitmap &bitmaps) {
        auto &[forest, stmtlst_parent, stmtlst_stmt] = info;
        for (const auto stmt_type : indirect_stmt_types) {
            auto &relationships = stmt_var_pairs_[ConvertStmtType(stmt_type)];
            AddIndirectRel(relationships, info, bitmaps);
        }
    }
    int ConvertStmtType(StmtType type) const {
        return static_cast<int>(type) - 1;
    }

    std::size_t num_stmts;
    std::size_t num_vars;
    std::size_t num_rels;
    std::size_t num_procs;
    IndexBimap<std::vector<int>> stmt_var_;
    IndexBimap<std::set<int>> complete_stmt_var_;
    IndexBimap<std::set<int>> proc_vars_;
    PairVec<int> proc_var_pair_;
    std::set<int> all_procs_;
    std::set<int> all_vars_;
    static constexpr std::array<StmtType, 3> KIndirect_stmt_types_{
            {StmtType::kIf, StmtType::kWhile, StmtType::kCall}};
    // Read, Print, Call, While, If, Assign
    std::array<PairVec<int>, 6> stmt_var_pairs_;
    // All, Read, Print, Call, While, If, Assign
    std::array<std::set<int>, 7> stmts_arr_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_USES_MODIFIES_STORE_BASE_H_
