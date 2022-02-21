#include "pattern_clause.h"

namespace spa {
PatternIdentExpr::PatternIdentExpr(std::string first,
                                   std::vector<QueryToken> second) {}
std::pair<ResultTable, bool> PatternIdentExpr::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
PatternIdentWild::PatternIdentWild(std::string first) {}
std::pair<ResultTable, bool> PatternIdentWild::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}

PatternSynExpr::PatternSynExpr(Synonym *first, std::vector<QueryToken> second) {

}
std::pair<ResultTable, bool> PatternSynExpr::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
PatternSynWild::PatternSynWild(Synonym *first) {}
std::pair<ResultTable, bool> PatternSynWild::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
PatternWildExpr::PatternWildExpr(std::vector<QueryToken> second) {}
std::pair<ResultTable, bool> PatternWildExpr::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
PatternWildWild::PatternWildWild() {}
std::pair<ResultTable, bool> PatternWildWild::Execute(
        const KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
}  // namespace spa
