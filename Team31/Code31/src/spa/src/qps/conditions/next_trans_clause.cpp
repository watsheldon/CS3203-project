#include "next_trans_clause.h"

#include <utility>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/type_convert_helpers.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
ResultTable NextTransClause::NumNum(KnowledgeBase *pkb, StmtNo first,
                                    StmtNo second) const noexcept {
    auto result = pkb->ExistNextT(Index<ArgPos::kFirst>(first),
                                  Index<ArgPos::kSecond>(second));
    return ResultTable(result);
}
ResultTable NextTransClause::NumSyn(KnowledgeBase *pkb, StmtNo first,
                                    Synonym *second) const noexcept {
    auto result =
            pkb->GetNextT(Index<ArgPos::kFirst>(first), SynToPkbType(second));
    return {second, std::move(result)};
}
ResultTable NextTransClause::SynNum(KnowledgeBase *pkb, Synonym *first,
                                    StmtNo second) const noexcept {
    auto result =
            pkb->GetNextT(Index<ArgPos::kSecond>(second), SynToPkbType(first));
    return {first, std::move(result)};
}
ResultTable NextTransClause::SynSyn(KnowledgeBase *pkb, Synonym *first,
                                    Synonym *second) const noexcept {
    if (first == second) {
        auto result = pkb->GetNextTSelf(SynToPkbType(first));
        return {first, std::move(result)};
    }
    auto [col_1, col_2] =
            pkb->GetNextTPairs(SynToPkbType(first), SynToPkbType(second));
    return {first, std::move(col_1), second, std::move(col_2)};
}
}  // namespace spa
