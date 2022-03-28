#include "next_base.h"

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "common/type_convert_helpers.h"
#include "pkb/knowledge_base.h"
#include "qps/synonym.h"

namespace spa {
ResultTable NextBase::NumWild(KnowledgeBase *pkb, StmtNo first) const noexcept {
    auto result = pkb->ExistNext(Index<ArgPos::kFirst>(first));
    return ResultTable(result);
}
ResultTable NextBase::WildNum(KnowledgeBase *pkb,
                              StmtNo second) const noexcept {
    auto result = pkb->ExistNext(Index<ArgPos::kSecond>(second));
    return ResultTable(result);
}
ResultTable NextBase::WildWild(KnowledgeBase *pkb) const noexcept {
    return ResultTable(pkb->ExistNext());
}
ResultTable NextBase::SynWild(KnowledgeBase *pkb,
                              Synonym *first) const noexcept {
    auto result = pkb->GetNext(ArgPos::kFirst, SynToPkbType(first));
    return {first, std::move(result)};
}
ResultTable NextBase::WildSyn(KnowledgeBase *pkb,
                              Synonym *second) const noexcept {
    auto result = pkb->GetNext(ArgPos::kSecond, SynToPkbType(second));
    return {second, std::move(result)};
}
}  // namespace spa
