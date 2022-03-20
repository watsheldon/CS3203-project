#include "follows_clause.h"

#include <cassert>
#include <utility>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/type_convert_helpers.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
ResultTable FollowsClause::Execute(
        KnowledgeBase *knowledge_base) const noexcept {
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
        case Type::kWildWild: {
            auto result = knowledge_base->ExistFollows();
            return ResultTable(result);
        }
        default:
            assert(false);
    }
}
}  // namespace spa
