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
            bool isName =
                    std::get<SynonymWithAttr>(second_param_).attribute_ <=
                    Attribute::kVarName;
            return isName ? NameSyn(pkb,
                                    std::get<std::string_view>(first_param_),
                                    std::get<SynonymWithAttr>(second_param_))
                          : IntSyn(pkb,
                                   std::get<std::string_view>(first_param_),
                                   std::get<SynonymWithAttr>(second_param_));
        }
        case kSynSyn: {
            return SynSyn(pkb, std::get<SynonymWithAttr>(first_param_),
                          std::get<SynonymWithAttr>(second_param_));
        }
    }
    assert(false);
    return ResultTable(false);
}
ResultTable WithClause::RawRaw(KnowledgeBase *pkb, WithClause::Param first,
                               WithClause::Param second) noexcept {
    return ResultTable(first == second);
}
ResultTable WithClause::IntSyn(KnowledgeBase *pkb, std::string_view value,
                               SynonymWithAttr second) noexcept {
    // 3 = stmt.stmt#
    int index =
            pkb->IdentToIndexValue(value, AttrToPkbType(second.attribute_));
    return index == 0 ? ResultTable(false)
                      : ResultTable(second.synonym_, std::set<int>{index});
}
ResultTable WithClause::NameSyn(KnowledgeBase *pkb, std::string_view name,
                                SynonymWithAttr synonym_with_attr) noexcept {
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
ResultTable WithClause::SynSyn(KnowledgeBase *pkb, SynonymWithAttr first,
                               SynonymWithAttr second) noexcept {
    if (first.attribute_ == Attribute::kValue ||
        first.attribute_ == Attribute::kStmtNum) {
        return SynSynNum(pkb, first, second);
    }
    std::vector<int> col_1;
    std::vector<int> col_2;
    auto domain = pkb->GetAllEntityIndices(AttrToPkbType(first.attribute_));
    std::list<std::string> names;
    pkb->ToName(AttrToPkbType(first.attribute_), domain, names);
    for (const auto &name : names) {
        if (pkb->IdentToIndexValue(name, AttrToPkbType(second.attribute_)) !=
            0) {
            col_1.emplace_back(pkb->IdentToIndexValue(
                    name, AttrToPkbType(first.attribute_)));
            col_2.emplace_back(pkb->IdentToIndexValue(
                    name, AttrToPkbType(second.attribute_)));
        }
    }
    return {first.synonym_, std::move(col_1), second.synonym_,
            std::move(col_2)};
}
ResultTable WithClause::SynSynNum(KnowledgeBase *pkb, SynonymWithAttr first,
                                  SynonymWithAttr second) noexcept {
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
        return ValueStmt(pkb, first, second);
    }
    // print.stmt# = constant.value
    if (second.attribute_ == Attribute::kValue) {
        return ValueStmt(pkb, second, first);
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
ResultTable WithClause::ValueStmt(KnowledgeBase *pkb, SynonymWithAttr first,
                                  SynonymWithAttr second) noexcept {
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
    return type_ == kSynSyn ? 11 : 4;
}
}  // namespace spa
