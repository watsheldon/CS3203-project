#include "program_knowledge_base.h"

#include <cassert>
#include <list>
#include <set>
#include <vector>

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "common/index.h"
#include "knowledge_base.h"
#include "secondary_structure/container_forest.h"

namespace spa {
ProgramKnowledgeBase::ProgramKnowledgeBase(BasicEntities init)
        : stmt_count_(init.assigns.size() + init.ifs.size() +
                      init.whiles.size() + init.calls.size() +
                      init.reads.size() + init.prints.size()),
          stmtlst_count_(init.procedures.size() + init.whiles.size() +
                         init.ifs.size() * 2 - 1),
          stmtlst_parent_(init.procedures.size() - 1, stmt_count_,
                          stmtlst_count_),
          stmtlst_stmt_(stmtlst_count_, stmt_count_),
          modifies_rel_(stmt_count_, init.variables.size() - 1),
          uses_rel_(stmt_count_, init.variables.size() - 1),
          call_proc_(stmt_count_, init.procedures.size() - 1,
                     std::move(init.proc_call_graph)),
          polish_notation_(stmt_count_, std::move(init.notations)),
          name_value_(std::move(init.procedures), std::move(init.variables),
                      std::move(init.constants)),
          type_stmt_(std::move(init.reads), std::move(init.prints),
                     std::move(init.calls), std::move(init.whiles),
                     std::move(init.ifs), std::move(init.assigns)),
          follows_parent_rel_(stmt_count_,
                              {type_stmt_, stmtlst_parent_, stmtlst_stmt_}) {}

void ProgramKnowledgeBase::SetIndex(
        Index<SetEntityType::kProc> proc_index,
        Index<SetEntityType::kStmtLst> stmtlst_index) {
    assert(!compiled);
    stmtlst_parent_.Set(proc_index, stmtlst_index);
}
void ProgramKnowledgeBase::SetIndex(
        Index<SetEntityType::kStmt> while_stmt,
        Index<SetEntityType::kStmtLst> stmtlst_index) {
    assert(!compiled);
    stmtlst_parent_.Set(while_stmt, stmtlst_index);
}
void ProgramKnowledgeBase::SetIndex(Index<SetEntityType::kStmt> if_stmt,
                                    Index<SetEntityType::kStmtLst> then_index,
                                    Index<SetEntityType::kStmtLst> else_index) {
    assert(!compiled);
    stmtlst_parent_.Set(if_stmt, then_index, else_index);
}
void ProgramKnowledgeBase::SetIndex(Index<SetEntityType::kStmt> call_stmt,
                                    Index<SetEntityType::kProc> proc_index) {
    assert(!compiled);
    call_proc_.Set(call_stmt.value, proc_index.value);
}

void ProgramKnowledgeBase::SetIndex(
        Index<SetEntityType::kStmt> assign_stmt,
        Index<SetEntityType::kNotation> notation_index) {
    assert(!compiled);
    polish_notation_.Set(assign_stmt.value, notation_index.value);
}

void ProgramKnowledgeBase::SetLst(Index<SetEntityType::kStmtLst> stmtlst_index,
                                  std::vector<int> stmtlst) {
    assert(!compiled);
    stmtlst_stmt_.Set(stmtlst_index.value, std::move(stmtlst));
}

void ProgramKnowledgeBase::SetRel(Index<SetEntityType::kStmt> stmt_no,
                                  Index<SetEntityType::kVar> var_index) {
    assert(!compiled);
    modifies_rel_.Set(stmt_no.value, var_index.value);
}

void ProgramKnowledgeBase::SetRel(Index<SetEntityType::kStmt> stmt_no,
                                  std::vector<int> var_indices) {
    assert(!compiled);
    uses_rel_.Set(stmt_no.value, std::move(var_indices));
}

bool ProgramKnowledgeBase::ExistFollows(bool transitive,
                                        Index<ArgPos::kFirst> first_stmt,
                                        Index<ArgPos::kSecond> second_stmt) {
    assert(compiled);
    return follows_parent_rel_.ExistFollows(transitive, first_stmt,
                                            second_stmt);
}
bool ProgramKnowledgeBase::ExistFollows(Index<ArgPos::kFirst> first_stmt) {
    assert(compiled);
    return follows_parent_rel_.ExistFollows(first_stmt);
}
bool ProgramKnowledgeBase::ExistFollows(Index<ArgPos::kSecond> second_stmt) {
    assert(compiled);
    return follows_parent_rel_.ExistFollows(second_stmt);
}
bool ProgramKnowledgeBase::ExistFollows() {
    assert(compiled);
    return follows_parent_rel_.ExistFollows();
}

std::set<int> ProgramKnowledgeBase::GetFollows(ArgPos return_pos,
                                               StmtType return_type) {
    assert(compiled);
    return follows_parent_rel_.GetFollows(return_pos, return_type);
}
std::set<int> ProgramKnowledgeBase::GetFollows(bool transitive,
                                               Index<ArgPos::kFirst> first_stmt,
                                               StmtType return_type) {
    assert(compiled);
    return follows_parent_rel_.GetFollows(transitive, first_stmt, return_type);
}

std::set<int> ProgramKnowledgeBase::GetFollows(
        bool transitive, Index<ArgPos::kSecond> second_stmt,
        StmtType return_type) {
    assert(compiled);
    return follows_parent_rel_.GetFollows(transitive, second_stmt, return_type);
}

PairVec<int> ProgramKnowledgeBase::GetFollowsPairs(bool transitive,
                                                   StmtType first_type,
                                                   StmtType second_type) {
    assert(compiled);
    return follows_parent_rel_.GetFollowsPairs(transitive, first_type,
                                               second_type);
}

bool ProgramKnowledgeBase::ExistParent(bool transitive,
                                       Index<ArgPos::kFirst> parent_stmt,
                                       Index<ArgPos::kSecond> child_stmt) {
    assert(compiled);
    return follows_parent_rel_.ExistParent(transitive, parent_stmt, child_stmt);
}
bool ProgramKnowledgeBase::ExistParent(Index<ArgPos::kFirst> parent_stmt) {
    assert(compiled);
    return follows_parent_rel_.ExistParent(parent_stmt);
}
bool ProgramKnowledgeBase::ExistParent(Index<ArgPos::kSecond> child_stmt) {
    assert(compiled);
    return follows_parent_rel_.ExistParent(child_stmt);
}
bool ProgramKnowledgeBase::ExistParent() {
    assert(compiled);
    return follows_parent_rel_.ExistParent();
}

std::set<int> ProgramKnowledgeBase::GetParent(ArgPos return_pos,
                                              StmtType return_type) {
    assert(compiled);
    return follows_parent_rel_.GetParent(return_pos, return_type);
}
std::set<int> ProgramKnowledgeBase::GetParent(bool transitive,
                                              Index<ArgPos::kFirst> parent_stmt,
                                              StmtType return_type) {
    assert(compiled);
    return follows_parent_rel_.GetParent(transitive, parent_stmt, return_type);
}
std::set<int> ProgramKnowledgeBase::GetParent(bool transitive,
                                              Index<ArgPos::kSecond> child_stmt,
                                              StmtType return_type) {
    assert(compiled);
    return follows_parent_rel_.GetParent(transitive, child_stmt, return_type);
}

PairVec<int> ProgramKnowledgeBase::GetParentPairs(bool transitive,
                                                  StmtType parent_type,
                                                  StmtType child_type) {
    assert(compiled);
    return follows_parent_rel_.GetParentPairs(transitive, parent_type,
                                              child_type);
}

bool ProgramKnowledgeBase::ExistModifies(int stmt_no, int var_index) {
    assert(compiled);
    assert(stmt_no != 0);
    if (stmt_no > stmt_count_) {
        return false;
    }
    auto modified_vars = modifies_rel_.GetAllVar(stmt_no);
    return var_index == 0
                   ? !modified_vars.empty()
                   : modified_vars.find(var_index) != modified_vars.end();
}

bool ProgramKnowledgeBase::ExistModifies(int stmt_no,
                                         std::string_view var_name) {
    auto var_index = IdentToIndex<QueryEntityType::kVar>(var_name);
    if (var_index == 0) return false;
    return ExistModifies(stmt_no, var_index);
}

bool ProgramKnowledgeBase::ExistUses(int stmt_no, int var_index) {
    assert(compiled);
    assert(stmt_no != 0);
    if (stmt_no > stmt_count_) {
        return false;
    }
    auto used_vars = uses_rel_.GetAllVar(stmt_no);
    return var_index == 0 ? !used_vars.empty()
                          : used_vars.find(var_index) != used_vars.end();
}

bool ProgramKnowledgeBase::ExistUses(int stmt_no, std::string_view var_name) {
    auto var_index = IdentToIndex<QueryEntityType::kVar>(var_name);
    if (var_index == 0) return false;
    return ExistUses(stmt_no, var_index);
}

std::set<int> ProgramKnowledgeBase::GetModifies(
        Index<QueryEntityType::kStmt> stmt_no) {
    assert(compiled);
    if (stmt_no.value > stmt_count_) {
        return {};
    }
    return modifies_rel_.GetAllVar(stmt_no.value);
}

std::set<int> ProgramKnowledgeBase::GetModifies(std::string_view var_name,
                                                StmtType type) {
    auto var_index = IdentToIndex<QueryEntityType::kVar>(var_name);
    if (var_index == 0) return {};
    return GetModifies(var_index, type);
}

std::set<int> ProgramKnowledgeBase::GetModifies(
        Index<QueryEntityType::kVar> var_index, StmtType type) {
    assert(compiled);
    assert(var_index.value != 0);
    auto stmts = modifies_rel_.GetAllStmt(var_index.value);
    if (type == StmtType::kAll) {
        return stmts;
    }
    for (auto it = stmts.begin(); it != stmts.end();) {
        if (type_stmt_.GetType(*it) != type) {
            it = stmts.erase(it);
        } else {
            ++it;
        }
    }
    return stmts;
}

std::set<int> ProgramKnowledgeBase::GetModifies(StmtType type) {
    assert(compiled);
    return modifies_rel_.GetStmt(type);
}

PairVec<int> ProgramKnowledgeBase::GetModifiesStmtVar(StmtType type) {
    assert(compiled);
    return modifies_rel_.GetStmtVar(type);
}

std::set<int> ProgramKnowledgeBase::GetUses(
        Index<QueryEntityType::kStmt> stmt_no) {
    assert(compiled);
    if (stmt_no.value > stmt_count_) {
        return {};
    }
    return uses_rel_.GetAllVar(stmt_no.value);
}
std::set<int> ProgramKnowledgeBase::GetUses(
        Index<QueryEntityType::kVar> var_index, StmtType type) {
    assert(compiled);
    assert(var_index.value != 0);
    auto stmts = uses_rel_.GetAllStmt(var_index.value);
    if (type == StmtType::kAll) {
        return stmts;
    }
    for (auto it = stmts.begin(); it != stmts.end();) {
        if (type_stmt_.GetType(*it) != type) {
            it = stmts.erase(it);
        } else {
            ++it;
        }
    }
    return stmts;
}
std::set<int> ProgramKnowledgeBase::GetUses(std::string_view var_name,
                                            StmtType type) {
    auto var_index = IdentToIndex<QueryEntityType::kVar>(var_name);
    if (var_index == 0) return {};
    return GetUses(var_index, type);
}

std::set<int> ProgramKnowledgeBase::GetUses(StmtType type) {
    return uses_rel_.GetStmt(type);
}

PairVec<int> ProgramKnowledgeBase::GetUsesStmtVar(StmtType type) {
    assert(compiled);
    return uses_rel_.GetStmtVar(type);
}

bool ProgramKnowledgeBase::ExistModifies(std::string_view proc_name,
                                         std::string_view var_name) {
    return false;
}
bool ProgramKnowledgeBase::ExistModifies(std::string_view proc_name) {
    return false;
}
std::set<int> ProgramKnowledgeBase::GetModifies(
        Name<ArgPos::kFirst> proc_name) {
    return {};
}
std::set<int> ProgramKnowledgeBase::GetModifies(
        Name<ArgPos::kSecond> var_name) {
    return {};
}
std::set<int> ProgramKnowledgeBase::GetModifiesProc() { return {}; }
PairVec<int> ProgramKnowledgeBase::GetModifiesProcVar() { return {}; }
bool ProgramKnowledgeBase::ExistUses(std::string_view proc_name,
                                     std::string_view var_name) {
    return false;
}
bool ProgramKnowledgeBase::ExistUses(std::string_view proc_name) {
    return false;
}
std::set<int> ProgramKnowledgeBase::GetUses(Name<ArgPos::kFirst> proc_name) {
    return {};
}
std::set<int> ProgramKnowledgeBase::GetUses(Name<ArgPos::kSecond> var_name) {
    return {};
}
std::set<int> ProgramKnowledgeBase::GetUsesProc() { return {}; }
PairVec<int> ProgramKnowledgeBase::GetUsesProcVar() { return {}; }

// ( _, " "), (_, _" "_)
std::set<int> ProgramKnowledgeBase::GetPattern(std::vector<QueryToken> tokens,
                                               bool partial_match) {
    assert(compiled);
    auto converted_token = ConvertFromQueryTokens(tokens);
    if (converted_token.second) {
        return {};
    }

    std::vector<int> assign_stmt;
    assign_stmt = type_stmt_.GetStatements(StmtType::kAssign);
    PN converted_pn = converted_token.first;

    if (partial_match) {
        std::set<int> partial_match_stmt;

        for (int i : assign_stmt) {
            const PN &pn = polish_notation_.GetNotation(
                    polish_notation_.GetPolishIndex(i));
            if (pn.Contains(converted_pn)) {
                partial_match_stmt.emplace(i);
            }
        }
        return partial_match_stmt;
    }

    assert(partial_match == false);
    std::set<int> full_match_stmt;

    for (auto &i : assign_stmt) {
        const PN &pn = polish_notation_.GetNotation(
                polish_notation_.GetPolishIndex(i));
        if (pn == converted_pn) {
            full_match_stmt.emplace(i);
        }
    }

    return full_match_stmt;
}

// (" ", _)
std::set<int> ProgramKnowledgeBase::GetPattern(int var_index) {
    assert(compiled);
    return GetModifies(Index<QueryEntityType::kVar>(var_index),
                       StmtType::kAssign);
}
std::set<int> ProgramKnowledgeBase::GetPattern(std::string_view var_name) {
    auto var_index = IdentToIndex<QueryEntityType::kVar>(var_name);
    if (var_index == 0) return {};
    return GetPattern(var_index);
}
// (" ", " ") , (" ", _" "_)
std::set<int> ProgramKnowledgeBase::GetPattern(
        std::string_view var_name, std::vector<QueryToken> second_tokens,
        bool partial_match) {
    auto var_index = IdentToIndex<QueryEntityType::kVar>(var_name);
    if (var_index == 0) return {};
    return GetPattern(var_index, second_tokens, partial_match);
}
std::set<int> ProgramKnowledgeBase::GetPattern(
        int var_index, std::vector<QueryToken> second_tokens,
        bool partial_match) {
    assert(compiled);

    std::set<int> assign_stmt;
    std::set<int> filtered_assign_stmt;
    assign_stmt = GetPattern(second_tokens, partial_match);

    for (auto &i : assign_stmt) {
        if (ExistModifies(i, var_index)) {
            filtered_assign_stmt.emplace(i);
        }
    }
    return filtered_assign_stmt;
}
// (v, " ") , (v, _" "_)
PairVec<int> ProgramKnowledgeBase::GetPatternPair(
        std::vector<QueryToken> tokens, bool partial_match) {
    assert(compiled);

    std::set<int> assign_stmt_set;
    std::vector<int> assign_var;
    std::vector<int> assign_stmt;
    assign_stmt_set = GetPattern(tokens, partial_match);

    for (auto &i : assign_stmt_set) {
        auto index = Index<QueryEntityType::kStmt>(i);
        assign_var.emplace_back(*GetModifies(index).begin());
    }

    assign_stmt.assign(assign_stmt_set.begin(), assign_stmt_set.end());

    return {assign_stmt, assign_var};
}

// (v, _)
PairVec<int> ProgramKnowledgeBase::GetPatternPair() {
    assert(compiled);
    return GetModifiesStmtVar(StmtType::kAssign);
}
bool ProgramKnowledgeBase::ExistCalls(Index<ArgPos::kFirst> first_proc,
                                      Index<ArgPos::kSecond> second_proc) {
    assert(compiled);
    const auto &callees = call_proc_.GetCalleeProcs(first_proc);
    return callees.find(second_proc) != callees.end();
}
bool ProgramKnowledgeBase::ExistCalls(std::string_view first_proc_name,
                                      std::string_view second_proc_name) {
    auto first_proc = IdentToIndex<ArgPos::kFirst>(first_proc_name,
                                                   QueryEntityType::kProc);
    auto second_proc = IdentToIndex<ArgPos::kSecond>(second_proc_name,
                                                     QueryEntityType::kProc);
    if (first_proc == 0 || second_proc == 0) return {};
    return ExistCalls(first_proc, second_proc);
}
bool ProgramKnowledgeBase::ExistCallsT(Index<ArgPos::kFirst> first_proc,
                                       Index<ArgPos::kSecond> second_proc) {
    assert(compiled);
    const auto &callees = call_proc_.GetCalleeProcsT(first_proc);
    return callees.find(second_proc) != callees.end();
}
bool ProgramKnowledgeBase::ExistCallsT(std::string_view first_proc_name,
                                       std::string_view second_proc_name) {
    auto first_proc = IdentToIndex<ArgPos::kFirst>(first_proc_name,
                                                   QueryEntityType::kProc);
    auto second_proc = IdentToIndex<ArgPos::kSecond>(second_proc_name,
                                                     QueryEntityType::kProc);
    if (first_proc == 0 || second_proc == 0) return {};
    return ExistCallsT(first_proc, second_proc);
}
bool ProgramKnowledgeBase::ExistCalls(Index<ArgPos::kFirst> first_proc) {
    assert(compiled);
    return !call_proc_.GetCalleeProcs(first_proc).empty();
}
bool ProgramKnowledgeBase::ExistCalls(Name<ArgPos::kFirst> proc_name) {
    auto first_proc =
            IdentToIndex<ArgPos::kFirst>(proc_name, QueryEntityType::kProc);
    if (first_proc == 0) return {};
    return ExistCalls(first_proc);
}
bool ProgramKnowledgeBase::ExistCalls(Index<ArgPos::kSecond> proc) {
    assert(compiled);
    return !call_proc_.GetStmts(proc).empty();
}
bool ProgramKnowledgeBase::ExistCalls(Name<ArgPos::kSecond> proc_name) {
    auto second_proc =
            IdentToIndex<ArgPos::kSecond>(proc_name, QueryEntityType::kProc);
    if (second_proc == 0) return {};
    return ExistCalls(second_proc);
}
bool ProgramKnowledgeBase::ExistCalls() {
    assert(compiled);
    return !type_stmt_.GetStatements(StmtType::kCall).empty();
}
std::set<int> ProgramKnowledgeBase::GetCalls(ArgPos return_pos) {
    assert(compiled);
    if (return_pos == ArgPos::kFirst) {
        return call_proc_.GetAllCallers();
    }
    return call_proc_.GetAllCallees();
}
std::set<int> ProgramKnowledgeBase::GetCalls(Index<ArgPos::kFirst> proc) {
    assert(compiled);
    return call_proc_.GetCalleeProcs(proc);
}
std::set<int> ProgramKnowledgeBase::GetCalls(Name<ArgPos::kFirst> proc_name) {
    auto first_proc =
            IdentToIndex<ArgPos::kFirst>(proc_name, QueryEntityType::kProc);
    if (first_proc == 0) return {};
    return GetCalls(first_proc);
}
std::set<int> ProgramKnowledgeBase::GetCallsT(
        Index<ArgPos::kFirst> first_proc) {
    assert(compiled);
    return call_proc_.GetCalleeProcsT(first_proc.value);
}
std::set<int> ProgramKnowledgeBase::GetCallsT(Name<ArgPos::kFirst> proc_name) {
    auto first_proc =
            IdentToIndex<ArgPos::kFirst>(proc_name, QueryEntityType::kProc);
    if (first_proc == 0) return {};
    return GetCallsT(first_proc);
}
std::set<int> ProgramKnowledgeBase::GetCalls(Index<ArgPos::kSecond> proc) {
    assert(compiled);
    return call_proc_.GetCallerProcs(proc);
}
std::set<int> ProgramKnowledgeBase::GetCalls(Name<ArgPos::kSecond> proc_name) {
    auto second_proc =
            IdentToIndex<ArgPos::kSecond>(proc_name, QueryEntityType::kProc);
    if (second_proc == 0) return {};
    return GetCalls(second_proc);
}
std::set<int> ProgramKnowledgeBase::GetCallsT(Index<ArgPos::kSecond> proc) {
    assert(compiled);
    return call_proc_.GetCallerProcsT(proc);
}
std::set<int> ProgramKnowledgeBase::GetCallsT(Name<ArgPos::kSecond> proc_name) {
    auto second_proc =
            IdentToIndex<ArgPos::kSecond>(proc_name, QueryEntityType::kProc);
    if (second_proc == 0) return {};
    return GetCallsT(second_proc);
}
PairVec<int> ProgramKnowledgeBase::GetCallsPairs() {
    assert(compiled);
    return call_proc_.GetCallsPairs();
}
PairVec<int> ProgramKnowledgeBase::GetCallsTPairs() {
    assert(compiled);
    return call_proc_.GetCallsTPairs();
}

void ProgramKnowledgeBase::Compile() {
    assert(!compiled);
    container_forest_ = std::make_unique<ContainerForest>(
            stmtlst_parent_, stmtlst_stmt_, stmtlst_count_);
    follows_parent_rel_.container_forest_ = container_forest_.get();
    uses_rel_.Compile(type_stmt_,
                      {*container_forest_, stmtlst_parent_, stmtlst_stmt_});
    modifies_rel_.Compile(type_stmt_,
                          {*container_forest_, stmtlst_parent_, stmtlst_stmt_});
    compiled = true;
}
std::vector<int> ProgramKnowledgeBase::GetAllEntityIndices(QueryEntityType et) {
    assert(compiled);
    std::vector<int> results;
    switch (et) {
        case QueryEntityType::kProc:
            results.resize(name_value_.GetSize(QueryEntityType::kProc));
            break;
        case QueryEntityType::kVar:
            results.resize(name_value_.GetSize(QueryEntityType::kVar));
            break;
        case QueryEntityType::kConst:
            results.resize(name_value_.GetSize(QueryEntityType::kConst));
            break;
        case QueryEntityType::kStmt:
            results.resize(stmt_count_);
            break;
        default:
            results.resize(0);
    }
    std::iota(results.begin(), results.end(), 1);
    return results;
}
std::vector<int> ProgramKnowledgeBase::GetAllEntityIndices(StmtType st) {
    assert(compiled);
    if (st == StmtType::kAll) {
        std::vector<int> results(stmt_count_);
        std::iota(results.begin(), results.end(), 1);
        return results;
    }
    return type_stmt_.GetStatements(st);
}

std::vector<int> ProgramKnowledgeBase::GetAllEntityIndices(
        Synonym::Type synType) {
    assert(compiled);
    std::vector<int> results;
    switch (synType) {
        case Synonym::kProc:
            return GetAllEntityIndices(QueryEntityType::kProc);
        case Synonym::kVar:
            return GetAllEntityIndices(QueryEntityType::kVar);
        case Synonym::kConst:
            return GetAllEntityIndices(QueryEntityType::kConst);
        case Synonym::kStmtAny:
            return GetAllEntityIndices(StmtType::kAll);
        case Synonym::kStmtAssign:
            return GetAllEntityIndices(StmtType::kAssign);
        case Synonym::kStmtCall:
            return GetAllEntityIndices(StmtType::kCall);
        case Synonym::kStmtIf:
            return GetAllEntityIndices(StmtType::kIf);
        case Synonym::kStmtPrint:
            return GetAllEntityIndices(StmtType::kPrint);
        case Synonym::kStmtRead:
            return GetAllEntityIndices(StmtType::kRead);
        case Synonym::kStmtWhile:
            return GetAllEntityIndices(StmtType::kWhile);
        default:
            assert(false);
    }
}

void ProgramKnowledgeBase::ToName(Synonym::Type syn_type,
                                  const std::vector<int> &index_list,
                                  std::list<std::string> &names) {
    assert(compiled);
    switch (syn_type) {
        case Synonym::kProc:
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(names), [this](int i) {
                               return name_value_.GetNameValue(
                                       i, QueryEntityType::kProc);
                           });
            break;
        case Synonym::kVar:
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(names), [this](int i) {
                               return name_value_.GetNameValue(
                                       i, QueryEntityType::kVar);
                           });
            break;
        case Synonym::kConst:
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(names), [this](int i) {
                               return name_value_.GetNameValue(
                                       i, QueryEntityType::kConst);
                           });
            break;
        case Synonym::kNone:
            break;
        default:
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(names),
                           [](int i) { return std::to_string(i); });
    }
}

