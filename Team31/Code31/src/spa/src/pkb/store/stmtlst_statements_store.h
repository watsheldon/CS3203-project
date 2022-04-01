#ifndef SRC_SPA_SRC_PKB_STORE_STMTLST_STATEMENTS_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_STMTLST_STATEMENTS_STORE_H_

#include <utility>
#include <vector>

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "common/index.h"

namespace spa {
struct StmtProperties {
    int stmtlst_index = 0;
    int pos_in_stmtlst = 0;
};
class StmtlstStatementsStore {
  public:
    StmtlstStatementsStore(std::size_t stmtlst_count,
                           std::size_t stmt_count) noexcept;
    void Set(int stmtlst_index, std::vector<int> stmtlst);
    [[nodiscard]] int GetStmtlst(StmtNo stmt_no) const noexcept;
    [[nodiscard]] int GetStmtRelativePos(StmtNo stmt_no) const noexcept;
    [[nodiscard]] StmtProperties GetStmtProperties(
            StmtNo stmt_no) const noexcept;
    [[nodiscard]] const std::vector<int> &GetStatements(
            int stmtlst_index) const noexcept;
    [[nodiscard]] bool ExistFollows(StmtNo first, StmtNo second) const noexcept;
    [[nodiscard]] bool ExistFollowsT(StmtNo first,
                                     StmtNo second) const noexcept;
    [[nodiscard]] bool ExistFollowsSecondArg(StmtNo first_stmt) const noexcept;
    [[nodiscard]] bool ExistFollowsFirstArg(StmtNo second_stmt) const noexcept;
    [[nodiscard]] bool ExistFollows() const noexcept;
    [[nodiscard]] StmtNo GetFollowsSecondArg(StmtNo first_stmt) const noexcept;
    [[nodiscard]] std::vector<StmtNo> GetFollowsTSecondArg(
            StmtNo first_stmt) const noexcept;
    [[nodiscard]] StmtNo GetFollowsFirstArg(StmtNo second_stmt) const noexcept;
    [[nodiscard]] std::vector<StmtNo> GetFollowsTFirstArg(
            StmtNo second_stmt) const noexcept;
    [[nodiscard]] PairVec<StmtNo> GetFollowsPairs() const noexcept;
    [[nodiscard]] PairVec<StmtNo> GetFollowsPairsT() const noexcept;
    [[nodiscard]] std::vector<StmtNo> GetFollowsWildcard() const noexcept;
    [[nodiscard]] std::vector<StmtNo> GetFollowedByWildcard() const noexcept;

  private:
    std::vector<std::vector<StmtNo>> stmtlst_to_statements_;
    std::vector<StmtProperties> statement_to_stmtlst_;
    [[nodiscard]] inline std::size_t GetStmtlstSize(
            StmtNo stmt_no) const noexcept;
    [[nodiscard]] std::size_t GetFollowColumnSize() const noexcept;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_PKB_STORE_STMTLST_STATEMENTS_STORE_H_
