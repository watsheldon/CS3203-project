#include "parent_clause.h"

namespace spa {
ParentIntInt::ParentIntInt(int first, int second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
ResultTable ParentIntInt::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
ParentIntSyn::ParentIntSyn(int first, Synonym *second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
ResultTable ParentIntSyn::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
ParentIntWild::ParentIntWild(int first) : first_(first) {}
ResultTable ParentIntWild::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
ParentSynInt::ParentSynInt(Synonym *first, int second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
ResultTable ParentSynInt::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
ParentSynSyn::ParentSynSyn(Synonym *first, Synonym *second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
ResultTable ParentSynSyn::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
ParentSynWild::ParentSynWild(Synonym *first) : first_(first) {}
ResultTable ParentSynWild::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
ParentWildInt::ParentWildInt(int second) : second_(second) {}
ResultTable ParentWildInt::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
ParentWildSyn::ParentWildSyn(Synonym *second) : second_(second) {}
ResultTable ParentWildSyn::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
ParentWildWild::ParentWildWild() = default;
ResultTable ParentWildWild::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
}  // namespace spa
