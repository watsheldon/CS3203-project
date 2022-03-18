#ifndef SRC_SPA_SRC_PKB_STORE_POLISH_NOTATION_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_POLISH_NOTATION_STORE_H_

#include <set>
#include <vector>

#include "common/polish_notation.h"
#include "name_value_store.h"
#include "qps/query_token.h"

namespace spa {
using PN = spa::PolishNotation;

class PolishNotationStore {
  public:
    explicit PolishNotationStore(size_t stmt_count, std::vector<PN> pns,
                                 const NameValueStore &name_value);
    void Set(int stmt_no, int polish_index);
    [[nodiscard]] const PN &GetNotation(int polish_index) const;
    [[nodiscard]] int GetPolishStmt(int polish_index) const;
    [[nodiscard]] int GetPolishIndex(int stmt_no) const;
    [[nodiscard]] std::set<int> CheckPattern(PN converted_pn,
                                             bool partial_match,
                                             std::vector<int> assign_stmt);
    [[nodiscard]] std::unique_ptr<PolishNotation> ConvertFromQueryTokens(
            const std::vector<QueryToken> &tokens);
    [[nodiscard]] PolishNotationNode TokenToNotation(QueryTokenType token_type);
    [[nodiscard]] bool AddConstant(QueryToken token,
                                   std::vector<PolishNotationNode> &expr);
    [[nodiscard]] bool AddSingleToken(QueryToken token,
                                      std::vector<PolishNotationNode> &expr);

  private:
    std::vector<int> stmt_to_index_;
    std::vector<int> index_to_stmt_;
    std::vector<PN> index_to_pn_;
    const NameValueStore &name_value_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_POLISH_NOTATION_STORE_H_
