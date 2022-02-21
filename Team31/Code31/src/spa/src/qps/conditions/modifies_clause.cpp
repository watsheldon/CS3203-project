#include "modifies_clause.h"

namespace spa {
ModifiesIntIdent::ModifiesIntIdent(int first, std::string second) {}
std::pair<ResultTable, bool> ModifiesIntIdent::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ModifiesIntSyn::ModifiesIntSyn(int first, Synonym *second) {}
std::pair<ResultTable, bool> ModifiesIntSyn::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ModifiesIntWild::ModifiesIntWild(int first) {}
std::pair<ResultTable, bool> ModifiesIntWild::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ModifiesSynIdent::ModifiesSynIdent(Synonym *first, std::string second) {}
std::pair<ResultTable, bool> ModifiesSynIdent::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ModifiesSynSyn::ModifiesSynSyn(Synonym *first, Synonym *second) {}
std::pair<ResultTable, bool> ModifiesSynSyn::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
ModifiesSynWild::ModifiesSynWild(Synonym *first) {}
std::pair<ResultTable, bool> ModifiesSynWild::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
}  // namespace spa
