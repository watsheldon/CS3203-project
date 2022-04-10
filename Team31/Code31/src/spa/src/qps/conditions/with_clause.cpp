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
            return RawRaw(pkb, first_param_, second_param_);
        }
        case kRawSyn: {
            bool isName = std::get<SynonymWithAttr>(second_param_).attribute_ <=
                          Attribute::kValue;
            return isName ? NameSyn(pkb,
                                    std::get<std::string_view>(first_param_),
                                    std::get<SynonymWithAttr>(second_param_))
                          : StmtSyn(pkb,
                                    std::get<std::string_view>(first_param_),
                                    std::get<SynonymWithAttr>(second_param_));
        }
        case kSynSyn: {
            return SynSyn(pkb);
        }
    }
    assert(false);
    return ResultTable(false);
}
ResultTable WithClause::RawRaw(KnowledgeBase *pkb, WithClause::Param first,
                               WithClause::Param second) noexcept {
    return ResultTable(first == second);
}
ResultTable WithClause::StmtSyn(KnowledgeBase *pkb, std::string_view value,
                                SynonymWithAttr second) noexcept {
    // 4 = print.stmt#
    auto domain = pkb->GetAllEntityIndices(SynToPkbType(second.synonym_));
    auto index = std::stoi(value.data());
    auto result = std::binary_search(domain.begin(), domain.end(), index);
    return result ? ResultTable(second.synonym_, std::set<int>{index})
                  : ResultTable{false};
}
ResultTable WithClause::NameSyn(KnowledgeBase *pkb, std::string_view name,
                                SynonymWithAttr synonym_with_attr) noexcept {
    assert(synonym_with_attr.attribute_ != Attribute::kStmtNum);
    switch (synonym_with_attr.synonym_->type) {
        case Synonym::kStmtCall: {
            auto result = pkb->GetCalls(Name<ArgPos::kSecond>(name));
            return {synonym_with_attr.synonym_, std::move(result)};
        }
        case Synonym::kStmtRead: {
            auto result =
                    pkb->GetModifies(Name<ArgPos::kSecond>(name),
                                     SynToPkbType(synonym_with_attr.synonym_));
            return {synonym_with_attr.synonym_, std::move(result)};
        }
        case Synonym::kStmtPrint: {
            auto result =
                    pkb->GetUses(Name<ArgPos::kSecond>(name),
                                 SynToPkbType(synonym_with_attr.synonym_));
            return {synonym_with_attr.synonym_, std::move(result)};
        }
        case Synonym::kProc:
        case Synonym::kVar:
        case Synonym::kConst: {
            int index = pkb->IdentToIndexValue(
                    name, AttrToPkbType(synonym_with_attr.attribute_));
            return index == 0 ? ResultTable(false)
                              : ResultTable(synonym_with_attr.synonym_,
                                            std::set<int>{index});
        }
        default:
            assert(false);
            return ResultTable{false};
    }
}
ResultTable WithClause::SynSyn(KnowledgeBase *pkb) const noexcept {
    auto first = std::get<SynonymWithAttr>(first_param_);
    if (first.attribute_ == Attribute::kValue ||
        first.attribute_ == Attribute::kStmtNum) {
        return SynSynNum(pkb);
    }
    return SynSynName(pkb);
}
ResultTable WithClause::SynSynName(KnowledgeBase *pkb) const noexcept {
    auto first = std::get<SynonymWithAttr>(first_param_);
    auto second = std::get<SynonymWithAttr>(second_param_);
    std::vector<int> col_1;
    std::vector<int> col_2;
    std::vector<int> indices_1;
    std::vector<int> indices_2;
    std::list<std::string> names_1;
    std::list<std::string> names_2;
    ToNames(pkb, first, indices_1, names_1);
    ToNames(pkb, second, indices_2, names_2);
    for (auto it_1 = names_1.begin(); it_1 != names_1.end(); ++it_1) {
        auto name_1 = *it_1;
        for (auto it_2 = names_2.begin(); it_2 != names_2.end(); ++it_2) {
            auto name_2 = *it_2;
            if (name_1 == name_2) {
                auto pos_1 = distance(names_1.begin(), it_1);
                auto pos_2 = distance(names_2.begin(), it_2);
                col_1.emplace_back(indices_1[pos_1]);
                col_2.emplace_back(indices_2[pos_2]);
            }
        }
    }
    return {first.synonym_, std::move(col_1), second.synonym_,
            std::move(col_2)};
}
void WithClause::ToNames(KnowledgeBase *pkb, SynonymWithAttr syn,
                         std::vector<int> &indices,
                         std::list<std::string> &names) const noexcept {
    auto type = syn.synonym_->type;
    if (type == Synonym::Type::kStmtPrint || type == Synonym::Type::kStmtRead ||
        type == Synonym::Type::kStmtCall) {
        auto stmt_type = SynToPkbType(syn.synonym_);
        indices = std::move(pkb->GetAllEntityIndices(stmt_type));
        pkb->ToAttr(stmt_type, indices, names);
    } else {
        auto query_entity_type = AttrToPkbType(syn.attribute_);
        indices = std::move(pkb->GetAllEntityIndices(query_entity_type));
        pkb->ToName(query_entity_type, indices, names);
    }
}
ResultTable WithClause::SynSynNum(KnowledgeBase *pkb) const noexcept {
    auto first = std::get<SynonymWithAttr>(first_param_);
    auto second = std::get<SynonymWithAttr>(second_param_);
    // constant.value = constant2.value
    if (first.attribute_ == Attribute::kValue &&
        second.attribute_ == Attribute::kValue) {
        auto col_1 = pkb->GetAllEntityIndices(AttrToPkbType(first.attribute_));
        auto col_2 = col_1;
        return {first.synonym_, std::move(col_1), second.synonym_,
                std::move(col_2)};
    }
    // constant.value = stmt.stmt#
    if (first.attribute_ == Attribute::kValue) {
        return ValueStmt(pkb);
    }
    // print.stmt# = constant.value
    if (second.attribute_ == Attribute::kValue) {
        return ValueStmt(pkb);
    }
    // read.stmt# = print.stmt#
    if (first.synonym_->type != Synonym::Type::kStmtAny &&
        second.synonym_->type != Synonym::Type::kStmtAny) {
        return ResultTable(false);
    }
    // stmt.stmt# == read.stmt#
    if (first.synonym_->type == Synonym::Type::kStmtAny) {
        auto col_1 = pkb->GetAllEntityIndices(AttrToPkbType(second.attribute_));
        auto col_2 = col_1;
        return {first.synonym_, std::move(col_1), second.synonym_,
                std::move(col_2)};
    }
    // print.stmt# = stmt.stmt# || stmt.stmt# = stmt2.stmt#
    auto col_1 = pkb->GetAllEntityIndices(AttrToPkbType(first.attribute_));
    auto col_2 = col_1;
    return {first.synonym_, std::move(col_1), second.synonym_,
            std::move(col_2)};
}
ResultTable WithClause::ValueStmt(KnowledgeBase *pkb) const noexcept {
    auto first = std::get<SynonymWithAttr>(first_param_);
    auto second = std::get<SynonymWithAttr>(second_param_);
    assert(first.attribute_ == Attribute::kValue &&
           second.attribute_ == Attribute::kStmtNum);
    auto constants = pkb->GetAllEntityIndices(AttrToPkbType(first.attribute_));
    auto stmts = pkb->GetAllEntityIndices(AttrToPkbType(second.attribute_));
    std::list<std::string> names;
    pkb->ToName(AttrToPkbType(first.attribute_), constants, names);
    std::vector<int> col_1;
    std::vector<int> col_2;
    for (const auto &name : names) {
        int value = std::stoi(name);
        if (std::binary_search(stmts.begin(), stmts.end(), value)) {
            col_1.emplace_back(pkb->IdentToIndexValue(
                    name, AttrToPkbType(first.attribute_)));
            col_2.emplace_back(value);
        }
    }
    return {first.synonym_, std::move(col_1), second.synonym_,
            std::move(col_2)};
}
int WithClause::GetPriority() const noexcept {
    return kPriority[GetSynCount()];
}
int WithClause::GetSynCount() const noexcept {
    return kSynCount[static_cast<int>(type_)];
}
}  // namespace spa
