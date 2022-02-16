#ifndef SPA_SRC_SPA_SRC_PKB_STORE_STMTLST_STATEMENTS_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_STORE_STMTLST_STATEMENTS_STORE_H_

#include <vector>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "pkb/knowledge_base.h"

namespace spa {
struct StmtlstIndexPosSize {
    int stmtlst_index = 0;
    int pos_in_stmtlst = 0;
    int stmtlst_size = 0;
};
class StmtlstStatementsStore {
  public:
    StmtlstStatementsStore(size_t stmtlst, size_t stmt);
    void Set(int stmtlst_index, std::vector<int> &&stmtlst);
    [[nodiscard]] int GetStmtlstIndex(int stmt_no) const;
    [[nodiscard]] int GetStmtlstPos(int stmt_no) const;
    [[nodiscard]] int GetStmtlstSize(int stmt_no) const;
    [[nodiscard]] StmtlstIndexPosSize GetIndexPosSize(int stmt_no) const;
    [[nodiscard]] std::vector<int> GetStatements(int stmtlst_index) const;
    [[nodiscard]] bool Follows() const;
    [[nodiscard]] bool Follows(Index<ArgPos::kFirst> first_stmt) const;
    [[nodiscard]] bool Follows(Index<ArgPos::kSecond> second_stmt) const;
    [[nodiscard]] bool Follows(bool transitive,
                               Index<ArgPos::kFirst> first_stmt,
                               Index<ArgPos::kSecond> second_stmt) const;
    [[nodiscard]] std::vector<int> GetFollows(
            Index<ArgPos::kFirst> first_stmt) const;
    [[nodiscard]] std::vector<int> GetFollows(
            bool transitive, Index<ArgPos::kFirst> first_stmt) const;
    [[nodiscard]] std::vector<int> GetFollows(
            Index<ArgPos::kSecond> second_stmt) const;
    [[nodiscard]] std::vector<int> GetFollows(
            bool transitive, Index<ArgPos::kSecond> second_stmt) const;

  private:
    std::vector<std::vector<int>> stmtlst_to_statements_;
    std::vector<StmtlstIndexPosSize> statement_to_stmtlst_;
};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_PKB_STORE_STMTLST_STATEMENTS_STORE_H_
