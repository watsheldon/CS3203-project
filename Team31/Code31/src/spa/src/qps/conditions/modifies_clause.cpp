#include "modifies_clause.h"

namespace spa {
ModifiesIntIdent::ModifiesIntIdent(int first, std::string second)
        : first_(first), second_(std::move(second)) {}
std::pair<ResultTable, bool> ModifiesIntIdent::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ModifiesIntSyn::ModifiesIntSyn(int first, Synonym *second)
        : first_(first), second_(second) {}
std::pair<ResultTable, bool> ModifiesIntSyn::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ModifiesIntWild::ModifiesIntWild(int first) : first_(first) {}
std::pair<ResultTable, bool> ModifiesIntWild::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ModifiesSynIdent::ModifiesSynIdent(Synonym *first, std::string second)
        : first_(first), second_(std::move(second)) {}
std::pair<ResultTable, bool> ModifiesSynIdent::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ModifiesSynSyn::ModifiesSynSyn(Synonym *first, Synonym *second)
        : first_(first), second_(second) {}
std::pair<ResultTable, bool> ModifiesSynSyn::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ModifiesSynWild::ModifiesSynWild(Synonym *first) : first_(first) {}
std::pair<ResultTable, bool> ModifiesSynWild::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
}  // namespace spa
