#ifndef SRC_SPA_SRC_SOURCE_AST_ABSTRACT_SYNTAX_TREE_H_
#define SRC_SPA_SRC_SOURCE_AST_ABSTRACT_SYNTAX_TREE_H_

#include <map>
#include <memory>
#include <set>
#include <stack>
#include <vector>

#include "abstract_syntax_tree_node.h"
#include "common/polish_notation.h"
#include "condition_node.h"
#include "constant_node.h"
#include "pkb/knowledge_base.h"
#include "procedure_node.h"
#include "program_node.h"
#include "source/token.h"
#include "statement_node.h"
#include "stmt_lst_node.h"
#include "variable_node.h"

namespace spa {
class AbstractSyntaxTree {
  public:
    using UniquePtrTokens = std::unique_ptr<std::vector<Token>>;
    template <typename T, typename = std::enable_if_t<
                                  std::is_base_of_v<AbstractSyntaxTreeNode, T>>>
    using NamePtrMap = std::map<std::string, std::unique_ptr<T>>;
    explicit AbstractSyntaxTree(UniquePtrTokens tokens);
    [[nodiscard]] std::shared_ptr<BasicEntities> getInitEntities() const;
    [[nodiscard]] const ProgramNode *GetRoot() const;

  private:
    enum class Mode {
        kProc,
        kStmtLst,
        kRead,
        kPrint,
        kCall,
        kCond,
        kWhile,
        kIf,
        kThen,
        kElse,
        kAssign,
    };
    UniquePtrTokens tokens_;
    std::shared_ptr<BasicEntities> entities_;
    NamePtrMap<VariableNode> variables_;
    NamePtrMap<ProcedureNode> procedures_;
    NamePtrMap<ConstantNode> constants_;
    std::vector<std::unique_ptr<StatementNode>> statements_;
    std::vector<std::unique_ptr<StmtLstNode>> stmt_lsts_;
    std::vector<std::unique_ptr<PolishNotation>> expressions_;
    std::vector<std::unique_ptr<ConditionNode>> conditions_;
    std::unique_ptr<ProgramNode> root_;
    std::vector<std::set<int>> call_edges_;
    bool tree_valid_ = false;

    void build_tree();
    ProcedureNode *CreateProcedure(std::string name);
    void check_tree();
    bool Cyclic(int curr, std::vector<bool> visited);
};
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_AST_ABSTRACT_SYNTAX_TREE_H_
