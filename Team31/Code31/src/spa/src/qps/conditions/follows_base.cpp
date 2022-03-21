#include "follows_base.h"

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "common/type_convert_helpers.h"
#include "pkb/knowledge_base.h"
#include "qps/synonym.h"

namespace spa {
ResultTable FollowsBase::NumWild(KnowledgeBase *pkb,
                                 StmtNo first) const noexcept {
    auto result = pkb->ExistFollows(Index<ArgPos::kFirst>(first));
    return ResultTable(result);
}
ResultTable FollowsBase::SynWild(KnowledgeBase *pkb,
                                 Synonym *first) const noexcept {
    auto result = pkb->GetFollows(ArgPos::kFirst, SynToPkbType(first));
    return {first, std::move(result)};
}
ResultTable FollowsBase::WildNum(KnowledgeBase *pkb,
                                 StmtNo second) const noexcept {
    auto result = pkb->ExistFollows(Index<ArgPos::kSecond>(second));
    return ResultTable(result);
}
ResultTable FollowsBase::WildSyn(KnowledgeBase *pkb,
                                 Synonym *second) const noexcept {
    auto result = pkb->GetFollows(ArgPos::kSecond, SynToPkbType(second));
    return {second, std::move(result)};
}
ResultTable FollowsBase::WildWild(KnowledgeBase *pkb) const noexcept {
    return ResultTable(pkb->ExistFollows());
}
}  // namespace spa
