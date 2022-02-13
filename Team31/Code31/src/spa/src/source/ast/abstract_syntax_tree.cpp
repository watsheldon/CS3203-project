#include "abstract_syntax_tree.h"

#include <cassert>
#include <deque>
#include <iterator>
#include <list>
#include <memory>
#include <stack>
#include <utility>

#include "common/lazy_factory.h"
#include "common/polish_notation_node.h"
#include "condition_node.h"
#include "constant_node.h"
#include "pkb/knowledge_base.h"
#include "procedure_node.h"
#include "program_node.h"
#include "source/token.h"
#include "statement_node.h"
#include "stmt_lst_node.h"
#include "stmt_lst_parent.h"
#include "variable_node.h"

namespace spa {
AbstractSyntaxTree::AbstractSyntaxTree(UniquePtrTokens tokens)
        : tokens_(std::move(tokens)),
          entities_(std::make_shared<BasicEntities>()) {
    build_tree();
    check_tree();
}
std::shared_ptr<BasicEntities> AbstractSyntaxTree::getInitEntities() const {
    if (!tree_valid_) return nullptr;
    entities_->procedures.resize(procedures_.size() + 1);
    for (auto &[name, ptr] : procedures_) {
        entities_->procedures[ptr->GetIndex()] = name;
    }
    entities_->variables.resize(variables_.size() + 1);
    for (auto &[name, ptr] : variables_) {
        entities_->variables[ptr->GetIndex()] = name;
    }
    entities_->constants.resize(constants_.size() + 1);
    for (auto &[name, ptr] : constants_) {
        entities_->constants[ptr->GetIndex()] = name;
    }
    entities_->notations.emplace_back(std::list<PolishNotationNode>());
    entities_->notations.reserve(expressions_.size() + 1);
    std::transform(expressions_.begin(), expressions_.end(),
                   std::back_inserter(entities_->notations),
                   [](const auto &p) { return *p; });

    return entities_;
}
void AbstractSyntaxTree::build_tree() {
    root_ = std::make_unique<ProgramNode>();
    tree_valid_ = true;
    std::stack<Mode> mode_history;
    std::deque<StmtLstParent *> parent_path;
    std::stack<StmtLstNode *> stmt_lst_path;
    std::list<PolishNotationNode> curr_expr;
    int cond_depth = 0;
    for (const auto &[type, name] : *tokens_) {
        if (!tree_valid_) break;
        switch (type) {
            case SourceTokenType::kKeywordProcedure:
                mode_history.emplace(Mode::kProc);
                break;
            case SourceTokenType::kKeywordRead:
                mode_history.emplace(Mode::kRead);
                break;
            case SourceTokenType::kKeywordPrint:
                mode_history.emplace(Mode::kPrint);
                break;
            case SourceTokenType::kKeywordCall:
                mode_history.emplace(Mode::kCall);
                break;
            case SourceTokenType::kKeywordWhile:
                mode_history.emplace(Mode::kWhile);
                break;
            case SourceTokenType::kKeywordIf:
                mode_history.emplace(Mode::kIf);
                break;
            case SourceTokenType::kKeywordThen:
                mode_history.emplace(Mode::kThen);
                break;
            case SourceTokenType::kKeywordElse:
                mode_history.emplace(Mode::kElse);
                break;
            case SourceTokenType::kBracketL:
                if (mode_history.top() == Mode::kAssign) {
                    curr_expr.emplace_back(ExprNodeType::kBracketL);
                } else if (mode_history.top() == Mode::kWhile ||
                           mode_history.top() == Mode::kIf) {
                    mode_history.emplace(Mode::kCond);
                    conditions_.emplace_back(std::make_unique<ConditionNode>());
                    ++cond_depth;
                } else if (mode_history.top() == Mode::kCond) {
                    ++cond_depth;
                } else {
                    assert(false);
                }
                break;
            case SourceTokenType::kBracketR:
                if (mode_history.top() == Mode::kAssign) {
                    curr_expr.emplace_back(ExprNodeType::kBracketR);
                } else if (mode_history.top() == Mode::kCond) {
                    if (--cond_depth != 0) break;
                    mode_history.pop();
                    if (mode_history.top() == Mode::kWhile) {
                        auto while_stmt = std::make_unique<WhileNode>();
                        while_stmt->SetCondition(conditions_.back().get());
                        parent_path.emplace_back(while_stmt.get());
                        entities_->whiles.emplace_back(while_stmt->GetIndex());
                        statements_.emplace_back(std::move(while_stmt));
                    } else if (mode_history.top() == Mode::kIf) {
                        auto if_stmt = std::make_unique<IfNode>();
                        if_stmt->SetCondition(conditions_.back().get());
                        parent_path.emplace_back(if_stmt.get());
                        entities_->ifs.emplace_back(if_stmt->GetIndex());
                        statements_.emplace_back(std::move(if_stmt));
                        mode_history.pop();
                    } else {
                        assert(false);
                    }
                } else {
                    assert(false);
                }
                break;
            case SourceTokenType::kBraceL: {
                mode_history.emplace(Mode::kStmtLst);
                auto &stmt_lst = stmt_lsts_.emplace_back(
                        std::make_unique<StmtLstNode>());
                stmt_lst_path.emplace(stmt_lst.get());
                break;
            }
            case SourceTokenType::kBraceR: {
                assert(mode_history.top() == Mode::kStmtLst);
                mode_history.pop();
                parent_path.back()->AddStmtLst(stmt_lst_path.top());
                stmt_lst_path.pop();
                if (mode_history.top() == Mode::kWhile) {
                    auto while_stmt =
                            dynamic_cast<const WhileNode *>(parent_path.back());
                    assert(while_stmt);
                    stmt_lst_path.top()->AddStatement(while_stmt);
                    parent_path.pop_back();
                } else if (mode_history.top() == Mode::kThen) {
                    auto if_stmt = dynamic_cast<IfNode *>(parent_path.back());
                    assert(if_stmt);
                } else if (mode_history.top() == Mode::kElse) {
                    auto if_stmt = dynamic_cast<IfNode *>(parent_path.back());
                    assert(if_stmt);
                    stmt_lst_path.top()->AddStatement(if_stmt);
                    parent_path.pop_back();
                } else if (mode_history.top() == Mode::kProc) {
                    auto proc =
                            dynamic_cast<ProcedureNode *>(parent_path.back());
                    assert(proc);
                    root_->AddProcedure(proc);
                    parent_path.pop_back();
                } else {
                    assert(false);
                }
                mode_history.pop();
                break;
            }
            case SourceTokenType::kAssignEqual:
                assert(mode_history.top() == Mode::kAssign);
                curr_expr.clear();
                break;
            case SourceTokenType::kOperatorPlus: {
                if (mode_history.top() == Mode::kAssign) {
                    curr_expr.emplace_back(OperatorType::kPlus);
                } else if (mode_history.top() == Mode::kCond) {
                    // ignore
                } else {
                    assert(false);
                }
                break;
            }
            case SourceTokenType::kOperatorMinus: {
                if (mode_history.top() == Mode::kAssign) {
                    curr_expr.emplace_back(OperatorType::kMinus);
                } else if (mode_history.top() == Mode::kCond) {
                    // ignore
                } else {
                    assert(false);
                }
                break;
            }
            case SourceTokenType::kOperatorTimes: {
                if (mode_history.top() == Mode::kAssign) {
                    curr_expr.emplace_back(OperatorType::kTimes);
                } else if (mode_history.top() == Mode::kCond) {
                    // ignore
                } else {
                    assert(false);
                }
                break;
            }
            case SourceTokenType::kOperatorDivide: {
                if (mode_history.top() == Mode::kAssign) {
                    curr_expr.emplace_back(OperatorType::kDivide);
                } else if (mode_history.top() == Mode::kCond) {
                    // ignore
                } else {
                    assert(false);
                }
                break;
            }
            case SourceTokenType::kOperatorModulo: {
                if (mode_history.top() == Mode::kAssign) {
                    curr_expr.emplace_back(OperatorType::kModulo);
                } else if (mode_history.top() == Mode::kCond) {
                    // ignore
                } else {
                    assert(false);
                }
                break;
            }
            case SourceTokenType::kCondNot:
            case SourceTokenType::kCondAnd:
            case SourceTokenType::kCondOr:
            case SourceTokenType::kRelLt:
            case SourceTokenType::kRelLeq:
            case SourceTokenType::kRelEq:
            case SourceTokenType::kRelNeq:
            case SourceTokenType::kRelGt:
            case SourceTokenType::kRelGeq:
                assert(mode_history.top() == Mode::kCond);
                break;
            case SourceTokenType::kSemicolon:
                if (mode_history.top() == Mode::kRead ||
                    mode_history.top() == Mode::kPrint ||
                    mode_history.top() == Mode::kCall) {
                    stmt_lst_path.top()->AddStatement(statements_.back().get());
                } else if (mode_history.top() == Mode::kAssign) {
                    auto expr = std::make_unique<PolishNotation>(
                            std::move(curr_expr));
                    auto assign = dynamic_cast<AssignNode *>(
                            statements_.back().get());
                    assert(assign);
                    assign->SetExpr(expr.get());
                    stmt_lst_path.top()->AddStatement(assign);
                    expressions_.emplace_back(std::move(expr));
                } else {
                    assert(false);
                }
                mode_history.pop();
                break;
            case SourceTokenType::kName: {
                if (mode_history.top() == Mode::kProc) {
                    auto ptr = CreateProcedure(name);
                    parent_path.emplace_back(ptr);
                } else if (mode_history.top() == Mode::kRead) {
                    auto [pair, inserted] = variables_.try_emplace(
                            name, LazyFactory([] {
                                return std::make_unique<VariableNode>();
                            }));
                    auto read = std::make_unique<ReadNode>();
                    read->SetVariable(pair->second.get());
                    entities_->reads.emplace_back(read->GetIndex());
                    statements_.emplace_back(std::move(read));
                } else if (mode_history.top() == Mode::kPrint) {
                    auto [pair, success] = variables_.try_emplace(
                            name, LazyFactory([] {
                                return std::make_unique<VariableNode>();
                            }));
                    auto print = std::make_unique<PrintNode>();
                    print->SetVariable(pair->second.get());
                    entities_->prints.emplace_back(print->GetIndex());
                    statements_.emplace_back(std::move(print));
                } else if (mode_history.top() == Mode::kCall) {
                    auto [pair, inserted] = procedures_.try_emplace(
                            name, LazyFactory([] {
                                return std::make_unique<ProcedureNode>();
                            }));
                    if (inserted) call_edges_.resize(procedures_.size() + 1);
                    auto call = std::make_unique<CallNode>();
                    auto callee = pair->second.get();
                    call->SetProcedure(callee);
                    entities_->calls.emplace_back(call->GetIndex());

                    // update the call graph
                    auto caller =
                            dynamic_cast<ProcedureNode *>(parent_path.front());
                    assert(caller);
                    call_edges_[caller->GetIndex()].emplace(callee->GetIndex());

                    statements_.emplace_back(std::move(call));
                } else if (mode_history.top() == Mode::kCond) {
                    auto [pair, success] = variables_.try_emplace(
                            name, LazyFactory([] {
                                return std::make_unique<VariableNode>();
                            }));
                    auto cond = conditions_.back().get();
                    cond->AddVariable(pair->second.get());
                } else {  // has to be assignment
                    auto [pair, success] = variables_.try_emplace(
                            name, LazyFactory([] {
                                return std::make_unique<VariableNode>();
                            }));
                    auto var = pair->second.get();
                    if (mode_history.top() != Mode::kAssign) {
                        auto assign = std::make_unique<AssignNode>();
                        assign->SetModifiedVar(var);
                        entities_->assigns.emplace_back(assign->GetIndex());
                        statements_.emplace_back(std::move(assign));
                        mode_history.emplace(Mode::kAssign);
                        break;
                    }
                    assert(mode_history.top() == Mode::kAssign);
                    curr_expr.emplace_back(ExprNodeType::kVariable,
                                           var->GetIndex());
                }
            } break;
            case SourceTokenType::kInteger:
                if (mode_history.top() == Mode::kAssign) {
                    auto [pair, success] = constants_.try_emplace(
                            name, LazyFactory([] {
                                return std::make_unique<ConstantNode>();
                            }));
                    auto constant = pair->second.get();
                    curr_expr.emplace_back(ExprNodeType::kConstant,
                                           constant->GetIndex());
                } else if (mode_history.top() == Mode::kCond) {
                    auto [pair, success] = constants_.try_emplace(
                            name, LazyFactory([] {
                                return std::make_unique<ConstantNode>();
                            }));
                    auto cond = conditions_.back().get();
                    cond->AddConstant(pair->second.get());
                } else {
                    assert(false);
                }
                break;
        }
    }
}
ProcedureNode *AbstractSyntaxTree::CreateProcedure(std::string name) {
    auto [pair, inserted] = procedures_.try_emplace(
            std::move(name),
            LazyFactory([] { return std::make_unique<ProcedureNode>(); }));
    auto proc = pair->second.get();
    // A program cannot have two procedures with the same name.
    if (!inserted && proc->GetStmtlst()) {
        tree_valid_ = false;
        return nullptr;
    }
    call_edges_.resize(procedures_.size() + 1);
    return proc;
}
const ProgramNode *AbstractSyntaxTree::GetRoot() const {
    return tree_valid_ ? root_.get() : nullptr;
}
void AbstractSyntaxTree::check_tree() {
    if (!tree_valid_) return;
    // A procedure cannot call a non-existing procedure.
    if (procedures_.size() != root_->GetProcedures().size()) {
        tree_valid_ = false;
        return;
    }
    // Recursive and cyclic calls are not allowed.
    for (int i = 1; i < call_edges_.size(); ++i) {
        std::vector<bool> visited(call_edges_.size());
        if (Cyclic(i, visited)) {
            tree_valid_ = false;
            break;
        }
    }
}
bool AbstractSyntaxTree::Cyclic(int curr, std::vector<bool> visited) {
    visited[curr] = true;
    for (auto i : call_edges_[curr]) {
        if (visited[i] || Cyclic(i, visited)) {
            tree_valid_ = false;
            return true;
        }
    }
    return false;
}
}  // namespace spa
