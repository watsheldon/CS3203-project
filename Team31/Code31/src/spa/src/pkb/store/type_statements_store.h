#ifndef SRC_SPA_SRC_PKB_STORE_TYPE_STATEMENTS_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_TYPE_STATEMENTS_STORE_H_

#include <common/entity_type_enum.h>

#include <vector>

namespace spa {
class TypeStatementsStore {
  public:
    TypeStatementsStore(size_t size, std::vector<int>&& reads,
                        std::vector<int>&& prints, std::vector<int>&& calls,
                        std::vector<int>&& whiles, std::vector<int>&& ifs,
                        std::vector<int>&& assigns);
    [[nodiscard]] const std::vector<int>& GetStatements(StmtType st) const;
    [[nodiscard]] StmtType GetType(int stmt_no) const;

  private:
    std::vector<std::vector<int>> type_to_statements_;
    std::vector<StmtType> statement_to_type_;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_PKB_STORE_TYPE_STATEMENTS_STORE_H_