int ProgramKnowledgeBase::IdentToIndexValue(std::string_view name,
                                            QueryEntityType et) {
    assert(compiled);
    assert(et == QueryEntityType::kVar || et == QueryEntityType::kProc);
    return (et == QueryEntityType::kVar)
                   ? name_value_.GetIndex(name.data(), QueryEntityType::kVar)
                   : name_value_.GetIndex(name.data(), QueryEntityType::kProc);
}

std::pair<PolishNotation, bool> ProgramKnowledgeBase::ConvertFromQueryTokens(
        const std::vector<QueryToken> &tokens) {
    std::vector<PolishNotationNode> expr;
    bool contains_unseen = false;
    for (const auto &token : tokens) {
        switch (token.type) {
            case QueryTokenType::kWord: {
                int var_index = name_value_.GetIndex(token.value,
                                                     QueryEntityType::kVar);
                if (var_index == 0) {
                    contains_unseen = true;
                    break;
                }
                PolishNotationNode node(ExprNodeType::kVariable, var_index);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::kInteger: {
                int const_index = name_value_.GetIndex(token.value,
                                                       QueryEntityType::kConst);
                if (const_index == 0) {
                    contains_unseen = true;
                    break;
                }
                PolishNotationNode node(ExprNodeType::kConstant, const_index);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::kOperatorPlus: {
                PolishNotationNode node(OperatorType::kPlus);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::kOperatorMinus: {
                PolishNotationNode node(OperatorType::kMinus);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::kOperatorTimes: {
                PolishNotationNode node(OperatorType::kTimes);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::kOperatorDivide: {
                PolishNotationNode node(OperatorType::kDivide);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::kOperatorModulo: {
                PolishNotationNode node(OperatorType::kModulo);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::kBracketL: {
                PolishNotationNode node(ExprNodeType::kBracketL);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::kBracketR: {
                PolishNotationNode node(ExprNodeType::kBracketR);
                expr.emplace_back(node);
                break;
            }
            default:
                assert(false);
        }
        if (contains_unseen) {
            break;
        }
    }
    return {PolishNotation(expr), contains_unseen};
}

}  // namespace spa
