#include "follows_clause.h"

namespace spa {
FollowsIntInt::FollowsIntInt(int first, int second, bool isTrans) {}
std::pair<ResultTable, bool> FollowsIntInt::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}

FollowsIntSyn::FollowsIntSyn(int first, Synonym *second, bool isTrans) {}
std::pair<ResultTable, bool> FollowsIntSyn::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
FollowsIntWild::FollowsIntWild(int first, bool isTrans) {}
std::pair<ResultTable, bool> FollowsIntWild::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
FollowsSynInt::FollowsSynInt(Synonym *first, int second, bool isTrans) {}
std::pair<ResultTable, bool> FollowsSynInt::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
FollowsSynSyn::FollowsSynSyn(Synonym *first, Synonym *second, bool isTrans) {}
std::pair<ResultTable, bool> FollowsSynSyn::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
FollowsSynWild::FollowsSynWild(Synonym *first, bool isTrans) {}
std::pair<ResultTable, bool> FollowsSynWild::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
FollowsWildInt::FollowsWildInt(int second, bool isTrans) {}
std::pair<ResultTable, bool> FollowsWildInt::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
FollowsWildSyn::FollowsWildSyn(Synonym *second, bool isTrans) {}
std::pair<ResultTable, bool> FollowsWildSyn::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
FollowsWildWild::FollowsWildWild() {}
std::pair<ResultTable, bool> FollowsWildWild::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
}  // namespace spa
