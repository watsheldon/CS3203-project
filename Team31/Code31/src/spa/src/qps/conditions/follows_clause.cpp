#include "follows_clause.h"

#include <utility>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/type_convert_helpers.h"

namespace spa {
FollowsClause::FollowsClause() : type_(Type::kWildWild) {}
FollowsClause::FollowsClause(int first, int second)
        : type_(Type::kIntInt), first_int_(first), second_int_(second) {}
FollowsClause::FollowsClause(int first, Synonym *second)
        : type_(Type::kIntSyn),
          first_int_(first),
          second_syn_(std::move(second)) {}
FollowsClause::FollowsClause(Synonym *first, int second)
        : type_(Type::kSynInt),
          first_syn_(std::move(first)),
          second_int_(second) {}
FollowsClause::FollowsClause(Synonym *first, Synonym *second)
        : type_(Type::kSynSyn),
          first_syn_(std::move(first)),
          second_syn_(std::move(second)) {}
FollowsClause::FollowsClause(ArgPos pos, int integer) {
    if (pos == ArgPos::kFirst) {
        type_ = Type::kIntWild;
        first_int_ = integer;
    } else {
        assert(pos == ArgPos::kSecond);
        type_ = Type::kWildInt;
        second_int_ = integer;
    }
}
FollowsClause::FollowsClause(ArgPos pos, Synonym *syn) {
    if (pos == ArgPos::kFirst) {
        type_ = Type::kIntWild;
        first_syn_ = syn;
    } else {
        assert(pos == ArgPos::kSecond);
        type_ = Type::kWildInt;
        second_syn_ = syn;
    }
}
ResultTable FollowsClause::Execute(KnowledgeBase *knowledge_base) const {
    switch (type_) {
        case Type::kIntInt: {
            auto result = knowledge_base->ExistFollows(
                    false, Index<ArgPos::kFirst>(first_int_),
                    Index<ArgPos::kSecond>(second_int_));
            return ResultTable(result);
        }
        case Type::kIntSyn: {
            auto result = knowledge_base->GetFollows(
                    false, Index<ArgPos::kFirst>(first_int_),
                    SynToPkbType(second_syn_));
            return {second_syn_, std::move(result)};
        }
        case Type::kIntWild: {
            auto result = knowledge_base->ExistFollows(
                    Index<ArgPos::kFirst>(first_int_));
            return ResultTable(result);
        }
        case Type::kSynInt: {
            auto result = knowledge_base->GetFollows(
                    false, Index<ArgPos::kSecond>(second_int_),
                    SynToPkbType(first_syn_));
            return {first_syn_, std::move(result)};
        }
        case Type::kSynSyn: {
            auto [col_1, col_2] = knowledge_base->GetFollowsPairs(
                    false, SynToPkbType(first_syn_), SynToPkbType(second_syn_));
            return {first_syn_, std::move(col_1), second_syn_,
                    std::move(col_2)};
        }
        case Type::kSynWild: {
            auto result = knowledge_base->GetFollows(ArgPos::kFirst,
                                                     SynToPkbType(first_syn_));
            return {first_syn_, std::move(result)};
        }
        case Type::kWildInt: {
            auto result = knowledge_base->ExistFollows(
                    Index<ArgPos::kSecond>(second_int_));
            return ResultTable(result);
        }
        case Type::kWildSyn: {
            auto result = knowledge_base->GetFollows(ArgPos::kSecond,
                                                     SynToPkbType(second_syn_));
            return {second_syn_, std::move(result)};
        }
        case Type::kWildWild:
            auto result = knowledge_base->ExistFollows();
            return ResultTable(result);
    }
}
FollowsTransClause::FollowsTransClause() : type_(Type::kWildWild) {}
FollowsTransClause::FollowsTransClause(int first, int second)
        : type_(Type::kIntInt), first_int_(first), second_int_(second) {}
FollowsTransClause::FollowsTransClause(int first, Synonym *second)
        : type_(Type::kIntSyn),
          first_int_(first),
          second_syn_(std::move(second)) {}
FollowsTransClause::FollowsTransClause(Synonym *first, int second)
        : type_(Type::kSynInt),
          first_syn_(std::move(first)),
          second_int_(second) {}
FollowsTransClause::FollowsTransClause(Synonym *first, Synonym *second)
        : type_(Type::kSynSyn),
          first_syn_(std::move(first)),
          second_syn_(std::move(second)) {}
FollowsTransClause::FollowsTransClause(ArgPos pos, int integer) {
    if (pos == ArgPos::kFirst) {
        type_ = Type::kIntWild;
        first_int_ = integer;
    } else {
        assert(pos == ArgPos::kSecond);
        type_ = Type::kWildInt;
        second_int_ = integer;
    }
}
FollowsTransClause::FollowsTransClause(ArgPos pos, Synonym *syn) {
    if (pos == ArgPos::kFirst) {
        type_ = Type::kIntWild;
        first_syn_ = std::move(syn);
    } else {
        assert(pos == ArgPos::kSecond);
        type_ = Type::kWildInt;
        second_syn_ = std::move(syn);
    }
}
ResultTable FollowsTransClause::Execute(KnowledgeBase *knowledge_base) const {
    switch (type_) {
        case Type::kIntInt: {
            auto result = knowledge_base->ExistFollows(
                    true, Index<ArgPos::kFirst>(first_int_),
                    Index<ArgPos::kSecond>(second_int_));
            return ResultTable(result);
        }
        case Type::kIntSyn: {
            auto result = knowledge_base->GetFollows(
                    true, Index<ArgPos::kFirst>(first_int_),
                    SynToPkbType(second_syn_));
            return {second_syn_, std::move(result)};
        }
        case Type::kIntWild: {
            auto result = knowledge_base->ExistFollows(
                    Index<ArgPos::kFirst>(first_int_));
            return ResultTable(result);
        }
        case Type::kSynInt: {
            auto result = knowledge_base->GetFollows(
                    true, Index<ArgPos::kSecond>(second_int_),
                    SynToPkbType(first_syn_));
            return {first_syn_, std::move(result)};
        }
        case Type::kSynSyn: {
            auto [col_1, col_2] = knowledge_base->GetFollowsPairs(
                    true, SynToPkbType(first_syn_), SynToPkbType(second_syn_));
            return {first_syn_, std::move(col_1), second_syn_,
                    std::move(col_2)};
        }
        case Type::kSynWild: {
            auto result = knowledge_base->GetFollows(ArgPos::kFirst,
                                                     SynToPkbType(first_syn_));
            return {first_syn_, std::move(result)};
        }
        case Type::kWildInt: {
            auto result = knowledge_base->ExistFollows(
                    Index<ArgPos::kSecond>(second_int_));
            return ResultTable(result);
        }
        case Type::kWildSyn: {
            auto result = knowledge_base->GetFollows(ArgPos::kSecond,
                                                     SynToPkbType(second_syn_));
            return {second_syn_, std::move(result)};
        }
        case Type::kWildWild:
            auto result = knowledge_base->ExistFollows();
            return ResultTable(result);
    }
}
}  // namespace spa
