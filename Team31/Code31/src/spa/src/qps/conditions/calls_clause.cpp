#include "calls_clause.h"

#include <cassert>
#include <utility>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/type_convert_helpers.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
ResultTable CallsClause::Execute(KnowledgeBase *knowledge_base) const noexcept {
    switch (type_) {
        case kSynSyn: {
            auto [col_1, col_2] = knowledge_base->GetCallsPairs();
            return {first_syn_, std::move(col_1), second_syn_,
                    std::move(col_2)};
        }
        case kSynWild: {
            auto result = knowledge_base->GetCalls(ArgPos::kFirst);
            return {first_syn_, std::move(result)};
        }
        case kSynIdent: {
            auto result = knowledge_base->GetCalls(
                    Name<ArgPos::kSecond>(second_ident_));
            return {first_syn_, std::move(result)};
        }
        case kWildSyn: {
            auto result = knowledge_base->GetCalls(ArgPos::kSecond);
            return {second_syn_, std::move(result)};
        }
        case kWildWild: {
            auto result = knowledge_base->ExistCalls();
            return ResultTable(result);
        }
        case kWildIdent: {
            auto result = knowledge_base->ExistCalls(
                    Name<ArgPos::kSecond>(second_ident_));
            return ResultTable(result);
        }
        case kIdentSyn: {
            auto result = knowledge_base->GetCalls(
                    Name<ArgPos::kFirst>(first_ident_));
            return {second_syn_, std::move(result)};
        }
        case kIdentWild: {
            auto result = knowledge_base->ExistCalls(
                    Name<ArgPos::kFirst>(first_ident_));
            return ResultTable(result);
        }
        case kIdentIdent: {
            auto result =
                    knowledge_base->ExistCalls(first_ident_, second_ident_);
            return ResultTable(result);
        }
        default:
            assert(false);
    }
}
}  // namespace spa
