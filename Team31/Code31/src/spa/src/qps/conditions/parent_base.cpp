#include "parent_base.h"

#include <utility>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/type_convert_helpers.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
ResultTable ParentBase::NumWild(KnowledgeBase* pkb,
                                StmtNo first) const noexcept {
    auto result = pkb->ExistParent(Index<ArgPos::kFirst>(first));
    return ResultTable(result);
}
ResultTable ParentBase::SynWild(KnowledgeBase* pkb,
                                Synonym* first) const noexcept {
    auto result = pkb->GetParent(ArgPos::kFirst, SynToPkbType(first));
    return {first, std::move(result)};
}
ResultTable ParentBase::WildNum(KnowledgeBase* pkb,
                                StmtNo second) const noexcept {
    auto result = pkb->ExistParent(Index<ArgPos::kSecond>(second));
    return ResultTable(result);
}
ResultTable ParentBase::WildSyn(KnowledgeBase* pkb,
                                Synonym* second) const noexcept {
    auto result = pkb->GetParent(ArgPos::kSecond, SynToPkbType(second));
    return {second, std::move(result)};
}
ResultTable ParentBase::WildWild(KnowledgeBase* pkb) const noexcept {
    return ResultTable(pkb->ExistParent());
}
}  // namespace spa
