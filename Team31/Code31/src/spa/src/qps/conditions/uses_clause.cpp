#include "uses_clause.h"

namespace spa {
UsesIntIdent::UsesIntIdent(int first, std::string second) {}
std::pair<ResultTable, bool> UsesIntIdent::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
UsesIntSyn::UsesIntSyn(int first, Synonym *second) {}
std::pair<ResultTable, bool> UsesIntSyn::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
UsesIntWild::UsesIntWild(int first) {}
std::pair<ResultTable, bool> UsesIntWild::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
UsesSynIdent::UsesSynIdent(Synonym *first, std::string second) {}
std::pair<ResultTable, bool> UsesSynIdent::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
UsesSynSyn::UsesSynSyn(Synonym *first, Synonym *second) {}
std::pair<ResultTable, bool> UsesSynSyn::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
UsesSynWild::UsesSynWild(Synonym *first) {}
std::pair<ResultTable, bool> UsesSynWild::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
}  // namespace spa
