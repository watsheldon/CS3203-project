#ifndef SPA_SRC_SPA_SRC_PKB_STORE_STMTLST_STATEMENTS_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_STORE_STMTLST_STATEMENTS_STORE_H_

#include <utility>
#include <vector>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "pkb/knowledge_base.h"

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
    [[nodiscard]] bool ExistFollows(bool transitive,
                                    Index<ArgPos::kFirst> first_stmt,
                                    Index<ArgPos::kSecond> second_stmt) const;
    [[nodiscard]] std::vector<int> GetFollows(
            bool transitive, Index<ArgPos::kFirst> first_stmt) const;
    [[nodiscard]] std::vector<int> GetFollows(
            bool transitive, Index<ArgPos::kSecond> second_stmt) const;
    [[nodiscard]] std::vector<std::pair<int, int>> GetFollowsPairs(
            bool transitive) const;

  private:
    std::vector<std::vector<int>> stmtlst_to_statements_;
    std::vector<StmtProperties> statement_to_stmtlst_;
    [[nodiscard]] inline int GetStmtlstSize(int stmt_no) const;
    [[nodiscard]] inline bool FollowsBetweenWildcards() const;
    [[nodiscard]] inline bool FollowedByWildcard(
            Index<ArgPos::kFirst> first_stmt) const;
    [[nodiscard]] inline bool FollowsWildcard(
            Index<ArgPos::kSecond> second_stmt) const;
    [[nodiscard]] inline bool Follows(bool transitive,
                                      Index<ArgPos::kFirst> first_stmt,
                                      Index<ArgPos::kSecond> second_stmt) const;
    [[nodiscard]] std::vector<int> GetFollowsWildcard() const;
    [[nodiscard]] std::vector<int> GetFollowsFirst(
            bool transitive, Index<ArgPos::kFirst> first_stmt) const;
    [[nodiscard]] std::vector<int> GetFollowedByWildcard() const;
    [[nodiscard]] std::vector<int> GetFollowedBySecond(
            bool transitive, Index<ArgPos::kSecond> second_stmt) const;
    void AddPairs(const std::vector<int> &stmtlst,
                  std::vector<std::pair<int, int>> result) const;
    [[nodiscard]] std::vector<std::pair<int, int>> GetTransitivePairs() const;
    [[nodiscard]] std::vector<std::pair<int, int>> GetNonTransitivePairs()
            const;
};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_PKB_STORE_STMTLST_STATEMENTS_STORE_H_
