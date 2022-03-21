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
    StmtlstStatementsStore(size_t stmtlst, size_t stmt);
    void Set(int stmtlst_index, std::vector<int> &&stmtlst);
    [[nodiscard]] int GetStmtlst(int stmt_no) const;
    [[nodiscard]] int GetStmtRelativePos(int stmt_no) const;
    [[nodiscard]] StmtProperties GetStmtProperties(int stmt_no) const;
    [[nodiscard]] std::vector<int> GetStatements(int stmtlst_index) const;
    [[nodiscard]] bool ExistFollows(Index<ArgPos::kFirst> first_stmt,
                                    Index<ArgPos::kSecond> second_stmt) const;
    [[nodiscard]] bool ExistFollowsT(Index<ArgPos::kFirst> first_stmt,
                                     Index<ArgPos::kSecond> second_stmt) const;
    [[nodiscard]] bool ExistFollows(Index<ArgPos::kFirst> first_stmt) const;
    [[nodiscard]] bool ExistFollows(Index<ArgPos::kSecond> second_stmt) const;
    [[nodiscard]] bool ExistFollows() const;
    [[nodiscard]] std::vector<int> GetFollows(
            Index<ArgPos::kFirst> first_stmt) const;
    [[nodiscard]] std::vector<int> GetFollowsT(
            Index<ArgPos::kFirst> first_stmt) const;
    [[nodiscard]] std::vector<int> GetFollows(
            Index<ArgPos::kSecond> second_stmt) const;
    [[nodiscard]] std::vector<int> GetFollowsT(
            Index<ArgPos::kSecond> second_stmt) const;
    [[nodiscard]] PairVec<int> GetFollowsPairs() const;
    [[nodiscard]] PairVec<int> GetFollowsPairsT() const;
    [[nodiscard]] std::vector<int> GetFollowsWildcard() const;
    [[nodiscard]] std::vector<int> GetFollowedByWildcard() const;

  private:
    std::vector<std::vector<int>> stmtlst_to_statements_;
    std::vector<StmtProperties> statement_to_stmtlst_;
    [[nodiscard]] inline size_t GetStmtlstSize(int stmt_no) const;
    void AddPairs(const std::vector<int> &stmtlst, PairVec<int> &results) const;
    [[nodiscard]] PairVec<int> GetTransitivePairs() const;
    [[nodiscard]] PairVec<int> GetNonTransitivePairs() const;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_PKB_STORE_STMTLST_STATEMENTS_STORE_H_
