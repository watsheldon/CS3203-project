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
    struct ContainerInfo {
        const ContainerForest &forest;
        const StmtlstParentStore &stmtlst_parent;
        const StmtlstStatementsStore &stmtlst_stmt;
    };
    UsesModifiesStoreBase(std::size_t stmt_size, std::size_t var_size,
                          std::size_t proc_size);
    [[nodiscard]] bool ExistRel(int stmt_no, int var_index) const;
    [[nodiscard]] bool ExistRelP(int proc_index) const;
    [[nodiscard]] bool ExistRelP(int proc_index, int var_index) const;
    [[nodiscard]] std::set<int> GetRelRelatedVars(int stmt_no) const;
    [[nodiscard]] std::set<int> GetRelRelatedVars(
            int var_index, StmtType type,
            const TypeStatementsStore &store) const;
    [[nodiscard]] const std::vector<int> &GetStmtNo(int var_index) const;
    [[nodiscard]] const std::set<int> &GetAllVar(int stmt_no) const;
    [[nodiscard]] const std::set<int> &GetVarAccessByProc(int proc_index) const;
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
    struct ContainerAddedTrackers {
        BitVec2D &if_added;
        BitVec2D &while_added;
        BitVec2D &proc_added;
    };
    struct AuxiliaryData {
        const TypeStatementsStore &type_statement_store;
        const ContainerInfo &container_info;
        ContainerAddedTrackers &bitmaps;
    };

    [[nodiscard]] PairVec<int> GetAllRel() const;
    void AddDirectRel(PairVec<int> &stmt_var_pair,
                      const std::vector<int> &stmt_no) const;
    void AddIndirectRel(const PairVec<int> &basic_pairs,
                        const ContainerInfo &info,
                        ContainerAddedTrackers &bitmaps);
    void AddAllContainerRel(const TypeStatementsStore &type_statement_store,
                            const ContainerInfo &info,
                            const CallsRelationshipStore &calls_rel_store);
    void AddCallStmtVar(PairVec<int> &stmt_var_pair,
                        const std::vector<int> &stmt_no,
                        const CallsRelationshipStore &calls_rel_store) const;
    void ProcessProcedureAncestor(int proc_index, int var_index,
                                  BitVec2D &proc_added);
    void FillStmts();
    void FillVars();
    void FillRels();
    void FillProcs();
    void CalculateNumRels();

    virtual void AddAllDirectRel(const TypeStatementsStore &store) = 0;
    virtual void AddAllIndirectRel(const AuxiliaryData &data_store) = 0;
    virtual void AddConditionRel(const AuxiliaryData &data_store) = 0;
    virtual void AddPatternRelated(StmtType type, int stmt_no) = 0;

    template <std::size_t n>
    void FillDirectRels(std::array<StmtType, n> direct_stmt_types,
                        const TypeStatementsStore &type_statement_store) {
        for (const auto stmt_type : direct_stmt_types) {
            auto &relationships = stmt_var_pairs_[StmtTypeToIndex(stmt_type)];
            AddDirectRel(relationships,
                         type_statement_store.GetStatements(stmt_type));
        }
    }
    template <std::size_t n>
    void FillIndirectRels(std::array<StmtType, n> indirect_stmt_types,
                          const AuxiliaryData &data_store) {
        auto &[type_stmt, cont_info, bitmaps] = data_store;
        auto &[forest, stmtlst_parent, stmtlst_stmt] = cont_info;
        for (const auto stmt_type : indirect_stmt_types) {
            auto &relationships = stmt_var_pairs_[StmtTypeToIndex(stmt_type)];
            AddIndirectRel(relationships, cont_info, bitmaps);
        }
    }
    static constexpr int StmtTypeToIndex(StmtType type) {
        return static_cast<int>(type) - 1;
    }
    static constexpr std::array<StmtType, 2> kContainerStmtTypes{
            StmtType::kIf, StmtType::kWhile};
    static constexpr std::array<StmtType, 3> kIndirectStmtTypes{
            StmtType::kIf, StmtType::kWhile, StmtType::kCall};

    std::size_t num_stmts_;
    std::size_t num_vars_;
    std::size_t num_procs_;
    std::size_t num_rels_{};
    IndexBimap<std::vector<int>> stmt_var_;
    IndexBimap<std::set<int>> complete_stmt_var_;
    IndexBimap<std::set<int>> proc_vars_;
    PairVec<int> proc_var_pair_;
    std::set<int> all_procs_;
    std::set<int> all_vars_;
    // Read, Print, Call, While, If, Assign
    std::array<PairVec<int>, 6> stmt_var_pairs_;
    // All, Read, Print, Call, While, If, Assign
    std::array<std::set<int>, 7> stmts_arr_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_USES_MODIFIES_STORE_BASE_H_
