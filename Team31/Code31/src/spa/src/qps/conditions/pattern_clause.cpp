#include "pattern_clause.h"

#include <string>
#include <utility>
#include <vector>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/type_convert_helpers.h"
#include "qps/query_token.h"

namespace spa {
PatternIdentExpr::PatternIdentExpr(Synonym *const assign, std::string first,
                                   std::vector<QueryToken> &&second,
                                   bool partial)
        : assign_(assign),
          first_(std::move(first)),
          second_(std::move(second)),
          partial_(partial) {}
ResultTable PatternIdentExpr::Execute(KnowledgeBase *knowledge_base) const {
    auto index_1st = knowledge_base->NameToIndex(QueryEntityType::kVar, first_);
    if (index_1st == 0) return ResultTable(false);
    auto result = knowledge_base->GetPattern(index_1st, second_, partial_);
    return {assign_, std::move(result)};
}
PatternIdentWild::PatternIdentWild(Synonym *const assign, std::string first)
        : assign_(assign), first_(std::move(first)) {}
ResultTable PatternIdentWild::Execute(KnowledgeBase *knowledge_base) const {
    auto index_1st = knowledge_base->NameToIndex(QueryEntityType::kVar, first_);
    if (index_1st == 0) return ResultTable(false);
    auto result = knowledge_base->GetPattern(index_1st);
    return {assign_, std::move(result)};
}

PatternSynExpr::PatternSynExpr(Synonym *const assign, Synonym *first,
                               std::vector<QueryToken> &&second, bool partial)
        : assign_(assign),
          first_(first),
          second_(std::move(second)),
          partial_(partial) {}
ResultTable PatternSynExpr::Execute(KnowledgeBase *knowledge_base) const {
    auto [col_1, col_2] = knowledge_base->GetPatternPair(second_, partial_);
    return {assign_, std::move(col_1), first_, std::move(col_2)};
}
PatternSynWild::PatternSynWild(Synonym *const assign, Synonym *first)
        : assign_(assign), first_(first) {}
ResultTable PatternSynWild::Execute(KnowledgeBase *knowledge_base) const {
    auto [col_1, col_2] = knowledge_base->GetPatternPair();
    return {assign_, std::move(col_1), first_, std::move(col_2)};
}
PatternWildExpr::PatternWildExpr(Synonym *const assign,
                                 std::vector<QueryToken> &&second, bool partial)
        : assign_(assign), second_(std::move(second)), partial_(partial) {}
ResultTable PatternWildExpr::Execute(KnowledgeBase *knowledge_base) const {
    auto result = knowledge_base->GetPattern(second_, partial_);
    return {assign_, std::move(result)};
}
PatternWildWild::PatternWildWild(Synonym *const assign) : assign_(assign){};
ResultTable PatternWildWild::Execute(KnowledgeBase *knowledge_base) const {
    auto result = knowledge_base->GetAllEntityIndices(StmtType::kAssign);
    return {assign_, {result.begin(), result.end()}};
}
}  // namespace spa
