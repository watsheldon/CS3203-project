#ifndef SRC_SPA_SRC_SOURCE_AST_ABSTRACT_SYNTAX_TREE_H_
#define SRC_SPA_SRC_SOURCE_AST_ABSTRACT_SYNTAX_TREE_H_

#include <memory>
#include <vector>
#include <set>

#include "pkb/knowledge_base.h"
#include "source/token.h"

namespace spa {
class AbstractSyntaxTree {
  public:
    using SharedVecToken = std::shared_ptr<std::vector<Token>>;
    explicit AbstractSyntaxTree(SharedVecToken tokens);
    [[nodiscard]] std::shared_ptr<BasicEntities> getInitEntities() const;
  private:
    SharedVecToken tokens_;
    std::shared_ptr<BasicEntities> basic_entities_;
    std::set<std::string> variables_;
    std::set<std::string> procedures_;
    std::set<std::string> constants_;

    void AddProcedure(const std::string &name);
    void AddVariable(const std::string &name);
    void AddConstant(const std::string &value);
};
}

#endif //SRC_SPA_SRC_SOURCE_AST_ABSTRACT_SYNTAX_TREE_H_
