#include "parent_clause.h"

#include <utility>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/type_convert_helpers.h"

namespace spa {
ParentIntInt::ParentIntInt(int first, int second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
ResultTable ParentIntInt::Execute(KnowledgeBase *knowledge_base) const {
    auto result = knowledge_base->ExistParent(transitive_,
                                              Index<ArgPos::kFirst>(first_),
                                              Index<ArgPos::kSecond>(second_));
    return ResultTable(result);
}
ParentIntSyn::ParentIntSyn(int first, Synonym *second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
ResultTable ParentIntSyn::Execute(KnowledgeBase *knowledge_base) const {
    auto result = knowledge_base->GetParent(
            transitive_, Index<ArgPos::kFirst>(first_), SynToPkbType(second_));
    return {second_, std::move(result)};
}
ParentIntWild::ParentIntWild(int first) : first_(first) {}
ResultTable ParentIntWild::Execute(KnowledgeBase *knowledge_base) const {
    auto result = knowledge_base->ExistParent(Index<ArgPos::kFirst>(first_));
    return ResultTable(result);
}
ParentSynInt::ParentSynInt(Synonym *first, int second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
ResultTable ParentSynInt::Execute(KnowledgeBase *knowledge_base) const {
    auto result = knowledge_base->GetParent(
            transitive_, Index<ArgPos::kSecond>(second_), SynToPkbType(first_));
    return {first_, std::move(result)};
}
ParentSynSyn::ParentSynSyn(Synonym *first, Synonym *second, bool transitive)
        : first_(first), second_(second), transitive_(transitive) {}
ResultTable ParentSynSyn::Execute(KnowledgeBase *knowledge_base) const {
    auto [col_1, col_2] = knowledge_base->GetParentPairs(
            transitive_, SynToPkbType(first_), SynToPkbType(second_));
    return {first_, std::move(col_1), second_, std::move(col_2)};
}
ParentSynWild::ParentSynWild(Synonym *first) : first_(first) {}
ResultTable ParentSynWild::Execute(KnowledgeBase *knowledge_base) const {
    auto result =
            knowledge_base->GetParent(ArgPos::kFirst, SynToPkbType(first_));
    return {first_, std::move(result)};
}
ParentWildInt::ParentWildInt(int second) : second_(second) {}
ResultTable ParentWildInt::Execute(KnowledgeBase *knowledge_base) const {
    auto result = knowledge_base->ExistParent(Index<ArgPos::kSecond>(second_));
    return ResultTable(result);
}
ParentWildSyn::ParentWildSyn(Synonym *second) : second_(second) {}
ResultTable ParentWildSyn::Execute(KnowledgeBase *knowledge_base) const {
    auto result =
            knowledge_base->GetParent(ArgPos::kSecond, SynToPkbType(second_));
    return {second_, std::move(result)};
}
ParentWildWild::ParentWildWild() = default;
ResultTable ParentWildWild::Execute(KnowledgeBase *knowledge_base) const {
    auto result = knowledge_base->ExistParent();
    return ResultTable(result);
}
}  // namespace spa
