#ifndef SPA_SRC_SPA_SRC_PKB_TYPE_STATEMENTS_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_TYPE_STATEMENTS_STORE_H_

#include <vector>

#include "common/entity_type_enum.h"

namespace spa {
class TypeStatementsStore {
  public:
    TypeStatementsStore(size_t size, std::vector<int>&& reads,
                        std::vector<int>&& prints, std::vector<int>&& calls,
                        std::vector<int>&& whiles, std::vector<int>&& ifs,
                        std::vector<int>&& assigns);
    std::vector<int> GetStatements(StmtType et);
    StmtType GetType(int stmt_no);

  private:
    std::vector<std::vector<int>> type_to_statements_;
    std::vector<StmtType> statement_to_type_;
};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_PKB_TYPE_STATEMENTS_STORE_H_
