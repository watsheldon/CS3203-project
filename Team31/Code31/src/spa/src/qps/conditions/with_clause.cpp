#include "with_clause.h"

#include <algorithm>
#include <set>
#include <string_view>

#include "common/type_convert_helpers.h"
#include "qps/synonym_with_attr.h"

namespace spa {
WithClause::WithClause(std::string_view first_ident,
                       std::string_view second_ident) noexcept
        : first_param_(first_ident),
          second_param_(second_ident),
          type_(kRawRaw) {}
WithClause::WithClause(std::string_view string_view,
                       SynonymWithAttr syn) noexcept
        : first_param_(string_view), second_param_(syn), type_(kRawSyn) {}
WithClause::WithClause(SynonymWithAttr syn,
                       std::string_view string_view) noexcept
        : first_param_(string_view), second_param_(syn), type_(kRawSyn) {}
WithClause::WithClause(SynonymWithAttr first_syn,
                       SynonymWithAttr second_syn) noexcept
        : first_param_(first_syn), second_param_(second_syn), type_(kSynSyn) {}
ResultTable WithClause::Execute(KnowledgeBase *pkb) const noexcept {
    switch (type_) {
        case kRawRaw: {
            return RawRaw();
        }
        case kRawSyn: {
            const auto &second = std::get<SynonymWithAttr>(second_param_);
            bool isName = second.attribute <= Attribute::kValue;
            std::string_view first = std::get<std::string_view>(first_param_);
            return isName ? NameSyn(pkb, first, second)
                          : StmtSyn(pkb, first, second);
        }
        case kSynSyn: {
            return SynSyn(pkb);
        }
    }
    assert(false);
    return ResultTable(false);
}
ResultTable WithClause::RawRaw() const noexcept {
    return ResultTable(first_param_ == second_param_);
}
ResultTable WithClause::StmtSyn(KnowledgeBase *pkb, std::string_view value,
                                SynonymWithAttr second) noexcept {
    auto domain = pkb->GetAllEntityIndices(SynToPkbType(second.synonym));
    auto index = std::stoi(value.data());
    auto result = std::binary_search(domain.begin(), domain.end(), index);
    return result ? ResultTable(second.synonym, std::set<int>{index})
                  : ResultTable{false};
}
ResultTable WithClause::NameSyn(KnowledgeBase *pkb, std::string_view name,
                                SynonymWithAttr synonym_with_attr) noexcept {
    assert(synonym_with_attr.attribute != Attribute::kStmtNum);
    auto &[syn, attr] = synonym_with_attr;
    switch (syn->type) {
        case Synonym::kStmtCall: {
            auto result = pkb->GetCallStmts(name);
            return {syn, std::move(result)};
        }
        case Synonym::kStmtRead: {
            auto result = pkb->GetModifies(Name<ArgPos::kSecond>(name),
                                           SynToPkbType(syn));
            return {syn, std::move(result)};
        }
        case Synonym::kStmtPrint: {
            auto result = pkb->GetUses(Name<ArgPos::kSecond>(name),
                                       SynToPkbType(syn));
            return {syn, std::move(result)};
        }
        case Synonym::kProc:
        case Synonym::kVar:
        case Synonym::kConst: {
            int index = pkb->IdentToIndexValue(name, AttrToPkbType(attr));
            return index == 0 ? ResultTable(false)
                              : ResultTable(syn, std::set<int>{index});
        }
        default:
            assert(false);
    }
    return ResultTable{false};
}
ResultTable WithClause::SynSyn(KnowledgeBase *pkb) const noexcept {
    auto first_attr = std::get<SynonymWithAttr>(first_param_).attribute;
    return first_attr == Attribute::kValue || first_attr == Attribute::kStmtNum
                   ? SynSynNum(pkb)
                   : SynSynName(pkb);
}
ResultTable WithClause::SynSynName(KnowledgeBase *pkb) const noexcept {
    const auto &first = std::get<SynonymWithAttr>(first_param_);
    const auto &second = std::get<SynonymWithAttr>(second_param_);
    std::vector<int> first_col;
    std::vector<int> second_col;
    auto [first_indices, first_names] = ToNames(pkb, first);
    auto [second_indices, second_names] = ToNames(pkb, second);
    for (auto it_1 = first_names.begin(); it_1 != first_names.end(); ++it_1) {
        for (auto it_2 = second_names.begin(); it_2 != second_names.end();
             ++it_2) {
            if (*it_1 == *it_2) {
                auto pos_1 = it_1 - first_names.begin();
                auto pos_2 = it_2 - second_names.begin();
                first_col.emplace_back(first_indices[pos_1]);
                second_col.emplace_back(second_indices[pos_2]);
            }
        }
    }
    return {first.synonym, std::move(first_col), second.synonym,
            std::move(second_col)};
}
PairVec<int, std::string_view> WithClause::ToNames(
        KnowledgeBase *pkb, SynonymWithAttr syn) noexcept {
    auto type = syn.synonym->type;
    if (type == Synonym::Type::kStmtPrint || type == Synonym::Type::kStmtRead ||
        type == Synonym::Type::kStmtCall) {
        auto stmt_type = SynToPkbType(syn.synonym);
        auto stmts = pkb->GetAllEntityIndices(stmt_type);
        auto names = pkb->GetAttr(stmts, stmt_type);
        return {stmts, names};
    }
    auto query_entity_type = AttrToPkbType(syn.attribute);
    auto entities = pkb->GetAllEntityIndices(query_entity_type);
    auto names = pkb->GetNames(entities, query_entity_type);
    return {entities, names};
}
ResultTable WithClause::SynSynNum(KnowledgeBase *pkb) const noexcept {
    auto first = std::get<SynonymWithAttr>(first_param_);
    auto second = std::get<SynonymWithAttr>(second_param_);
    // constant.value = constant2.value
    if (first.attribute == Attribute::kValue &&
        second.attribute == Attribute::kValue) {
        return ValueValue(pkb, first, second);
    }
    // constant.value = stmt.stmt#
    if (first.attribute == Attribute::kValue) {
        return ValueStmt(pkb, first, second);
    }
    // print.stmt# = constant.value
    if (second.attribute == Attribute::kValue) {
        return ValueStmt(pkb, second, first);
    }
    return StmtStmt(pkb, first, second);
}
ResultTable WithClause::ValueValue(KnowledgeBase *pkb, SynonymWithAttr value_1,
                                   SynonymWithAttr value_2) noexcept {
    auto col_1 = pkb->GetAllEntityIndices(AttrToPkbType(value_1.attribute));
    auto col_2 = col_1;
    return {value_1.synonym, std::move(col_1), value_2.synonym,
            std::move(col_2)};
}
ResultTable WithClause::ValueStmt(KnowledgeBase *pkb, SynonymWithAttr value,
                                  SynonymWithAttr stmt) noexcept {
    assert(value.attribute == Attribute::kValue &&
           stmt.attribute == Attribute::kStmtNum);
    // constant.value = stmt.stmt#
    QueryEntityType et = AttrToPkbType(value.attribute);
    auto constants = pkb->GetAllEntityIndices(et);
    auto stmts = pkb->GetAllEntityIndices(SynToPkbType(stmt.synonym));
    auto names = pkb->GetNames(constants, et);
    std::vector<int> col_1;
    std::vector<int> col_2;
    for (auto name = names.begin(); name != names.end(); ++name) {
        int val = std::stoi(name->data());
        if (std::binary_search(stmts.begin(), stmts.end(), val)) {
            col_1.emplace_back(constants[name - names.begin()]);
            col_2.emplace_back(val);
        }
    }
    return {value.synonym, std::move(col_1), stmt.synonym, std::move(col_2)};
}
ResultTable WithClause::StmtStmt(KnowledgeBase *pkb, SynonymWithAttr stmt_1,
                                 SynonymWithAttr stmt_2) noexcept {
    auto type_1 = stmt_1.synonym->type;
    auto type_2 = stmt_2.synonym->type;
    if (type_1 == type_2) {
        auto col_1 = pkb->GetAllEntityIndices(SynToPkbType(stmt_1.synonym));
        auto col_2 = col_1;
        return ResultTable{stmt_1.synonym, std::move(col_1), stmt_2.synonym,
                           std::move(col_2)};
    }
    // read.stmt# = print.stmt#
    if (type_1 != Synonym::Type::kStmtAny &&
        type_2 != Synonym::Type::kStmtAny) {
        return ResultTable(false);
    }
    // stmt.stmt# == read.stmt#
    if (type_1 == Synonym::Type::kStmtAny) {
        auto col_1 = pkb->GetAllEntityIndices(SynToPkbType(stmt_2.synonym));
        auto col_2 = col_1;
        return {stmt_1.synonym, std::move(col_1), stmt_2.synonym,
                std::move(col_2)};
    }
    // print.stmt# = stmt.stmt#
    if (type_2 == Synonym::Type::kStmtAny) {
        auto col_1 = pkb->GetAllEntityIndices(SynToPkbType(stmt_1.synonym));
        auto col_2 = col_1;
        return {stmt_1.synonym, std::move(col_1), stmt_2.synonym,
                std::move(col_2)};
    }
    assert(false);
    return ResultTable{false};
}
int WithClause::GetPriority() const noexcept {
    return kPriority[GetSynCount()];
}
int WithClause::GetSynCount() const noexcept {
    return kSynCount[static_cast<int>(type_)];
}
}  // namespace spa
