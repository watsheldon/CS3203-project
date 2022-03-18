#include "polish_notation_store.h"

#include <cassert>

namespace spa {
PolishNotationStore::PolishNotationStore(size_t stmt_count, std::vector<PN> pns,
                                         const NameValueStore &name_value)
        : stmt_to_index_(stmt_count + 1),
          index_to_stmt_(pns.size()),
          index_to_pn_(std::move(pns)),
          name_value_(name_value) {}

void PolishNotationStore::Set(int stmt_no, int polish_index) {
    stmt_to_index_[stmt_no] = polish_index;
    index_to_stmt_[polish_index] = stmt_no;
}

const PN &PolishNotationStore::GetNotation(int polish_index) const {
    return index_to_pn_[polish_index];
}

int PolishNotationStore::GetPolishStmt(int polish_index) const {
    return index_to_stmt_[polish_index];
}

int PolishNotationStore::GetPolishIndex(int stmt_no) const {
    return stmt_to_index_[stmt_no];
}

std::set<int> PolishNotationStore::CheckPattern(PN converted_pn,
                                                bool partial_match,
                                                std::vector<int> assign_stmt) {
    if (partial_match) {
        std::set<int> partial_match_stmt;

        for (int i : assign_stmt) {
            const PN &pn = index_to_pn_[stmt_to_index_[i]];
            if (pn.Contains(converted_pn)) {
                partial_match_stmt.emplace(i);
            }
        }
        return partial_match_stmt;
    }

    assert(partial_match == false);
    std::set<int> full_match_stmt;

    for (const auto i : assign_stmt) {
        const PN &pn = index_to_pn_[stmt_to_index_[i]];
        if (pn == converted_pn) {
            full_match_stmt.emplace(i);
        }
    }

    return full_match_stmt;
}

std::unique_ptr<PolishNotation> PolishNotationStore::ConvertFromQueryTokens(
        const std::vector<QueryToken> &tokens) {
    std::vector<PolishNotationNode> expr;
    for (const auto &token : tokens) {
        if (AddSingleToken(token, expr)) {
            return {};
        }
    }
    return std::make_unique<PolishNotation>(expr);
}

PolishNotationNode PolishNotationStore::TokenToNotation(
        QueryTokenType token_type) {
    switch (token_type) {
        case QueryTokenType::kOperatorPlus:
            return PolishNotationNode(OperatorType::kPlus);
        case QueryTokenType::kOperatorMinus:
            return PolishNotationNode(OperatorType::kMinus);
        case QueryTokenType::kOperatorTimes:
            return PolishNotationNode(OperatorType::kTimes);
        case QueryTokenType::kOperatorDivide:
            return PolishNotationNode(OperatorType::kDivide);
        case QueryTokenType::kOperatorModulo:
            return PolishNotationNode(OperatorType::kModulo);
        case QueryTokenType::kBracketL:
            return PolishNotationNode(ExprNodeType::kBracketL);
        case QueryTokenType::kBracketR:
            return PolishNotationNode(ExprNodeType::kBracketR);
        default:
            assert(false);
    }
}

bool PolishNotationStore::AddConstant(QueryToken token,
                                      std::vector<PolishNotationNode> &expr) {
    if (token.type == QueryTokenType::kWord) {
        int var_index =
                name_value_.GetIndex(token.value, QueryEntityType::kVar);
        if (var_index == 0) {
            return true;
        }
        expr.emplace_back(
                PolishNotationNode(ExprNodeType::kVariable, var_index));
        return false;
    }
    assert(token.type == QueryTokenType::kInteger);
    int const_index =
            name_value_.GetIndex(token.value, QueryEntityType::kConst);
    if (const_index == 0) {
        return true;
    }
    expr.emplace_back(PolishNotationNode(ExprNodeType::kConstant, const_index));
    return false;
}

bool PolishNotationStore::AddSingleToken(
        QueryToken token, std::vector<PolishNotationNode> &expr) {
    bool contains_unseen = false;
    switch (token.type) {
        case QueryTokenType::kWord:
        case QueryTokenType::kInteger:
            contains_unseen = AddConstant(token, expr);
            break;
        case QueryTokenType::kOperatorPlus:
        case QueryTokenType::kOperatorMinus:
        case QueryTokenType::kOperatorTimes:
        case QueryTokenType::kOperatorDivide:
        case QueryTokenType::kOperatorModulo:
        case QueryTokenType::kBracketL:
        case QueryTokenType::kBracketR:
            expr.emplace_back(TokenToNotation(token.type));
            break;
        default:
            assert(false);
    }
    return contains_unseen;
}

}  // namespace spa
