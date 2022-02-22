#include "modifies_clause.h"

namespace spa {
ModifiesIntIdent::ModifiesIntIdent(int first, std::string second)
        : first_(first), second_(std::move(second)) {}
ResultTable ModifiesIntIdent::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
ModifiesIntSyn::ModifiesIntSyn(int first, Synonym *second)
        : first_(first), second_(second) {}
ResultTable ModifiesIntSyn::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
ModifiesIntWild::ModifiesIntWild(int first) : first_(first) {}
ResultTable ModifiesIntWild::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
ModifiesSynIdent::ModifiesSynIdent(Synonym *first, std::string second)
        : first_(first), second_(std::move(second)) {}
ResultTable ModifiesSynIdent::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
ModifiesSynSyn::ModifiesSynSyn(Synonym *first, Synonym *second)
        : first_(first), second_(second) {}
ResultTable ModifiesSynSyn::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
ModifiesSynWild::ModifiesSynWild(Synonym *first) : first_(first) {}
ResultTable ModifiesSynWild::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
}  // namespace spa
