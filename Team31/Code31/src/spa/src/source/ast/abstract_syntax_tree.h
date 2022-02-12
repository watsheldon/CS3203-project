#ifndef SRC_SPA_SRC_SOURCE_AST_ABSTRACT_SYNTAX_TREE_H_
#define SRC_SPA_SRC_SOURCE_AST_ABSTRACT_SYNTAX_TREE_H_

#include <memory>
#include <set>
#include <vector>

#include "pkb/knowledge_base.h"
#include "source/token.h"

namespace spa {
class AbstractSyntaxTree {
  public:
    using UniquePtrTokens = std::unique_ptr<std::vector<Token>>;
    template <typename T, typename = std::enable_if_t<
                                  std::is_base_of_v<AbstractSyntaxTreeNode, T>>>
    using NamePtrMap = std::map<std::string, std::unique_ptr<T>>;
    explicit AbstractSyntaxTree(UniquePtrTokens tokens);
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
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_AST_ABSTRACT_SYNTAX_TREE_H_
