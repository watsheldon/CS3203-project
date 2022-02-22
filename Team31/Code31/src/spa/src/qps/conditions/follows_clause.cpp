#include "follows_clause.h"

namespace spa {
FollowsIntInt::FollowsIntInt(int first, int second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
std::pair<ResultTable, bool> FollowsIntInt::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
FollowsIntSyn::FollowsIntSyn(int first, Synonym *second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
std::pair<ResultTable, bool> FollowsIntSyn::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
FollowsIntWild::FollowsIntWild(int first) : first_(first) {}
std::pair<ResultTable, bool> FollowsIntWild::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
FollowsSynInt::FollowsSynInt(Synonym *first, int second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
std::pair<ResultTable, bool> FollowsSynInt::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
FollowsSynSyn::FollowsSynSyn(Synonym *first, Synonym *second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
std::pair<ResultTable, bool> FollowsSynSyn::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
FollowsSynWild::FollowsSynWild(Synonym *first) : first_(first) {}
std::pair<ResultTable, bool> FollowsSynWild::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
FollowsWildInt::FollowsWildInt(int second) : second_(second) {}
std::pair<ResultTable, bool> FollowsWildInt::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
FollowsWildSyn::FollowsWildSyn(Synonym *second) : second_(second) {}
std::pair<ResultTable, bool> FollowsWildSyn::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
FollowsWildWild::FollowsWildWild() = default;
std::pair<ResultTable, bool> FollowsWildWild::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
}  // namespace spa
