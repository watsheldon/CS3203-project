#include "parent_clause.h"

namespace spa {
ParentIntInt::ParentIntInt(int first, int second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
std::pair<ResultTable, bool> ParentIntInt::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ParentIntSyn::ParentIntSyn(int first, Synonym *second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
std::pair<ResultTable, bool> ParentIntSyn::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ParentIntWild::ParentIntWild(int first) : first_(first) {}
std::pair<ResultTable, bool> ParentIntWild::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ParentSynInt::ParentSynInt(Synonym *first, int second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
std::pair<ResultTable, bool> ParentSynInt::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ParentSynSyn::ParentSynSyn(Synonym *first, Synonym *second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
std::pair<ResultTable, bool> ParentSynSyn::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ParentSynWild::ParentSynWild(Synonym *first) : first_(first) {}
std::pair<ResultTable, bool> ParentSynWild::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ParentWildInt::ParentWildInt(int second) : second_(second) {}
std::pair<ResultTable, bool> ParentWildInt::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ParentWildSyn::ParentWildSyn(Synonym *second) : second_(second) {}
std::pair<ResultTable, bool> ParentWildSyn::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ParentWildWild::ParentWildWild() = default;
std::pair<ResultTable, bool> ParentWildWild::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
}  // namespace spa
