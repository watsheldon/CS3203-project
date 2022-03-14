#include "parent_clause.h"

#include <utility>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/type_convert_helpers.h"

namespace spa {
ParentClause::ParentClause() : type_(Type::kWildWild) {}
ParentClause::ParentClause(int first, int second)
        : type_(Type::kIntInt), first_int_(first), second_int_(second) {}
ParentClause::ParentClause(int first, Synonym *second)
        : type_(Type::kIntSyn),
          first_int_(first),
          second_syn_(std::move(second)) {}
ParentClause::ParentClause(Synonym *first, int second)
        : type_(Type::kSynInt),
          first_syn_(std::move(first)),
          second_int_(second) {}
ParentClause::ParentClause(Synonym *first, Synonym *second)
        : type_(Type::kSynSyn),
          first_syn_(std::move(first)),
          second_syn_(std::move(second)) {}
ParentClause::ParentClause(ArgPos pos, int integer) {
    if (pos == ArgPos::kFirst) {
        type_ = Type::kIntWild;
        first_int_ = integer;
    } else {
        assert(pos == ArgPos::kSecond);
        type_ = Type::kWildInt;
        second_int_ = integer;
    }
}
ParentClause::ParentClause(ArgPos pos, Synonym *syn) {
    if (pos == ArgPos::kFirst) {
        type_ = Type::kIntWild;
        first_syn_ = syn;
    } else {
        assert(pos == ArgPos::kSecond);
        type_ = Type::kWildInt;
        second_syn_ = syn;
    }
}
ResultTable ParentClause::Execute(KnowledgeBase *knowledge_base) const {
    switch (type_) {
        case Type::kIntInt: {
            auto result = knowledge_base->ExistParent(
                    false, Index<ArgPos::kFirst>(first_int_),
                    Index<ArgPos::kSecond>(second_int_));
            return ResultTable(result);
        }
        case Type::kIntSyn: {
            auto result = knowledge_base->GetParent(
                    false, Index<ArgPos::kFirst>(first_int_),
                    SynToPkbType(second_syn_));
            return {second_syn_, std::move(result)};
        }
        case Type::kIntWild: {
            auto result = knowledge_base->ExistParent(
                    Index<ArgPos::kFirst>(first_int_));
            return ResultTable(result);
        }
        case Type::kSynInt: {
            auto result = knowledge_base->GetParent(
                    false, Index<ArgPos::kSecond>(second_int_),
                    SynToPkbType(first_syn_));
            return {first_syn_, std::move(result)};
        }
        case Type::kSynSyn: {
            auto [col_1, col_2] = knowledge_base->GetParentPairs(
                    false, SynToPkbType(first_syn_), SynToPkbType(second_syn_));
            return {first_syn_, std::move(col_1), second_syn_,
                    std::move(col_2)};
        }
        case Type::kSynWild: {
            auto result = knowledge_base->GetParent(ArgPos::kFirst,
                                                    SynToPkbType(first_syn_));
            return {first_syn_, std::move(result)};
        }
        case Type::kWildInt: {
            auto result = knowledge_base->ExistParent(
                    Index<ArgPos::kSecond>(second_int_));
            return ResultTable(result);
        }
        case Type::kWildSyn: {
            auto result = knowledge_base->GetParent(ArgPos::kSecond,
                                                    SynToPkbType(second_syn_));
            return {second_syn_, std::move(result)};
        }
        case Type::kWildWild: {
            auto result = knowledge_base->ExistParent();
            return ResultTable(result);
        }
    }
}
ParentTransClause::ParentTransClause() : type_(Type::kWildWild) {}
ParentTransClause::ParentTransClause(int first, int second)
        : type_(Type::kIntInt), first_int_(first), second_int_(second) {}
ParentTransClause::ParentTransClause(int first, Synonym *second)
        : type_(Type::kIntSyn),
          first_int_(first),
          second_syn_(std::move(second)) {}
ParentTransClause::ParentTransClause(Synonym *first, int second)
        : type_(Type::kSynInt),
          first_syn_(std::move(first)),
          second_int_(second) {}
ParentTransClause::ParentTransClause(Synonym *first, Synonym *second)
        : type_(Type::kSynSyn),
          first_syn_(std::move(first)),
          second_syn_(std::move(second)) {}
ParentTransClause::ParentTransClause(ArgPos pos, int integer) {
    if (pos == ArgPos::kFirst) {
        type_ = Type::kIntWild;
        first_int_ = integer;
    } else {
        assert(pos == ArgPos::kSecond);
        type_ = Type::kWildInt;
        second_int_ = integer;
    }
}
ParentTransClause::ParentTransClause(ArgPos pos, Synonym *syn) {
    if (pos == ArgPos::kFirst) {
        type_ = Type::kIntWild;
        first_syn_ = std::move(syn);
    } else {
        assert(pos == ArgPos::kSecond);
        type_ = Type::kWildInt;
        second_syn_ = std::move(syn);
    }
}
ResultTable ParentTransClause::Execute(KnowledgeBase *knowledge_base) const {
    switch (type_) {
        case Type::kIntInt: {
            auto result = knowledge_base->ExistParent(
                    true, Index<ArgPos::kFirst>(first_int_),
                    Index<ArgPos::kSecond>(second_int_));
            return ResultTable(result);
        }
        case Type::kIntSyn: {
            auto result = knowledge_base->GetParent(
                    true, Index<ArgPos::kFirst>(first_int_),
                    SynToPkbType(second_syn_));
            return {second_syn_, std::move(result)};
        }
        case Type::kIntWild: {
            auto result = knowledge_base->ExistParent(
                    Index<ArgPos::kFirst>(first_int_));
            return ResultTable(result);
        }
        case Type::kSynInt: {
            auto result = knowledge_base->GetParent(
                    true, Index<ArgPos::kSecond>(second_int_),
                    SynToPkbType(first_syn_));
            return {first_syn_, std::move(result)};
        }
        case Type::kSynSyn: {
            auto [col_1, col_2] = knowledge_base->GetParentPairs(
                    true, SynToPkbType(first_syn_), SynToPkbType(second_syn_));
            return {first_syn_, std::move(col_1), second_syn_,
                    std::move(col_2)};
        }
        case Type::kSynWild: {
            auto result = knowledge_base->GetParent(ArgPos::kFirst,
                                                    SynToPkbType(first_syn_));
            return {first_syn_, std::move(result)};
        }
        case Type::kWildInt: {
            auto result = knowledge_base->ExistParent(
                    Index<ArgPos::kSecond>(second_int_));
            return ResultTable(result);
        }
        case Type::kWildSyn: {
            auto result = knowledge_base->GetParent(ArgPos::kSecond,
                                                    SynToPkbType(second_syn_));
            return {second_syn_, std::move(result)};
        }
        case Type::kWildWild: {
            auto result = knowledge_base->ExistParent();
            return ResultTable(result);
        }
    }
}
}  // namespace spa