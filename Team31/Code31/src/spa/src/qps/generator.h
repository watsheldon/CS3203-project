#ifndef SPA_SRC_SPA_SRC_QPS_GENERATOR_H_
#define SPA_SRC_SPA_SRC_QPS_GENERATOR_H_

#include <memory>
#include <vector>

#include "PQL_validator.h"
#include "query_object.h"
#include "query_token.h"
#include "synonym.h"

namespace spa {

class Generator {
    using SynonymMap = std::map<std::string_view, Synonym* const>;
    using VecTokens = std::vector<QueryToken>;

  public:
    std::unique_ptr<QueryObject> Generate(const VecTokens& tokens);

  private:
    enum class Mode {
        kDeclaration = 0,
        kSelect,
        kParent,
        kFollows,
        kUses,
        kModifies,
        kPattern,
        kExpression,
        kIdentifier,
        kZero,
        kFirst,
        kSecond,
    };
    static constexpr Synonym::Type TokenToSynType(QueryTokenType type);
    static constexpr Mode TokenToClauseMode(QueryTokenType type);
    static constexpr bool UnsuitableFirstSynType(Mode mode, Synonym::Type type);
    static constexpr bool UnsuitableSecondSynType(Mode mode,
                                                  Synonym::Type type);
};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_QPS_GENERATOR_H_
