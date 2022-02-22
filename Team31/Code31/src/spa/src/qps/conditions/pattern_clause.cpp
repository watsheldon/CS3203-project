#include "pattern_clause.h"

#include <string>
#include <utility>
#include <vector>

#include "qps/query_token.h"

namespace spa {
PatternIdentExpr::PatternIdentExpr(std::string first,
                                   std::vector<QueryToken> &&second,
                                   bool partial)
        : first_(std::move(first)),
          second_(std::move(second)),
          partial_(partial) {}
std::pair<ResultTable, bool> PatternIdentExpr::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
PatternIdentWild::PatternIdentWild(std::string first)
        : first_(std::move(first)) {}
std::pair<ResultTable, bool> PatternIdentWild::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}

PatternSynExpr::PatternSynExpr(Synonym *first, std::vector<QueryToken> &&second,
                               bool partial)
        : first_(first), second_(std::move(second)), partial_(partial) {}
std::pair<ResultTable, bool> PatternSynExpr::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
PatternSynWild::PatternSynWild(Synonym *first) : first_(first) {}
std::pair<ResultTable, bool> PatternSynWild::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
PatternWildExpr::PatternWildExpr(std::vector<QueryToken> &&second, bool partial)
        : second_(std::move(second)), partial_(partial) {}
std::pair<ResultTable, bool> PatternWildExpr::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
PatternWildWild::PatternWildWild() = default;
std::pair<ResultTable, bool> PatternWildWild::Execute(
        KnowledgeBase *knowledge_base) const {
    return {ResultTable(true), true};
}
}  // namespace spa
