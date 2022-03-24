#include "follows_clause.h"

#include <utility>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/type_convert_helpers.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"
#include "qps/synonym.h"

namespace spa {
ResultTable FollowsClause::NumNum(KnowledgeBase *pkb, StmtNo first,
                                  StmtNo second) const noexcept {
    auto result = pkb->ExistFollows(Index<ArgPos::kFirst>(first),
                                    Index<ArgPos::kSecond>(second));
    return ResultTable(result);
}
ResultTable FollowsClause::NumSyn(KnowledgeBase *pkb, StmtNo first,
                                  Synonym *second) const noexcept {
    auto result =
            pkb->GetFollows(Index<ArgPos::kFirst>(first), SynToPkbType(second));
    return {second, std::move(result)};
}
ResultTable FollowsClause::SynNum(KnowledgeBase *pkb, Synonym *first,
                                  StmtNo second) const noexcept {
    auto result = pkb->GetFollows(Index<ArgPos::kSecond>(second),
                                  SynToPkbType(first));
    return {first, std::move(result)};
}
ResultTable FollowsClause::SynSyn(KnowledgeBase *pkb, Synonym *first,
                                  Synonym *second) const noexcept {
    auto [col_1, col_2] =
            pkb->GetFollowsPairs(SynToPkbType(first), SynToPkbType(second));
    return {first, std::move(col_1), second, std::move(col_2)};
}
}  // namespace spa
