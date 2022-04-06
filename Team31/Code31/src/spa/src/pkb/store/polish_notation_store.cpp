#include "polish_notation_store.h"

#include <cassert>
#include <utility>

namespace spa {
PolishNotationStore::PolishNotationStore(size_t stmt_count, std::vector<PN> pns,
                                         const NameValueStore& name_value)
        : stmt_to_index_(stmt_count + 1),
          index_to_stmt_(pns.size()),
          index_to_pn_(std::move(pns)),
          name_value_(name_value) {}
void PolishNotationStore::Set(StmtNo stmt_no, PNIndex polish_index) {
    stmt_to_index_[stmt_no] = polish_index;
    index_to_stmt_[polish_index] = stmt_no;
}
const PN& PolishNotationStore::GetNotation(PNIndex polish_index) const {
    return index_to_pn_[polish_index];
}
StmtNo PolishNotationStore::GetPolishStmt(PNIndex polish_index) const {
    return index_to_stmt_[polish_index];
}
PNIndex PolishNotationStore::GetPolishIndex(StmtNo stmt_no) const {
    return stmt_to_index_[stmt_no];
}
std::set<StmtNo> PolishNotationStore::CheckPattern(
        const PN& converted_pn, const std::vector<StmtNo>& assign_stmt) {
    std::set<StmtNo> full_match_stmt;
    for (const auto i : assign_stmt) {
        const PN& pn = index_to_pn_[stmt_to_index_[i]];
        if (pn == converted_pn) {
            full_match_stmt.emplace(i);
        }
    }
    return full_match_stmt;
}
std::set<StmtNo> PolishNotationStore::CheckPatternP(
        const PN& converted_pn, const std::vector<StmtNo>& assign_stmt) {
    std::set<StmtNo> partial_match_stmt;
    for (const auto i : assign_stmt) {
        const PN& pn = index_to_pn_[stmt_to_index_[i]];
        if (pn.Contains(converted_pn)) {
            partial_match_stmt.emplace(i);
        }
    }
    return partial_match_stmt;
}
std::unique_ptr<PolishNotation> PolishNotationStore::ConvertFromQueryTokens(
        const std::vector<QueryToken>& tokens) {
    expr_.clear();
    for (const auto& token : tokens) {
        if (!AddSingleToken(token)) {
            return {};
        }
    }
    return std::make_unique<PolishNotation>(std::move(expr_));
}
bool PolishNotationStore::AddVariable(const QueryToken& token) {
    int index = name_value_.GetIndex(token.value, QueryEntityType::kVar);
    if (index == 0) return false;
    expr_.emplace_back(PolishNotationNode(ExprNodeType::kVariable, index));
    return true;
}
bool PolishNotationStore::AddConstant(const QueryToken& token) {
    int index = name_value_.GetIndex(token.value, QueryEntityType::kConst);
    if (index == 0) return false;
    expr_.emplace_back(PolishNotationNode(ExprNodeType::kConstant, index));
    return true;
}
bool PolishNotationStore::AddSingleToken(const QueryToken& token) {
    bool varconst_present = true;
    switch (token.type) {
        case QueryTokenType::kWord:
            varconst_present = AddVariable(token);
            break;
        case QueryTokenType::kInteger:
            varconst_present = AddConstant(token);
            break;
        case QueryTokenType::kOperatorPlus:
        case QueryTokenType::kOperatorMinus:
        case QueryTokenType::kOperatorTimes:
        case QueryTokenType::kOperatorDivide:
        case QueryTokenType::kOperatorModulo:
        case QueryTokenType::kBracketL:
        case QueryTokenType::kBracketR:
            expr_.emplace_back(TokenTypeToPNNode(token.type));
            break;
        default:
            assert(false);
    }
    return varconst_present;
}
PolishNotationNode PolishNotationStore::TokenTypeToPNNode(
        QueryTokenType token_type) noexcept {
    assert(token_type >= QueryTokenType::kOperatorPlus &&
           token_type <= QueryTokenType::kBracketR);
    auto opr = static_cast<OperatorType>(
            static_cast<int>(token_type) -
            static_cast<int>(QueryTokenType::kOperatorPlus));
    return PolishNotationNode(opr);
}
}  // namespace spa
