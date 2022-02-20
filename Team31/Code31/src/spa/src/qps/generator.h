#ifndef SPA_SRC_SPA_SRC_QPS_GENERATOR_H_
#define SPA_SRC_SPA_SRC_QPS_GENERATOR_H_

#include <filesystem>
#include <map>
#include <memory>
#include <vector>

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
    kCondition,
    kExpression,

};
class Generator {
    // class generator does the semantic checks on queries
    // Undeclared synonyms
    // Ambiguous wildcard
    // Declaration of repeated synonyms

  private:
    PQLValidator validator_;
    std::shared_ptr<std::vector<QueryToken>> tokens;
    std::map<std::string, std::unique_ptr<Synonym>> map;

  public:
    Generator(std::filesystem::path &filePath);
    QueryObject Generate();
    bool GenerateDeclarations();
    bool GenerateSelect();
    bool GenerateSuchThat();
    DeclarationType TypeConvert(QueryTokenType type);
};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_QPS_GENERATOR_H_
