#include "follows_clause.h"

namespace spa {
FollowsIntInt::FollowsIntInt(int first, int second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
ResultTable FollowsIntInt::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
FollowsIntSyn::FollowsIntSyn(int first, Synonym *second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
ResultTable FollowsIntSyn::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
FollowsIntWild::FollowsIntWild(int first) : first_(first) {}
ResultTable FollowsIntWild::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
FollowsSynInt::FollowsSynInt(Synonym *first, int second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
ResultTable FollowsSynInt::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
FollowsSynSyn::FollowsSynSyn(Synonym *first, Synonym *second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
ResultTable FollowsSynSyn::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
FollowsSynWild::FollowsSynWild(Synonym *first) : first_(first) {}
ResultTable FollowsSynWild::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
FollowsWildInt::FollowsWildInt(int second) : second_(second) {}
ResultTable FollowsWildInt::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
FollowsWildSyn::FollowsWildSyn(Synonym *second) : second_(second) {}
ResultTable FollowsWildSyn::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
FollowsWildWild::FollowsWildWild() = default;
ResultTable FollowsWildWild::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
}  // namespace spa
