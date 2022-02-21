#include "parent_clause.h"

namespace spa {
ParentIntInt::ParentIntInt(int first, int second, bool isTrans) {}
std::pair<ResultTable, bool> ParentIntInt::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ParentIntSyn::ParentIntSyn(int first, Synonym *second, bool isTrans) {}
std::pair<ResultTable, bool> ParentIntSyn::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ParentIntWild::ParentIntWild(int first, bool isTrans) {}
std::pair<ResultTable, bool> ParentIntWild::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ParentSynInt::ParentSynInt(Synonym *first, int second, bool isTrans) {}
std::pair<ResultTable, bool> ParentSynInt::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ParentSynSyn::ParentSynSyn(Synonym *first, Synonym *second, bool isTrans) {}
std::pair<ResultTable, bool> ParentSynSyn::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ParentSynWild::ParentSynWild(Synonym *first, bool isTrans) {}
std::pair<ResultTable, bool> ParentSynWild::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ParentWildInt::ParentWildInt(int second, bool isTrans) {}
std::pair<ResultTable, bool> ParentWildInt::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ParentWildSyn::ParentWildSyn(Synonym *second, bool isTrans) {}
std::pair<ResultTable, bool> ParentWildSyn::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ParentWildWild::ParentWildWild() {}
std::pair<ResultTable, bool> ParentWildWild::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
}  // namespace spa
