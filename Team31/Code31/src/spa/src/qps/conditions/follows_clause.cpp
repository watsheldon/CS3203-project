#include "follows_clause.h"

#include <utility>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/type_convert_helpers.h"

namespace spa {
FollowsIntInt::FollowsIntInt(int first, int second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
ResultTable FollowsIntInt::Execute(KnowledgeBase *knowledge_base) const {
    auto result = knowledge_base->ExistFollows(transitive_,
                                               Index<ArgPos::kFirst>(first_),
                                               Index<ArgPos::kSecond>(second_));
    return ResultTable(result);
}
FollowsIntSyn::FollowsIntSyn(int first, Synonym *second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
ResultTable FollowsIntSyn::Execute(KnowledgeBase *knowledge_base) const {
    auto result = knowledge_base->GetFollows(
            transitive_, Index<ArgPos::kFirst>(first_), SynToPkbType(second_));
    return {second_, std::move(result)};
}
FollowsIntWild::FollowsIntWild(int first) : first_(first) {}
ResultTable FollowsIntWild::Execute(KnowledgeBase *knowledge_base) const {
    auto result = knowledge_base->ExistFollows(Index<ArgPos::kFirst>(first_));
    return ResultTable(result);
}
FollowsSynInt::FollowsSynInt(Synonym *first, int second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
ResultTable FollowsSynInt::Execute(KnowledgeBase *knowledge_base) const {
    auto result = knowledge_base->GetFollows(
            transitive_, Index<ArgPos::kSecond>(second_), SynToPkbType(first_));
    return {first_, std::move(result)};
}
FollowsSynSyn::FollowsSynSyn(Synonym *first, Synonym *second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
ResultTable FollowsSynSyn::Execute(KnowledgeBase *knowledge_base) const {
    auto [col_1, col_2] = knowledge_base->GetFollowsPairs(
            transitive_, SynToPkbType(first_), SynToPkbType(second_));
    return {first_, std::move(col_1), second_, std::move(col_2)};
}
FollowsSynWild::FollowsSynWild(Synonym *first) : first_(first) {}
ResultTable FollowsSynWild::Execute(KnowledgeBase *knowledge_base) const {
    auto result =
            knowledge_base->GetFollows(ArgPos::kFirst, SynToPkbType(first_));
    return {first_, std::move(result)};
}
FollowsWildInt::FollowsWildInt(int second) : second_(second) {}
ResultTable FollowsWildInt::Execute(KnowledgeBase *knowledge_base) const {
    auto result = knowledge_base->ExistFollows(Index<ArgPos::kSecond>(second_));
    return ResultTable(result);
}
FollowsWildSyn::FollowsWildSyn(Synonym *second) : second_(second) {}
ResultTable FollowsWildSyn::Execute(KnowledgeBase *knowledge_base) const {
    auto result =
            knowledge_base->GetFollows(ArgPos::kSecond, SynToPkbType(second_));
    return {second_, std::move(result)};
}
FollowsWildWild::FollowsWildWild() = default;
ResultTable FollowsWildWild::Execute(KnowledgeBase *knowledge_base) const {
    auto result = knowledge_base->ExistFollows();
    return ResultTable(result);
}
}  // namespace spa
