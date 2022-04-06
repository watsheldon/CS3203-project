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
                          std::size_t proc_size) noexcept;
    [[nodiscard]] bool ExistRel(StmtNo stmt_no,
                                VarIndex var_index) const noexcept;
    [[nodiscard]] bool ExistRelP(ProcIndex proc_index) const noexcept;
    [[nodiscard]] bool ExistRelP(ProcIndex proc_index,
                                 VarIndex var_index) const noexcept;
    [[nodiscard]] std::set<VarIndex> GetRelRelatedVars(
            StmtNo stmt_no) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetRelRelatedVars(
            VarIndex var_index, StmtType type,
            const TypeStatementsStore &store) const noexcept;
    [[nodiscard]] const std::vector<StmtNo> &GetStmtNo(
            VarIndex var_index) const noexcept;
    [[nodiscard]] const std::set<VarIndex> &GetAllVar(
            StmtNo stmt_no) const noexcept;
    [[nodiscard]] const std::set<VarIndex> &GetVarAccessByProc(
            ProcIndex proc_index) const noexcept;
    [[nodiscard]] const std::set<StmtNo> &GetAllStmt(
            VarIndex var_index) const noexcept;
    [[nodiscard]] const std::set<ProcIndex> &GetAllProc(
            VarIndex var_index) const noexcept;
    [[nodiscard]] const std::set<ProcIndex> &GetAllProc() const noexcept;
    [[nodiscard]] PairVec<StmtNo, VarIndex> GetStmtVar(
            StmtType stmt_type) const noexcept;
    [[nodiscard]] PairVec<ProcIndex, VarIndex> GetProcVar() const noexcept;
    [[nodiscard]] std::set<StmtNo> GetStmt(StmtType stmt_type) const noexcept;
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

    [[nodiscard]] PairVec<StmtNo, VarIndex> GetAllRel() const noexcept;
    void AddDirectRel(PairVec<StmtNo, VarIndex> &stmt_var_pair,
                      const std::vector<StmtNo> &stmt_no) const noexcept;
    void AddIndirectRel(const PairVec<StmtNo, VarIndex> &basic_pairs,
                        const ContainerInfo &info,
                        ContainerAddedTrackers &bitmaps) noexcept;
    void AddAllContainerRel(
            const TypeStatementsStore &type_statement_store,
            const ContainerInfo &info,
            const CallsRelationshipStore &calls_rel_store) noexcept;
    void AddCallStmtVar(
            PairVec<StmtNo, VarIndex> &stmt_var_pair,
            const std::vector<StmtNo> &stmt_no,
            const CallsRelationshipStore &calls_rel_store) const noexcept;
    void ProcessProcedureAncestor(ProcIndex proc_index, VarIndex var_index,
                                  BitVec2D &proc_added) noexcept;
    void FillStmts() noexcept;
    void FillVars() noexcept;
    void FillRels() noexcept;
    void FillProcs() noexcept;
    void CalculateNumRels() noexcept;

    virtual void PrecompileStep(const TypeStatementsStore &type_store) noexcept;
    virtual void AddAllDirectRel(const TypeStatementsStore &store) noexcept = 0;
    virtual void AddAllIndirectRel(
            const AuxiliaryData &data_store) noexcept = 0;
    virtual void AddConditionRel(const AuxiliaryData &data_store) noexcept;

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
    PairVec<ProcIndex, VarIndex> proc_var_pair_;
    std::set<ProcIndex> all_procs_;
    std::set<VarIndex> all_vars_;
    // Read, Print, Call, While, If, Assign
    std::array<PairVec<int>, 6> stmt_var_pairs_;
    // All, Read, Print, Call, While, If, Assign
    std::array<std::set<int>, 7> stmts_arr_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_USES_MODIFIES_STORE_BASE_H_
