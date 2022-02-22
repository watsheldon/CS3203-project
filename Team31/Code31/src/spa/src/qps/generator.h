#ifndef SPA_SRC_SPA_SRC_QPS_GENERATOR_H_
#define SPA_SRC_SPA_SRC_QPS_GENERATOR_H_

#include <vector>
#include <memory>

#include "PQL_validator.h"
#include "ent_ref.h"
#include "query_object.h"
#include "query_object_builder.h"
#include "query_token.h"
#include "stmt_ref.h"
#include "synonym.h"

namespace spa {

enum class Mode {
    kDeclaration,
    kSelect,
    kParent,
    kFollows,
    kUses,
    kModifies,
    kPattern,
    kExpression,

};
class Generator {
    // class generator does the semantic checks on queries
    // Undeclared synonyms
    // Ambiguous wildcard
    // Declaration of repeated synonyms

  public:
    static std::unique_ptr<QueryObject> Generate(
            const std::vector<QueryToken>& tokens);
    //    bool GenerateDeclarations();
    //    bool GenerateSelect();
    //    bool GenerateSuchThat();
    static DeclarationType TypeConvert(QueryTokenType type);
};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_QPS_GENERATOR_H_
