#ifndef SRC_SPA_SRC_PKB_STORE_USES_RELATIONSHIP_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_USES_RELATIONSHIP_STORE_H_

#include <cstddef>
#include <functional>
#include <set>
#include <vector>

#include "common/aliases.h"
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
    UsesRelationshipStore(std::size_t stmt_size, std::size_t var_size,
                          std::size_t proc_size);
    void Set(StmtNo stmt_no, std::vector<VarIndex>&& var_indices);
    [[nodiscard]] const std::vector<VarIndex>& GetVarIndex(
            StmtNo stmt_no) const;
    [[nodiscard]] bool ExistUses(StmtNo stmt_no, VarIndex var_index);
    [[nodiscard]] std::set<VarIndex> GetUses(StmtNo stmt_no) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetUses(VarIndex var_index, StmtType type,
                                           const TypeStatementsStore& store);
    [[nodiscard]] bool ExistUsesP(ProcIndex proc_index, VarIndex var_index);
    [[nodiscard]] bool ExistUsesP(ProcIndex proc_index);
    [[nodiscard]] const std::set<StmtNo>& GetContainers(
            StmtType type, VarIndex var_index) const;
    [[nodiscard]] std::set<StmtNo> GetContainers(StmtType type) const;
    [[nodiscard]] const PairVec<StmtNo, VarIndex>& GetContainerVarPairs(
            StmtType type) const;

  private:
    void PrecompileStep(
            const TypeStatementsStore& type_store) noexcept override;
    void AddConditionRel(const AuxiliaryData& data_store) noexcept override;
    void AddAllDirectRel(const TypeStatementsStore& store) noexcept override;
    void AddAllIndirectRel(const AuxiliaryData& data_store) noexcept override;
    void AddConditionDirectUses(StmtNo stmt_no,
                                PairVec<StmtNo, VarIndex>& stmt_var_pairs,
                                BitVec2D& bitmap) const;
    void AddConditionIndirectUses(StmtNo stmt_no, const ContainerInfo& info,
                                  ContainerAddedTrackers& bitmaps);
    static constexpr int GetCondTypeIndex(StmtType stmt_type) {
        return static_cast<int>(stmt_type) - static_cast<int>(StmtType::kWhile);
    }

    static constexpr std::array<StmtType, 2> relevant_stmt_types_{
            {StmtType::kAssign, StmtType::kPrint}};
    // while_var, if_var
    std::array<IndexBimap<std::set<int>>, 2> condition_direct_uses_;
    std::array<PairVec<int>, 2> condition_direct_pairs_;
    std::array<std::set<StmtNo>, 2> condition_direct_stmts_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_USES_RELATIONSHIP_STORE_H_
