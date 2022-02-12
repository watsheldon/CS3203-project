#ifndef SPA_SRC_SPA_SRC_PKB_TYPE_STATEMENTS_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_TYPE_STATEMENTS_STORE_H_

#include <vector>

#include "knowledge_base.h"

namespace spa {
struct StmtByType {
    std::vector<Index<kStmt>> reads;
    std::vector<Index<kStmt>> prints;
    std::vector<Index<kStmt>> calls;
    std::vector<Index<kStmt>> whiles;
    std::vector<Index<kStmt>> ifs;
    std::vector<Index<kStmt>> assigns;
};
class TypeStatementsStore {
  public:
    TypeStatementsStore(size_t size, std::vector<Index<kStmt>>&& reads,
                        std::vector<Index<kStmt>>&& prints,
                        std::vector<Index<kStmt>>&& calls,
                        std::vector<Index<kStmt>>&& whiles,
                        std::vector<Index<kStmt>>&& ifs,
                        std::vector<Index<kStmt>>&& assigns);
    std::vector<Index<kStmt>> GetStatements(PKBEntityType et);
    PKBEntityType GetType(Index<kStmt> stmt_no);

  private:
    StmtByType type_to_statements;
    std::vector<PKBEntityType> statement_to_type;
};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_PKB_TYPE_STATEMENTS_STORE_H_
