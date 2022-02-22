#include "uses_clause.h"

#include <utility>

namespace spa {
UsesIntIdent::UsesIntIdent(int first, std::string second)
        : first_(first), second_(std::move(second)) {}
std::pair<ResultTable, bool> UsesIntIdent::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
UsesIntSyn::UsesIntSyn(int first, Synonym *second)
        : first_(first), second_(second) {}
std::pair<ResultTable, bool> UsesIntSyn::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
UsesIntWild::UsesIntWild(int first) : first_(first) {}
std::pair<ResultTable, bool> UsesIntWild::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
UsesSynIdent::UsesSynIdent(Synonym *first, std::string second)
        : first_(first), second_(std::move(second)) {}
std::pair<ResultTable, bool> UsesSynIdent::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
UsesSynSyn::UsesSynSyn(Synonym *first, Synonym *second)
        : first_(first), second_(second) {}
std::pair<ResultTable, bool> UsesSynSyn::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
UsesSynWild::UsesSynWild(Synonym *first) : first_(first) {}
std::pair<ResultTable, bool> UsesSynWild::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
}  // namespace spa
