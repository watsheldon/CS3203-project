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
ResultTable PatternIdentExpr::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
PatternIdentWild::PatternIdentWild(std::string first)
        : first_(std::move(first)) {}
ResultTable PatternIdentWild::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}

PatternSynExpr::PatternSynExpr(Synonym *first, std::vector<QueryToken> &&second,
                               bool partial)
        : first_(first), second_(std::move(second)), partial_(partial) {}
ResultTable PatternSynExpr::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
PatternSynWild::PatternSynWild(Synonym *first) : first_(first) {}
ResultTable PatternSynWild::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
PatternWildExpr::PatternWildExpr(std::vector<QueryToken> &&second, bool partial)
        : second_(std::move(second)), partial_(partial) {}
ResultTable PatternWildExpr::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
PatternWildWild::PatternWildWild() = default;
ResultTable PatternWildWild::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable(true);
}
}  // namespace spa
