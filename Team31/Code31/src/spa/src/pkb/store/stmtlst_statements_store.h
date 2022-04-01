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
    StmtlstStatementsStore(std::size_t stmtlst_count, std::size_t stmt_count);
    void Set(int stmtlst_index, std::vector<int> stmtlst);
    [[nodiscard]] int GetStmtlst(int stmt_no) const;
    [[nodiscard]] int GetStmtRelativePos(int stmt_no) const;
    [[nodiscard]] StmtProperties GetStmtProperties(int stmt_no) const;
    [[nodiscard]] const std::vector<int> &GetStatements(
            int stmtlst_index) const;
    [[nodiscard]] bool ExistFollows(Index<ArgPos::kFirst> first,
                                    Index<ArgPos::kSecond> second) const;
    [[nodiscard]] bool ExistFollowsT(Index<ArgPos::kFirst> first,
                                     Index<ArgPos::kSecond> second) const;
    [[nodiscard]] bool ExistFollows(Index<ArgPos::kFirst> first_stmt) const;
    [[nodiscard]] bool ExistFollows(Index<ArgPos::kSecond> second_stmt) const;
    [[nodiscard]] bool ExistFollows() const;
    [[nodiscard]] StmtNo GetFollowsSecondArg(
            Index<ArgPos::kFirst> first_stmt) const;
    [[nodiscard]] std::vector<int> GetFollowsTSecondArg(
            Index<ArgPos::kFirst> first_stmt) const;
    [[nodiscard]] StmtNo GetFollowsFirstArg(
            Index<ArgPos::kSecond> second_stmt) const;
    [[nodiscard]] std::vector<int> GetFollowsTFirstArg(
            Index<ArgPos::kSecond> second_stmt) const;
    [[nodiscard]] PairVec<int> GetFollowsPairs() const;
    [[nodiscard]] PairVec<int> GetFollowsPairsT() const;
    [[nodiscard]] std::vector<int> GetFollowsWildcard() const;
    [[nodiscard]] std::vector<int> GetFollowedByWildcard() const;

  private:
    std::vector<std::vector<int>> stmtlst_to_statements_;
    std::vector<StmtProperties> statement_to_stmtlst_;
    [[nodiscard]] inline std::size_t GetStmtlstSize(int stmt_no) const;
    [[nodiscard]] std::size_t GetFollowColumnSize() const;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_PKB_STORE_STMTLST_STATEMENTS_STORE_H_
