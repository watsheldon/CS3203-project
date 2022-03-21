#include "parent_trans_clause.h"

#include <utility>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/type_convert_helpers.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
ResultTable ParentTransClause::NumNum(KnowledgeBase *pkb, StmtNo first,
                                      StmtNo second) const noexcept {
    auto result = pkb->ExistParent(true, Index<ArgPos::kFirst>(first),
                                   Index<ArgPos::kSecond>(second));
    return ResultTable(result);
}
ResultTable ParentTransClause::NumSyn(KnowledgeBase *pkb, StmtNo first,
                                      Synonym *second) const noexcept {
    auto result = pkb->GetParent(true, Index<ArgPos::kFirst>(first),
                                 SynToPkbType(second));
    return {second, std::move(result)};
}
ResultTable ParentTransClause::SynNum(KnowledgeBase *pkb, Synonym *first,
                                      StmtNo second) const noexcept {
    auto result = pkb->GetParent(true, Index<ArgPos::kSecond>(second),
                                 SynToPkbType(first));
    return {first, std::move(result)};
}
ResultTable ParentTransClause::SynSyn(KnowledgeBase *pkb, Synonym *first,
                                      Synonym *second) const noexcept {
    auto [col_1, col_2] = pkb->GetParentPairs(true, SynToPkbType(first),
                                              SynToPkbType(second));
    return {first, std::move(col_1), second, std::move(col_2)};
}
}  // namespace spa
