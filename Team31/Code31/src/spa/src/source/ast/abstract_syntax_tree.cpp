#include "abstract_syntax_tree.h"

#include <cassert>
#include <iterator>
#include <memory>
#include <string>
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
#include "variable_node.h"

namespace spa {
AbstractSyntaxTree::AbstractSyntaxTree(std::vector<Token> tokens)
        : tokens_(std::move(tokens)) {
    BuildTree();
    CheckTree();
}
BasicEntities AbstractSyntaxTree::GetInitEntities() const {
    if (!tree_valid_) return {};
    BasicEntities entities;
    ExtractStmtIndex(read_stmts_, entities.reads);
    ExtractStmtIndex(print_stmts_, entities.prints);
    ExtractStmtIndex(call_stmts_, entities.calls);
    ExtractStmtIndex(assign_stmts_, entities.assigns);
    ExtractStmtIndex(if_stmts_, entities.ifs);
    ExtractStmtIndex(while_stmts_, entities.whiles);
    ExtractNames(procedures_, entities.procedures);
    ExtractNames(variables_, entities.variables);
    ExtractNames(constants_, entities.constants);
    entities.notations.emplace_back(std::vector<PolishNotationNode>());
    entities.notations.reserve(expressions_.size() + 1);
    std::for_each(
            expressions_.begin(), expressions_.end(),
            [&entities](auto &p) { entities.notations.emplace_back(*p); });
    entities.proc_call_graph = call_edges_;
    return std::move(entities);
}
const ProgramNode *AbstractSyntaxTree::GetRoot() const {
    return tree_valid_ ? root_.get() : nullptr;
}
const VariableNode *AbstractSyntaxTree::AddVariable(std::string name) {
    auto [pair, ins] = variables_.try_emplace(
            std::move(name),
            LazyFactory([] { return std::make_unique<VariableNode>(); }));
    return pair->second.get();
}
const ConstantNode *AbstractSyntaxTree::AddConstant(std::string name) {
    auto [pair, ins] = constants_.try_emplace(
            std::move(name),
            LazyFactory([] { return std::make_unique<ConstantNode>(); }));
    return pair->second.get();
}
void AbstractSyntaxTree::BracketL() {
    if (mode_history_.top() == Mode::kAssign) {
        curr_expr_.emplace_back(ExprNodeType::kBracketL);
    } else if (mode_history_.top() == Mode::kWhile ||
               mode_history_.top() == Mode::kIf) {
        mode_history_.emplace(Mode::kCond);
        conditions_.emplace_back(std::make_unique<ConditionNode>());
        ++cond_depth_;
    } else if (mode_history_.top() == Mode::kCond) {
        ++cond_depth_;
    } else {
        assert(false);
    }
}
void AbstractSyntaxTree::BracketR() {
    if (mode_history_.top() == Mode::kAssign) {
        curr_expr_.emplace_back(ExprNodeType::kBracketR);
        return;
    }
    assert(mode_history_.top() == Mode::kCond);
    if (--cond_depth_ != 0) return;
    mode_history_.pop();
    if (mode_history_.top() == Mode::kWhile) {
        auto while_stmt = std::make_unique<WhileNode>();
        while_stmt->SetCondition(conditions_.back().get());
        parent_path_.emplace_back(while_stmt.get());
        while_stmts_.emplace_back(std::move(while_stmt));
        return;
    }
    assert(mode_history_.top() == Mode::kIf);
    auto if_stmt = std::make_unique<IfNode>();
    if_stmt->SetCondition(conditions_.back().get());
    parent_path_.emplace_back(if_stmt.get());
    if_stmts_.emplace_back(std::move(if_stmt));
    mode_history_.pop();
}
void AbstractSyntaxTree::BraceL() {
    mode_history_.emplace(Mode::kStmtLst);
    auto &stmt_lst = stmt_lsts_.emplace_back(std::make_unique<StmtLstNode>());
    stmt_lst_path_.emplace(stmt_lst.get());
}
void AbstractSyntaxTree::BraceR() {
    assert(mode_history_.top() == Mode::kStmtLst);
    mode_history_.pop();
    parent_path_.back()->AddStmtLst(stmt_lst_path_.top());
    stmt_lst_path_.pop();
    switch (mode_history_.top()) {
        case Mode::kWhile: {
            auto while_stmt =
                    dynamic_cast<const WhileNode *>(parent_path_.back());
            assert(while_stmt);
            stmt_lst_path_.top()->AddStatement(while_stmt);
            parent_path_.pop_back();
            break;
        }
        case Mode::kThen: {
            auto if_stmt = dynamic_cast<IfNode *>(parent_path_.back());
            assert(if_stmt);
            break;
        }
        case Mode::kElse: {
            auto if_stmt = dynamic_cast<IfNode *>(parent_path_.back());
            assert(if_stmt);
            stmt_lst_path_.top()->AddStatement(if_stmt);
            parent_path_.pop_back();
            break;
        }
        case Mode::kProc: {
            auto proc = dynamic_cast<ProcedureNode *>(parent_path_.back());
            assert(proc);
            root_->AddProcedure(proc);
            parent_path_.pop_back();
            break;
        }
        default:
            assert(false);
    }
    mode_history_.pop();
}
void AbstractSyntaxTree::Semicolon() {
    switch (mode_history_.top()) {
        case Mode::kRead:
            stmt_lst_path_.top()->AddStatement(read_stmts_.back().get());
            break;
        case Mode::kPrint:
            stmt_lst_path_.top()->AddStatement(print_stmts_.back().get());
            break;
        case Mode::kCall:
            stmt_lst_path_.top()->AddStatement(call_stmts_.back().get());
            break;
        case Mode::kAssign: {
            auto expr = std::make_unique<PolishNotation>(std::move(curr_expr_));
            auto assign = assign_stmts_.back().get();
            assign->SetExpr(expr.get());
            stmt_lst_path_.top()->AddStatement(assign);
            expressions_.emplace_back(std::move(expr));
            assign->SetRhsIndex((int)expressions_.size());
            break;
        }
        default:
            assert(false);
    }
    mode_history_.pop();
}
void AbstractSyntaxTree::Name(const std::string &name) {
    switch (mode_history_.top()) {
        case Mode::kStmtLst:
            AddAssign(name);
            break;
        case Mode::kProc:
            AddProcedure(name);
            break;
        case Mode::kRead:
            AddRead(name);
            break;
        case Mode::kPrint:
            AddPrint(name);
            break;
        case Mode::kCall:
            AddCall(name);
            break;
        case Mode::kCond:
            LastCondAddVar(name);
            break;
        case Mode::kAssign:
            ExprAddVar(name);
            break;
        default:
            assert(false);
    }
}
void AbstractSyntaxTree::Constant(std::string name) {
    auto constant = AddConstant(std::move(name));
    if (mode_history_.top() == Mode::kAssign) {
        curr_expr_.emplace_back(ExprNodeType::kConstant, constant->GetIndex());
        return;
    }
    assert(mode_history_.top() == Mode::kCond);
    auto last_condition = conditions_.back().get();
    last_condition->AddConstant(constant);
}
void AbstractSyntaxTree::AddProcedure(std::string name) {
    auto [pair, inserted] = procedures_.try_emplace(
            std::move(name),
            LazyFactory([] { return std::make_unique<ProcedureNode>(); }));
    auto proc = pair->second.get();
    // A program cannot have two procedures with the same name.
    tree_valid_ = inserted || !proc->GetStmtlst();
    call_edges_.resize(procedures_.size() + 1);
    parent_path_.emplace_back(proc);
}
void AbstractSyntaxTree::AddRead(std::string name) {
    auto variable = AddVariable(std::move(name));
    auto read = std::make_unique<ReadNode>();
    read->SetVariable(variable);
    read_stmts_.emplace_back(std::move(read));
}
void AbstractSyntaxTree::AddPrint(std::string name) {
    auto variable = AddVariable(std::move(name));
    auto print = std::make_unique<PrintNode>();
    print->SetVariable(variable);
    print_stmts_.emplace_back(std::move(print));
}
void AbstractSyntaxTree::AddCall(std::string name) {
    auto [pair, inserted] = procedures_.try_emplace(
            std::move(name),
            LazyFactory([] { return std::make_unique<ProcedureNode>(); }));
    if (inserted) call_edges_.resize(procedures_.size() + 1);
    auto call = std::make_unique<CallNode>();
    auto callee = pair->second.get();
    call->SetProcedure(callee);

    // update the call graph
    auto caller = dynamic_cast<ProcedureNode *>(parent_path_.front());
    assert(caller);
    call_edges_[caller->GetIndex()].emplace(callee->GetIndex());
    call_stmts_.emplace_back(std::move(call));
}
void AbstractSyntaxTree::AddAssign(std::string name) {
    auto var = AddVariable(std::move(name));
    auto assign = std::make_unique<AssignNode>();
    assign->SetModifiedVar(var);
    assign_stmts_.emplace_back(std::move(assign));
    mode_history_.emplace(Mode::kAssign);
}
void AbstractSyntaxTree::LastCondAddVar(std::string name) {
    const auto variable = AddVariable(std::move(name));
    auto last_condition = conditions_.back().get();
    last_condition->AddVariable(variable);
}
void AbstractSyntaxTree::ExprAddVar(std::string name) {
    const auto variable = AddVariable(std::move(name));
    curr_expr_.emplace_back(ExprNodeType::kVariable, variable->GetIndex());
}
void AbstractSyntaxTree::ParseToken(const Token &token) {
    auto &[type, name] = token;
    switch (type) {
        case SourceTokenType::kKeywordProcedure:
        case SourceTokenType::kKeywordRead:
        case SourceTokenType::kKeywordPrint:
        case SourceTokenType::kKeywordCall:
        case SourceTokenType::kKeywordWhile:
        case SourceTokenType::kKeywordIf:
        case SourceTokenType::kKeywordThen:
        case SourceTokenType::kKeywordElse:
            mode_history_.emplace(KeywordToMode(type));
            break;
        case SourceTokenType::kBracketL:
            BracketL();
            break;
        case SourceTokenType::kBracketR:
            BracketR();
            break;
        case SourceTokenType::kBraceL:
            BraceL();
            break;
        case SourceTokenType::kBraceR:
            BraceR();
            break;
        case SourceTokenType::kAssignEqual:
            assert(mode_history_.top() == Mode::kAssign);
            curr_expr_.clear();
            break;
        case SourceTokenType::kOperatorPlus:
        case SourceTokenType::kOperatorMinus:
        case SourceTokenType::kOperatorTimes:
        case SourceTokenType::kOperatorDivide:
        case SourceTokenType::kOperatorModulo:
            if (mode_history_.top() == Mode::kAssign)
                curr_expr_.emplace_back(OperatorForRpn(type));
            // nothing to do but just ensure that it is the case
            else
                assert(mode_history_.top() == Mode::kCond);
            break;
        case SourceTokenType::kCondNot:
        case SourceTokenType::kCondAnd:
        case SourceTokenType::kCondOr:
        case SourceTokenType::kRelLt:
        case SourceTokenType::kRelLeq:
        case SourceTokenType::kRelEq:
        case SourceTokenType::kRelNeq:
        case SourceTokenType::kRelGt:
        case SourceTokenType::kRelGeq:
            assert(mode_history_.top() == Mode::kCond);
            break;
        case SourceTokenType::kSemicolon:
            Semicolon();
            break;
        case SourceTokenType::kName:
            Name(name);
            break;
        case SourceTokenType::kInteger: {
            Constant(name);
        } break;
    }
}
void AbstractSyntaxTree::BuildTree() {
    root_ = std::make_unique<ProgramNode>();
    tree_valid_ = true;
    for (const auto &token : tokens_) {
        if (!tree_valid_) break;
        ParseToken(token);
    }
}
void AbstractSyntaxTree::CheckTree() {
    if (!tree_valid_) return;
    // A procedure cannot call a non-existing procedure.
    if (procedures_.size() != root_->GetProcedures().size()) {
        tree_valid_ = false;
        return;
    }
    // Recursive and cyclic calls are not allowed.
    for (int i = 1; i < call_edges_.size(); ++i) {
        std::vector<bool> visited(call_edges_.size());
        if (HasCycle(i, visited)) {
            tree_valid_ = false;
            break;
        }
    }
}
bool AbstractSyntaxTree::HasCycle(int curr, std::vector<bool> visited) {
    visited[curr] = true;
    for (auto i : call_edges_[curr]) {
        if (visited[i] || HasCycle(i, visited)) {
            tree_valid_ = false;
            return true;
        }
    }
    return false;
}
constexpr AbstractSyntaxTree::Mode AbstractSyntaxTree::KeywordToMode(
        SourceTokenType keyword) {
    switch (keyword) {
        case SourceTokenType::kKeywordProcedure:
            return Mode::kProc;
        case SourceTokenType::kKeywordRead:
            return Mode::kRead;
        case SourceTokenType::kKeywordPrint:
            return Mode::kPrint;
        case SourceTokenType::kKeywordCall:
            return Mode::kCall;
        case SourceTokenType::kKeywordWhile:
            return Mode::kWhile;
        case SourceTokenType::kKeywordIf:
            return Mode::kIf;
        case SourceTokenType::kKeywordThen:
            return Mode::kThen;
        case SourceTokenType::kKeywordElse:
            return Mode::kElse;
        default:
            assert(false);
    }
}
constexpr OperatorType AbstractSyntaxTree::OperatorForRpn(
        SourceTokenType keyword) {
    switch (keyword) {
        case SourceTokenType::kOperatorPlus:
            return OperatorType::kPlus;
        case SourceTokenType::kOperatorMinus:
            return OperatorType::kMinus;
        case SourceTokenType::kOperatorTimes:
            return OperatorType::kTimes;
        case SourceTokenType::kOperatorDivide:
            return OperatorType::kDivide;
        case SourceTokenType::kOperatorModulo:
            return OperatorType::kModulo;
        default:
            assert(false);
    }
}
}  // namespace spa
