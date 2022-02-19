#ifndef SRC_SPA_SRC_SOURCE_AST_ABSTRACT_SYNTAX_TREE_H_
#define SRC_SPA_SRC_SOURCE_AST_ABSTRACT_SYNTAX_TREE_H_

#include <algorithm>
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
    template <typename T>
    using UniquePtrVec = std::vector<std::unique_ptr<T>>;
    explicit AbstractSyntaxTree(UniquePtrTokens tokens);
    [[nodiscard]] BasicEntities getInitEntities() const;
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
    NamePtrMap<VariableNode> variables_;
    NamePtrMap<ProcedureNode> procedures_;
    NamePtrMap<ConstantNode> constants_;
    UniquePtrVec<ReadNode> read_stmts_;
    UniquePtrVec<PrintNode> print_stmts_;
    UniquePtrVec<CallNode> call_stmts_;
    UniquePtrVec<AssignNode> assign_stmts_;
    UniquePtrVec<IfNode> if_stmts_;
    UniquePtrVec<WhileNode> while_stmts_;
    UniquePtrVec<StmtLstNode> stmt_lsts_;
    UniquePtrVec<PolishNotation> expressions_;
    UniquePtrVec<ConditionNode> conditions_;
    std::unique_ptr<ProgramNode> root_;
    std::vector<std::set<int>> call_edges_;
    bool tree_valid_ = false;

    void build_tree();
    ProcedureNode *CreateProcedure(std::string name);
    void check_tree();
    bool Cyclic(int curr, std::vector<bool> visited);
    template <typename T,
              typename = std::enable_if_t<std::is_base_of_v<StatementNode, T>>>
    static void ExtractStmtIndex(const UniquePtrVec<T> &src,
                                 std::vector<int> &dst) {
        dst.reserve(src.size() + 1);
        dst.emplace_back();
        std::for_each(src.begin(), src.end(),
                      [&dst](auto &p) { dst.emplace_back(p->GetIndex()); });
    }
    template <typename T>
    static void ExtractNames(const NamePtrMap<T> &src,
                             std::vector<std::string> &dst) {
        dst.resize(src.size() + 1);
        std::for_each(src.begin(), src.end(), [&dst](auto &pair) {
            dst[pair.second->GetIndex()] = pair.first;
        });
    }
};
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_AST_ABSTRACT_SYNTAX_TREE_H_
