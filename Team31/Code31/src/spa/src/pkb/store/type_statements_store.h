#ifndef SRC_SPA_SRC_PKB_STORE_TYPE_STATEMENTS_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_TYPE_STATEMENTS_STORE_H_

#include <common/aliases.h>
#include <common/entity_type_enum.h>

#include <vector>

namespace spa {
class TypeStatementsStore {
  public:
    TypeStatementsStore(std::vector<StmtNo>&& reads,
                        std::vector<StmtNo>&& prints,
                        std::vector<StmtNo>&& calls,
                        std::vector<StmtNo>&& whiles, std::vector<StmtNo>&& ifs,
                        std::vector<StmtNo>&& assigns);
    [[nodiscard]] const std::vector<StmtNo>& GetStatements(StmtType st) const;
    [[nodiscard]] StmtType GetType(StmtNo stmt_no) const;

  private:
    std::vector<StmtType> statement_to_type_;
    std::vector<std::vector<StmtNo>> type_to_statements_;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_PKB_STORE_TYPE_STATEMENTS_STORE_H_
