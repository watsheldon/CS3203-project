#ifndef SRC_SPA_SRC_PKB_STORE_POLISH_NOTATION_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_POLISH_NOTATION_STORE_H_

#include <array>
#include <set>
#include <variant>
#include <vector>

#include "common/polish_notation.h"
#include "common/polish_notation_node.h"
#include "name_value_store.h"
#include "qps/query_token.h"

namespace spa {
using PN = spa::PolishNotation;

class PolishNotationStore {
  public:
    PolishNotationStore(size_t stmt_count, std::vector<PN> pns,
                        const NameValueStore& name_value);
    void Set(int stmt_no, int polish_index);
    [[nodiscard]] const PN& GetNotation(int polish_index) const;
    [[nodiscard]] int GetPolishStmt(int polish_index) const;
    [[nodiscard]] int GetPolishIndex(int stmt_no) const;
    [[nodiscard]] std::set<int> CheckPattern(
            const PN& converted_pn, bool partial_match,
            const std::vector<int>& assign_stmt);
    [[nodiscard]] std::set<int> CheckPattern(
            const PN& converted_pn, const std::vector<int>& assign_stmt);
    [[nodiscard]] std::set<int> CheckPatternP(
            const PN& converted_pn, const std::vector<int>& assign_stmt);
    [[nodiscard]] std::unique_ptr<PolishNotation> ConvertFromQueryTokens(
            const std::vector<QueryToken>& tokens);

  private:
    [[nodiscard]] bool AddSingleToken(const QueryToken& token);
    [[nodiscard]] bool AddConstant(const QueryToken& token);
    using PNNodeArgType = std::variant<OperatorType, ExprNodeType>;
    static constexpr std::array<PNNodeArgType, 7> kOperatorMap{
            OperatorType::kPlus,    OperatorType::kMinus,
            OperatorType::kTimes,   OperatorType::kDivide,
            OperatorType::kModulo,  ExprNodeType::kBracketL,
            ExprNodeType::kBracketR};
    static PolishNotationNode TokenTypeToPNNode(
            QueryTokenType token_type) noexcept;

    std::vector<int> stmt_to_index_;
    std::vector<int> index_to_stmt_;
    std::vector<PN> index_to_pn_;
    const NameValueStore& name_value_;
    std::vector<PolishNotationNode> expr_;
    bool AddVariable(const QueryToken& token);
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_POLISH_NOTATION_STORE_H_
