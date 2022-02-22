#include "uses_clause.h"

#include <utility>

namespace spa {
UsesIntIdent::UsesIntIdent(int first, std::string second)
        : first_(first), second_(std::move(second)) {}
ResultTable UsesIntIdent::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
UsesIntSyn::UsesIntSyn(int first, Synonym *second)
        : first_(first), second_(second) {}
ResultTable UsesIntSyn::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
UsesIntWild::UsesIntWild(int first) : first_(first) {}
ResultTable UsesIntWild::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
UsesSynIdent::UsesSynIdent(Synonym *first, std::string second)
        : first_(first), second_(std::move(second)) {}
ResultTable UsesSynIdent::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
UsesSynSyn::UsesSynSyn(Synonym *first, Synonym *second)
        : first_(first), second_(second) {}
ResultTable UsesSynSyn::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
UsesSynWild::UsesSynWild(Synonym *first) : first_(first) {}
ResultTable UsesSynWild::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
}  // namespace spa
