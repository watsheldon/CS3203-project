#ifndef SRC_SPA_SRC_SOURCE_AST_ABSTRACT_SYNTAX_TREE_H_
#define SRC_SPA_SRC_SOURCE_AST_ABSTRACT_SYNTAX_TREE_H_

#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <stack>
#include <string>
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
    template <typename T, typename = std::enable_if_t<
                                  std::is_base_of_v<AbstractSyntaxTreeNode, T>>>
    using NamePtrMap = std::map<std::string, std::unique_ptr<T>>;
    template <typename T>
    using UniquePtrVec = std::vector<std::unique_ptr<T>>;
    explicit AbstractSyntaxTree(std::vector<Token> tokens) noexcept;
    [[nodiscard]] BasicEntities GetInitEntities() const noexcept;
    [[nodiscard]] const ProgramNode *GetRoot() const noexcept;

  private:
    enum class Mode {
        kProc = 1,
        kRead,
        kPrint,
        kCall,
        kWhile,
        kIf,
        kThen,
        kElse,
        kCond,
        kAssign,
        kStmtLst
    };
    std::vector<Token> tokens_;
    std::stack<Mode> mode_history_;
    std::deque<StmtLstParent *> parent_path_;
    std::stack<StmtLstNode *> stmt_lst_path_;
    std::vector<PolishNotationNode> curr_expr_;
    int cond_depth_ = 0;

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

    const VariableNode *AddVariable(std::string name) noexcept;
    const ConstantNode *AddConstant(std::string name) noexcept;
    void BracketL() noexcept;
    void BracketR() noexcept;
    void BraceL() noexcept;
    void BraceR() noexcept;
    void Semicolon() noexcept;
    void Name(const std::string &name) noexcept;
    void Constant(std::string name) noexcept;
    void AddProcedure(std::string name) noexcept;
    void AddRead(std::string name) noexcept;
    void AddPrint(std::string name) noexcept;
    void AddCall(std::string name) noexcept;
    void AddAssign(std::string name) noexcept;
    void LastCondAddVar(std::string name) noexcept;
    void ExprAddVar(std::string name) noexcept;
    void ParseToken(const Token &token) noexcept;
    void BuildTree() noexcept;
    void CheckTree() noexcept;
    bool HasCycle(int curr, std::vector<bool> visited) noexcept;
    template <typename T,
              typename = std::enable_if_t<std::is_base_of_v<StatementNode, T>>>
    static void ExtractStmtIndex(const UniquePtrVec<T> &src,
                                 std::vector<int> &dst) noexcept {
        dst.reserve(src.size());
        std::for_each(src.begin(), src.end(),
                      [&dst](auto &p) { dst.emplace_back(p->GetIndex()); });
    }
    template <typename T>
    static void ExtractNames(const NamePtrMap<T> &src,
                             std::vector<std::string> &dst) noexcept {
        dst.resize(src.size() + 1);
        std::for_each(src.begin(), src.end(), [&dst](auto &pair) {
            dst[pair.second->GetIndex()] = pair.first;
        });
    }
    static constexpr Mode KeywordToMode(SourceTokenType keyword) noexcept;
    static constexpr OperatorType OperatorForRpn(
            SourceTokenType keyword) noexcept;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_AST_ABSTRACT_SYNTAX_TREE_H_
