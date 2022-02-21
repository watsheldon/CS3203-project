#ifndef SRC_SPA_SRC_PKB_STORE_POLISH_NOTATION_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_POLISH_NOTATION_STORE_H_

#include <vector>

#include "pkb/knowledge_base.h"

namespace spa {
class PolishNotationStore {
  public:
    explicit PolishNotationStore(size_t stmt_count, std::vector<PN> &&pn);
    void Set(int stmt_no, int polish_index);
    [[nodiscard]] const PN &GetNotation(int polish_index) const;
    [[nodiscard]] int GetPolishStmt(int polish_index) const;
    [[nodiscard]] int GetPolishIndex(int stmt_no) const;

  private:
    std::vector<int> stmt_to_index_;
    std::vector<int> index_to_stmt_;
    std::vector<PN> index_to_pn_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_POLISH_NOTATION_STORE_H_
