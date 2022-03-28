#include "affects_base.h"

#include "common/entity_type_enum.h"
#include "pkb/knowledge_base.h"
#include "qps/synonym.h"

namespace spa {
ResultTable AffectsBase::SynWild(KnowledgeBase *pkb,
                                 Synonym *first) const noexcept {
    auto result = pkb->GetAffects(ArgPos::kFirst);
    return {first, std::move(result)};
}
ResultTable AffectsBase::WildSyn(KnowledgeBase *pkb,
                                 Synonym *second) const noexcept {
    auto result = pkb->GetAffects(ArgPos::kSecond);
    return {second, std::move(result)};
}
ResultTable AffectsBase::NumWild(KnowledgeBase *pkb,
                                 StmtNo first) const noexcept {
    auto result = pkb->ExistAffects(Index<ArgPos::kFirst>(first));
    return ResultTable(result);
}
ResultTable AffectsBase::WildNum(KnowledgeBase *pkb,
                                 StmtNo second) const noexcept {
    auto result = pkb->ExistAffects(Index<ArgPos::kSecond>(second));
    return ResultTable(result);
}
ResultTable AffectsBase::WildWild(KnowledgeBase *pkb) const noexcept {
    return ResultTable(pkb->ExistAffects());
}
}  // namespace spa
