#ifndef SRC_SPA_SRC_PKB_STORE_USES_RELATIONSHIP_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_USES_RELATIONSHIP_STORE_H_

#include <cstddef>
#include <functional>
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
    UsesRelationshipStore(std::size_t stmt_size, std::size_t var_size,
                          std::size_t proc_size);
    void Set(int stmt_no, std::vector<int>&& var_indices);
    [[nodiscard]] const std::vector<int>& GetVarIndex(int stmt_no) const;
    [[nodiscard]] bool ExistUses(int stmt_no, int var_index);
    [[nodiscard]] std::set<int> GetUses(int stmt_no);
    [[nodiscard]] std::set<int> GetUses(int var_index, StmtType type,
                                        const TypeStatementsStore& store);
    [[nodiscard]] bool ExistUsesP(int proc_index, int var_index);
    [[nodiscard]] bool ExistUsesP(int proc_index);
    [[nodiscard]] const std::set<int> &GetContainers(StmtType type, int var_index) const;
    [[nodiscard]] std::set<int> GetContainers(StmtType type) const;
    [[nodiscard]] const PairVec<int> &GetContainerVarPairs(StmtType type) const;

  private:
    void AddConditionRel(const AuxiliaryData& data_store) override;
    void AddAllDirectRel(const TypeStatementsStore& store) override;
    void AddAllIndirectRel(const AuxiliaryData& data_store) override;
    void AddConditionDirectUses(StmtType type, int stmt_no,
                                PairVec<int>& stmt_var_pairs, BitVec2D& bitmap);
    void AddConditionIndirectUses(int stmt_no, const ContainerInfo& info,
                                  ContainerAddedTrackers& bitmaps);

    static constexpr std::array<StmtType, 2> relevant_stmt_types_{
            {StmtType::kAssign, StmtType::kPrint}};
    // while_var, if_var
    std::array<IndexBimap<std::set<int>>, 2> condition_direct_uses_;
    std::array<PairVec<int>, 2> condition_direct_pairs_;
    static constexpr int GetIndex(StmtType container_type) {
        return static_cast<int>(container_type) -
               static_cast<int>(StmtType::kWhile);
    }
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_USES_RELATIONSHIP_STORE_H_
