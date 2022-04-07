#include "program_knowledge_base.h"

#include <cassert>
#include <list>
#include <set>
#include <vector>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "knowledge_base.h"
#include "secondary_structure/container_forest.h"
#include "secondary_structure/control_flow_graph.h"
#include "secondary_structure/next_calculator.h"

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
          modifies_rel_(stmt_count_, init.variables.size() - 1,
                        init.procedures.size() - 1),
          uses_rel_(stmt_count_, init.variables.size() - 1,
                    init.procedures.size() - 1),
          call_proc_(stmt_count_, init.procedures.size() - 1,
                     std::move(init.proc_call_graph)),
          name_value_(std::move(init.procedures), std::move(init.variables),
                      std::move(init.constants)),
          polish_notation_(stmt_count_, std::move(init.notations), name_value_),
          type_stmt_(std::move(init.reads), std::move(init.prints),
                     std::move(init.calls), std::move(init.whiles),
                     std::move(init.ifs), std::move(init.assigns)),
          follows_store_({type_stmt_, stmtlst_parent_, stmtlst_stmt_}),
          parent_store_(stmt_count_,
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
                                  std::vector<StmtNo> stmtlst) {
    assert(!compiled);
    stmtlst_stmt_.Set(stmtlst_index.value, std::move(stmtlst));
}
void ProgramKnowledgeBase::SetRel(Index<SetEntityType::kStmt> stmt_no,
                                  Index<SetEntityType::kVar> var_index) {
    assert(!compiled);
    modifies_rel_.Set(stmt_no.value, var_index.value);
}
void ProgramKnowledgeBase::SetRel(Index<SetEntityType::kStmt> stmt_no,
                                  std::vector<VarIndex> var_indices) {
    assert(!compiled);
    uses_rel_.Set(stmt_no.value, std::move(var_indices));
}
bool ProgramKnowledgeBase::ExistFollows(Index<ArgPos::kFirst> first_stmt,
                                        Index<ArgPos::kSecond> second_stmt) {
    assert(compiled);
    return follows_store_.IsNonTransitive(first_stmt, second_stmt);
}
bool ProgramKnowledgeBase::ExistFollowsT(Index<ArgPos::kFirst> first_stmt,
                                         Index<ArgPos::kSecond> second_stmt) {
    assert(compiled);
    return follows_store_.IsTransitive(first_stmt, second_stmt);
}
bool ProgramKnowledgeBase::ExistFollows(Index<ArgPos::kFirst> first_stmt) {
    assert(compiled);
    return follows_store_.HasSecondValues(first_stmt);
}
bool ProgramKnowledgeBase::ExistFollows(Index<ArgPos::kSecond> second_stmt) {
    assert(compiled);
    return follows_store_.HasFirstValues(second_stmt);
}
bool ProgramKnowledgeBase::ExistFollows() {
    assert(compiled);
    return follows_store_.ExistRelationship();
}
std::set<StmtNo> ProgramKnowledgeBase::GetFollows(ArgPos return_pos,
                                                  StmtType return_type) {
    assert(compiled);
    return return_pos == ArgPos::kFirst
                   ? follows_store_.GetFirstArg(return_type)
                   : follows_store_.GetSecondArg(return_type);
}
std::set<StmtNo> ProgramKnowledgeBase::GetFollows(Index<ArgPos::kFirst> stmt_no,
                                                  StmtType return_type) {
    assert(compiled);
    return follows_store_.GetSecondArg(stmt_no, return_type);
}
std::set<StmtNo> ProgramKnowledgeBase::GetFollowsT(
        Index<ArgPos::kFirst> stmt_no, StmtType return_type) {
    assert(compiled);
    return follows_store_.GetSecondArgT(stmt_no, return_type);
}
std::set<StmtNo> ProgramKnowledgeBase::GetFollows(
        Index<ArgPos::kSecond> stmt_no, StmtType return_type) {
    assert(compiled);
    return follows_store_.GetFirstArg(stmt_no, return_type);
}
std::set<StmtNo> ProgramKnowledgeBase::GetFollowsT(
        Index<ArgPos::kSecond> stmt_no, StmtType return_type) {
    assert(compiled);
    return follows_store_.GetFirstArgT(stmt_no, return_type);
}
PairVec<StmtNo> ProgramKnowledgeBase::GetFollowsPairs(StmtType first_type,
                                                      StmtType second_type) {
    assert(compiled);
    return follows_store_.GetBothArgs(first_type, second_type);
}
PairVec<StmtNo> ProgramKnowledgeBase::GetFollowsTPairs(StmtType first_type,
                                                       StmtType second_type) {
    assert(compiled);
    return follows_store_.GetBothArgsT(first_type, second_type);
}
bool ProgramKnowledgeBase::ExistParent(Index<ArgPos::kFirst> parent_stmt,
                                       Index<ArgPos::kSecond> child_stmt) {
    assert(compiled);
    return parent_store_.IsNonTransitive(parent_stmt, child_stmt);
}
bool ProgramKnowledgeBase::ExistParentT(Index<ArgPos::kFirst> parent_stmt,
                                        Index<ArgPos::kSecond> child_stmt) {
    assert(compiled);
    return parent_store_.IsTransitive(parent_stmt, child_stmt);
}
bool ProgramKnowledgeBase::ExistParent(Index<ArgPos::kFirst> parent_stmt) {
    assert(compiled);
    return parent_store_.HasSecondValues(parent_stmt);
}
bool ProgramKnowledgeBase::ExistParent(Index<ArgPos::kSecond> child_stmt) {
    assert(compiled);
    return parent_store_.HasFirstValues(child_stmt);
}
bool ProgramKnowledgeBase::ExistParent() {
    assert(compiled);
    return parent_store_.ExistRelationship();
}
std::set<StmtNo> ProgramKnowledgeBase::GetParent(ArgPos return_pos,
                                                 StmtType return_type) {
    assert(compiled);
    return return_pos == ArgPos::kFirst
                   ? parent_store_.GetFirstArg(return_type)
                   : parent_store_.GetSecondArg(return_type);
}
std::set<StmtNo> ProgramKnowledgeBase::GetParent(
        Index<ArgPos::kFirst> parent_stmt, StmtType return_type) {
    assert(compiled);
    return parent_store_.GetSecondArg(parent_stmt, return_type);
}
std::set<StmtNo> ProgramKnowledgeBase::GetParentT(
        Index<ArgPos::kFirst> parent_stmt, StmtType return_type) {
    assert(compiled);
    return parent_store_.GetSecondArgT(parent_stmt, return_type);
}
std::set<StmtNo> ProgramKnowledgeBase::GetParent(
        Index<ArgPos::kSecond> child_stmt, StmtType return_type) {
    assert(compiled);
    return parent_store_.GetFirstArg(child_stmt, return_type);
}
std::set<StmtNo> ProgramKnowledgeBase::GetParentT(
        Index<ArgPos::kSecond> child_stmt, StmtType return_type) {
    assert(compiled);
    return parent_store_.GetFirstArgT(child_stmt, return_type);
}
PairVec<StmtNo> ProgramKnowledgeBase::GetParentPairs(StmtType parent_type,
                                                     StmtType child_type) {
    assert(compiled);
    return parent_store_.GetBothArgs(parent_type, child_type);
}
PairVec<StmtNo> ProgramKnowledgeBase::GetParentTPairs(StmtType parent_type,
                                                      StmtType child_type) {
    assert(compiled);
    return parent_store_.GetBothArgsT(parent_type, child_type);
}
bool ProgramKnowledgeBase::ExistModifies(StmtNo stmt_no, VarIndex var_index) {
    assert(compiled);
    return modifies_rel_.ExistModifies(stmt_no, var_index);
}
bool ProgramKnowledgeBase::ExistModifies(StmtNo stmt_no,
                                         std::string_view var_name) {
    auto var_index = IdentToIndex<QueryEntityType::kVar>(var_name);
    if (var_index == 0) return false;
    return ExistModifies(stmt_no, var_index);
}
bool ProgramKnowledgeBase::ExistUses(StmtNo stmt_no, VarIndex var_index) {
    assert(compiled);
    return uses_rel_.ExistUses(stmt_no, var_index);
}
bool ProgramKnowledgeBase::ExistUses(StmtNo stmt_no,
                                     std::string_view var_name) {
    auto var_index = IdentToIndex<QueryEntityType::kVar>(var_name);
    if (var_index == 0) return false;
    return ExistUses(stmt_no, var_index);
}
std::set<VarIndex> ProgramKnowledgeBase::GetModifies(
        Index<QueryEntityType::kStmt> stmt_no) {
    assert(compiled);
    return modifies_rel_.GetModifies(stmt_no);
}
std::set<StmtNo> ProgramKnowledgeBase::GetModifies(std::string_view var_name,
                                                   StmtType type) {
    auto var_index = IdentToIndex<QueryEntityType::kVar>(var_name);
    if (var_index == 0) return {};
    return GetModifies(var_index, type);
}
std::set<StmtNo> ProgramKnowledgeBase::GetModifies(
        Index<QueryEntityType::kVar> var_index, StmtType type) {
    assert(compiled);
    return modifies_rel_.GetModifies(var_index, type, type_stmt_);
}
std::set<StmtNo> ProgramKnowledgeBase::GetModifies(StmtType type) {
    assert(compiled);
    return modifies_rel_.GetStmt(type);
}
PairVec<StmtNo, VarIndex> ProgramKnowledgeBase::GetModifiesStmtVar(
        StmtType type) {
    assert(compiled);
    return modifies_rel_.GetStmtVar(type);
}
std::set<VarIndex> ProgramKnowledgeBase::GetUses(
        Index<QueryEntityType::kStmt> stmt_no) {
    assert(compiled);
    return uses_rel_.GetUses(stmt_no);
}
std::set<StmtNo> ProgramKnowledgeBase::GetUses(
        Index<QueryEntityType::kVar> var_index, StmtType type) {
    assert(compiled);
    return uses_rel_.GetUses(var_index, type, type_stmt_);
}
std::set<StmtNo> ProgramKnowledgeBase::GetUses(std::string_view var_name,
                                               StmtType type) {
    auto var_index = IdentToIndex<QueryEntityType::kVar>(var_name);
    if (var_index == 0) return {};
    return GetUses(var_index, type);
}
std::set<StmtNo> ProgramKnowledgeBase::GetUses(StmtType type) {
    return uses_rel_.GetStmt(type);
}
PairVec<StmtNo, VarIndex> ProgramKnowledgeBase::GetUsesStmtVar(StmtType type) {
    assert(compiled);
    return uses_rel_.GetStmtVar(type);
}
bool ProgramKnowledgeBase::ExistModifies(std::string_view proc_name,
                                         std::string_view var_name) {
    assert(compiled);
    ProcIndex proc_index = IdentToIndexValue(proc_name, QueryEntityType::kProc);
    VarIndex var_index = IdentToIndexValue(var_name, QueryEntityType::kVar);
    if (proc_index == 0 || var_index == 0) return false;
    return modifies_rel_.ExistModifiesP(proc_index, var_index);
}
bool ProgramKnowledgeBase::ExistModifies(std::string_view proc_name) {
    assert(compiled);
    ProcIndex proc_index = IdentToIndexValue(proc_name, QueryEntityType::kProc);
    return modifies_rel_.ExistModifiesP(proc_index);
}
std::set<VarIndex> ProgramKnowledgeBase::GetModifies(
        Name<ArgPos::kFirst> proc_name) {
    assert(compiled);
    ProcIndex proc_index = IdentToIndexValue(proc_name, QueryEntityType::kProc);
    return modifies_rel_.GetVarAccessByProc(proc_index);
}
std::set<ProcIndex> ProgramKnowledgeBase::GetModifies(
        Name<ArgPos::kSecond> var_name) {
    assert(compiled);
    VarIndex var_index = IdentToIndexValue(var_name, QueryEntityType::kVar);
    return modifies_rel_.GetAllProc(var_index);
}
std::set<ProcIndex> ProgramKnowledgeBase::GetModifiesProc() {
    return modifies_rel_.GetAllProc();
}
PairVec<ProcIndex, VarIndex> ProgramKnowledgeBase::GetModifiesProcVar() {
    return modifies_rel_.GetProcVar();
}
bool ProgramKnowledgeBase::ExistUses(std::string_view proc_name,
                                     std::string_view var_name) {
    assert(compiled);
    ProcIndex proc_index = IdentToIndexValue(proc_name, QueryEntityType::kProc);
    VarIndex var_index = IdentToIndexValue(var_name, QueryEntityType::kVar);
    if (proc_index == 0 || var_index == 0) return false;
    return uses_rel_.ExistUsesP(proc_index, var_index);
}
bool ProgramKnowledgeBase::ExistUses(std::string_view proc_name) {
    assert(compiled);
    ProcIndex proc_index = IdentToIndexValue(proc_name, QueryEntityType::kProc);
    return uses_rel_.ExistUsesP(proc_index);
}
std::set<VarIndex> ProgramKnowledgeBase::GetUses(
        Name<ArgPos::kFirst> proc_name) {
    assert(compiled);
    ProcIndex proc_index = IdentToIndexValue(proc_name, QueryEntityType::kProc);
    return uses_rel_.GetVarAccessByProc(proc_index);
}
std::set<ProcIndex> ProgramKnowledgeBase::GetUses(
        Name<ArgPos::kSecond> var_name) {
    assert(compiled);
    VarIndex var_index = IdentToIndexValue(var_name, QueryEntityType::kVar);
    return uses_rel_.GetAllProc(var_index);
}
std::set<ProcIndex> ProgramKnowledgeBase::GetUsesProc() {
    assert(compiled);
    return uses_rel_.GetAllProc();
}
PairVec<ProcIndex, VarIndex> ProgramKnowledgeBase::GetUsesProcVar() {
    assert(compiled);
    return uses_rel_.GetProcVar();
}
std::set<int> ProgramKnowledgeBase::GetPattern(std::vector<QueryToken> tokens) {
    assert(compiled);
    auto converted_token = polish_notation_.ConvertFromQueryTokens(tokens);
    if (!converted_token) {
        return {};
    }

    PN *converted_pn = converted_token.get();
    return polish_notation_.CheckPattern(
            *converted_pn, type_stmt_.GetStatements(StmtType::kAssign));
}
std::set<int> ProgramKnowledgeBase::GetPatternP(
        std::vector<QueryToken> tokens) {
    assert(compiled);
    auto converted_token = polish_notation_.ConvertFromQueryTokens(tokens);
    if (!converted_token) {
        return {};
    }

    PN *converted_pn = converted_token.get();
    return polish_notation_.CheckPatternP(
            *converted_pn, type_stmt_.GetStatements(StmtType::kAssign));
}
std::set<StmtNo> ProgramKnowledgeBase::GetPattern(VarIndex var_index) {
    assert(compiled);
    return GetModifies(Index<QueryEntityType::kVar>(var_index),
                       StmtType::kAssign);
}
std::set<StmtNo> ProgramKnowledgeBase::GetPattern(std::string_view var_name) {
    auto var_index = IdentToIndex<QueryEntityType::kVar>(var_name);
    if (var_index == 0) return {};
    return GetPattern(var_index);
}
std::set<StmtNo> ProgramKnowledgeBase::GetPattern(
        std::string_view var_name, std::vector<QueryToken> second_tokens) {
    auto var_index = IdentToIndex<QueryEntityType::kVar>(var_name);
    if (var_index == 0) return {};
    return GetPattern(var_index, second_tokens);
}
std::set<StmtNo> ProgramKnowledgeBase::GetPatternP(
        std::string_view var_name, std::vector<QueryToken> second_tokens) {
    auto var_index = IdentToIndex<QueryEntityType::kVar>(var_name);
    if (var_index == 0) return {};
    return GetPatternP(var_index, second_tokens);
}
std::set<StmtNo> ProgramKnowledgeBase::GetPattern(
        VarIndex var_index, std::vector<QueryToken> second_tokens) {
    assert(compiled);

    std::set<StmtNo> assign_stmt;
    std::set<StmtNo> filtered_assign_stmt;
    assign_stmt = GetPattern(second_tokens);

    for (auto &i : assign_stmt) {
        if (ExistModifies(i, var_index)) {
            filtered_assign_stmt.emplace(i);
        }
    }
    return filtered_assign_stmt;
}
std::set<StmtNo> ProgramKnowledgeBase::GetPatternP(
        VarIndex var_index, std::vector<QueryToken> second_tokens) {
    assert(compiled);

    std::set<StmtNo> assign_stmt;
    std::set<StmtNo> filtered_assign_stmt;
    assign_stmt = GetPatternP(second_tokens);

    for (auto &i : assign_stmt) {
        if (ExistModifies(i, var_index)) {
            filtered_assign_stmt.emplace(i);
        }
    }
    return filtered_assign_stmt;
}
PairVec<StmtNo, VarIndex> ProgramKnowledgeBase::GetPatternPair(
        std::vector<QueryToken> tokens) {
    assert(compiled);

    std::set<StmtNo> assign_stmt_set;
    std::vector<VarIndex> assign_var;
    std::vector<StmtNo> assign_stmt;
    assign_stmt_set = GetPattern(tokens);

    for (auto &i : assign_stmt_set) {
        auto index = Index<QueryEntityType::kStmt>(i);
        assign_var.emplace_back(*GetModifies(index).begin());
    }

    assign_stmt.assign(assign_stmt_set.begin(), assign_stmt_set.end());

    return {assign_stmt, assign_var};
}
PairVec<StmtNo, VarIndex> ProgramKnowledgeBase::GetPatternPairP(
        std::vector<QueryToken> tokens) {
    assert(compiled);

    std::set<StmtNo> assign_stmt_set;
    std::vector<VarIndex> assign_var;
    std::vector<StmtNo> assign_stmt;
    assign_stmt_set = GetPatternP(tokens);

    for (auto &i : assign_stmt_set) {
        auto index = Index<QueryEntityType::kStmt>(i);
        assign_var.emplace_back(*GetModifies(index).begin());
    }

    assign_stmt.assign(assign_stmt_set.begin(), assign_stmt_set.end());

    return {assign_stmt, assign_var};
}
PairVec<StmtNo, VarIndex> ProgramKnowledgeBase::GetPatternPair() {
    assert(compiled);
    return GetModifiesStmtVar(StmtType::kAssign);
}
std::set<StmtNo> ProgramKnowledgeBase::GetPattern(StmtType container_type,
                                                  std::string_view var_name) {
    assert(compiled);
    assert(container_type == StmtType::kWhile ||
           container_type == StmtType::kIf);
    VarIndex var_index = IdentToIndex<QueryEntityType::kVar>(var_name);
    if (var_index == 0) return {};
    return GetPattern(container_type, var_index);
}
std::set<StmtNo> ProgramKnowledgeBase::GetPattern(StmtType container_type,
                                                  VarIndex var_index) {
    assert(compiled);
    assert(container_type == StmtType::kWhile ||
           container_type == StmtType::kIf);
    return uses_rel_.GetContainers(container_type, var_index);
}
PairVec<StmtNo, VarIndex> ProgramKnowledgeBase::GetPatternPairs(
        StmtType container_type) {
    assert(compiled);
    assert(container_type == StmtType::kWhile ||
           container_type == StmtType::kIf);
    return uses_rel_.GetContainerVarPairs(container_type);
}
std::set<StmtNo> ProgramKnowledgeBase::GetPattern(StmtType container_type) {
    assert(compiled);
    assert(container_type == StmtType::kWhile ||
           container_type == StmtType::kIf);
    return uses_rel_.GetContainers(container_type);
}
bool ProgramKnowledgeBase::ExistCalls(Index<ArgPos::kFirst> first_proc,
                                      Index<ArgPos::kSecond> second_proc) {
    assert(compiled);
    return call_proc_.ExistCalls(first_proc, second_proc);
}
bool ProgramKnowledgeBase::ExistCalls(std::string_view first_proc_name,
                                      std::string_view second_proc_name) {
    auto first_proc = IdentToIndex<ArgPos::kFirst>(first_proc_name,
                                                   QueryEntityType::kProc);
    auto second_proc = IdentToIndex<ArgPos::kSecond>(second_proc_name,
                                                     QueryEntityType::kProc);
    if (first_proc == 0 || second_proc == 0) return false;
    return ExistCalls(first_proc, second_proc);
}
bool ProgramKnowledgeBase::ExistCallsT(Index<ArgPos::kFirst> first_proc,
                                       Index<ArgPos::kSecond> second_proc) {
    assert(compiled);
    return call_proc_.ExistCallsT(first_proc, second_proc);
}
bool ProgramKnowledgeBase::ExistCallsT(std::string_view first_proc_name,
                                       std::string_view second_proc_name) {
    auto first_proc = IdentToIndex<ArgPos::kFirst>(first_proc_name,
                                                   QueryEntityType::kProc);
    auto second_proc = IdentToIndex<ArgPos::kSecond>(second_proc_name,
                                                     QueryEntityType::kProc);
    if (first_proc == 0 || second_proc == 0) return false;
    return ExistCallsT(first_proc, second_proc);
}
bool ProgramKnowledgeBase::ExistCalls(Index<ArgPos::kFirst> first_proc) {
    assert(compiled);
    return call_proc_.ExistCalls(first_proc);
}
bool ProgramKnowledgeBase::ExistCalls(Name<ArgPos::kFirst> proc_name) {
    auto first_proc =
            IdentToIndex<ArgPos::kFirst>(proc_name, QueryEntityType::kProc);
    if (first_proc == 0) return false;
    return ExistCalls(first_proc);
}
bool ProgramKnowledgeBase::ExistCalls(Index<ArgPos::kSecond> proc) {
    assert(compiled);
    return !call_proc_.GetStmts(proc).empty();
}
bool ProgramKnowledgeBase::ExistCalls(Name<ArgPos::kSecond> proc_name) {
    auto second_proc =
            IdentToIndex<ArgPos::kSecond>(proc_name, QueryEntityType::kProc);
    if (second_proc == 0) return false;
    return ExistCalls(second_proc);
}
bool ProgramKnowledgeBase::ExistCalls() {
    assert(compiled);
    return !type_stmt_.GetStatements(StmtType::kCall).empty();
}
std::set<ProcIndex> ProgramKnowledgeBase::GetCalls(ArgPos return_pos) {
    assert(compiled);
    return call_proc_.GetCalls(return_pos);
}
std::set<ProcIndex> ProgramKnowledgeBase::GetCalls(Index<ArgPos::kFirst> proc) {
    assert(compiled);
    return call_proc_.GetCalleeProcs(proc);
}
std::set<ProcIndex> ProgramKnowledgeBase::GetCalls(
        Name<ArgPos::kFirst> proc_name) {
    auto first_proc =
            IdentToIndex<ArgPos::kFirst>(proc_name, QueryEntityType::kProc);
    if (first_proc == 0) return {};
    return GetCalls(first_proc);
}
std::set<ProcIndex> ProgramKnowledgeBase::GetCallsT(
        Index<ArgPos::kFirst> first_proc) {
    assert(compiled);
    return call_proc_.GetCalleeProcsT(first_proc.value);
}
std::set<ProcIndex> ProgramKnowledgeBase::GetCallsT(
        Name<ArgPos::kFirst> proc_name) {
    auto first_proc =
            IdentToIndex<ArgPos::kFirst>(proc_name, QueryEntityType::kProc);
    if (first_proc == 0) return {};
    return GetCallsT(first_proc);
}
std::set<ProcIndex> ProgramKnowledgeBase::GetCalls(
        Index<ArgPos::kSecond> proc) {
    assert(compiled);
    return call_proc_.GetCallerProcs(proc);
}
std::set<ProcIndex> ProgramKnowledgeBase::GetCalls(
        Name<ArgPos::kSecond> proc_name) {
    auto second_proc =
            IdentToIndex<ArgPos::kSecond>(proc_name, QueryEntityType::kProc);
    if (second_proc == 0) return {};
    return GetCalls(second_proc);
}
std::set<ProcIndex> ProgramKnowledgeBase::GetCallsT(
        Index<ArgPos::kSecond> proc) {
    assert(compiled);
    return call_proc_.GetCallerProcsT(proc);
}
std::set<ProcIndex> ProgramKnowledgeBase::GetCallsT(
        Name<ArgPos::kSecond> proc_name) {
    auto second_proc =
            IdentToIndex<ArgPos::kSecond>(proc_name, QueryEntityType::kProc);
    if (second_proc == 0) return {};
    return GetCallsT(second_proc);
}
PairVec<ProcIndex> ProgramKnowledgeBase::GetCallsPairs() {
    assert(compiled);
    return call_proc_.GetCallsPairs();
}
PairVec<ProcIndex> ProgramKnowledgeBase::GetCallsTPairs() {
    assert(compiled);
    return call_proc_.GetCallsTPairs();
}
bool ProgramKnowledgeBase::ExistNext(Index<ArgPos::kFirst> first_stmt,
                                     Index<ArgPos::kSecond> second_stmt) {
    return next_->IsNext(first_stmt, second_stmt);
}
bool ProgramKnowledgeBase::ExistNextT(Index<ArgPos::kFirst> first_stmt,
                                      Index<ArgPos::kSecond> second_stmt) {
    return next_->IsNextT(first_stmt, second_stmt);
}
bool ProgramKnowledgeBase::ExistNext(Index<ArgPos::kFirst> first_stmt) {
    return next_->HasNext(first_stmt);
}
bool ProgramKnowledgeBase::ExistNext(Index<ArgPos::kSecond> second_stmt) {
    return next_->HasPrev(second_stmt);
}
bool ProgramKnowledgeBase::ExistNext() { return next_->HasNext(); }
std::set<StmtNo> ProgramKnowledgeBase::GetNext(ArgPos return_pos,
                                               StmtType return_type) {
    return next_->GetNextSingleArg(return_pos, return_type);
}
std::set<StmtNo> ProgramKnowledgeBase::GetNext(Index<ArgPos::kFirst> stmt,
                                               StmtType return_type) {
    return next_->GetNext(stmt, return_type);
}
std::set<StmtNo> ProgramKnowledgeBase::GetNextT(Index<ArgPos::kFirst> stmt,
                                                StmtType return_type) {
    return next_->GetNextT(stmt, return_type);
}
std::set<StmtNo> ProgramKnowledgeBase::GetNext(Index<ArgPos::kSecond> stmt,
                                               StmtType return_type) {
    return next_->GetPrev(stmt, return_type);
}
std::set<StmtNo> ProgramKnowledgeBase::GetNextT(Index<ArgPos::kSecond> stmt,
                                                StmtType return_type) {
    return next_->GetPrevT(stmt, return_type);
}
PairVec<StmtNo> ProgramKnowledgeBase::GetNextPairs(StmtType first_type,
                                                   StmtType second_type) {
    return next_->GetNextPairs(first_type, second_type);
}
PairVec<StmtNo> ProgramKnowledgeBase::GetNextTPairs(StmtType first_type,
                                                    StmtType second_type) {
    return next_->GetNextTPairs(first_type, second_type);
}
std::set<StmtNo> ProgramKnowledgeBase::GetNextTSelf(StmtType type) {
    return next_->GetNextTSelf(type);
}
bool ProgramKnowledgeBase::ExistAffects(Index<ArgPos::kFirst> first_assign,
                                        Index<ArgPos::kSecond> second_assign) {
    return false;
}
bool ProgramKnowledgeBase::ExistAffectsT(Index<ArgPos::kFirst> first_assign,
                                         Index<ArgPos::kSecond> second_assign) {
    return false;
}
bool ProgramKnowledgeBase::ExistAffects(Index<ArgPos::kFirst> first_assign) {
    return false;
}
bool ProgramKnowledgeBase::ExistAffects(Index<ArgPos::kSecond> second_assign) {
    return false;
}
bool ProgramKnowledgeBase::ExistAffects() { return false; }
std::set<StmtNo> ProgramKnowledgeBase::GetAffects(ArgPos return_pos) {
    return {};
}
std::set<StmtNo> ProgramKnowledgeBase::GetAffects(
        Index<ArgPos::kFirst> assign) {
    return {};
}
std::set<StmtNo> ProgramKnowledgeBase::GetAffectsT(
        Index<ArgPos::kFirst> assign) {
    return {};
}
std::set<StmtNo> ProgramKnowledgeBase::GetAffects(
        Index<ArgPos::kSecond> assign) {
    return {};
}
std::set<StmtNo> ProgramKnowledgeBase::GetAffectsT(
        Index<ArgPos::kSecond> assign) {
    return {};
}
PairVec<StmtNo> ProgramKnowledgeBase::GetAffectsPairs() { return {}; }

PairVec<StmtNo> ProgramKnowledgeBase::GetAffectsTPairs() { return {}; }
void ProgramKnowledgeBase::Compile() {
    assert(!compiled);
    container_forest_ = std::make_unique<ContainerForest>(
            stmtlst_parent_, stmtlst_stmt_, stmtlst_count_);
    parent_store_.container_forest_ = container_forest_.get();
    uses_rel_.Compile(type_stmt_,
                      {*container_forest_, stmtlst_parent_, stmtlst_stmt_},
                      call_proc_);
    modifies_rel_.Compile(type_stmt_,
                          {*container_forest_, stmtlst_parent_, stmtlst_stmt_},
                          call_proc_);
    cfg_ = std::make_unique<ControlFlowGraph>(
            stmt_count_, stmtlst_count_,
            ControlFlowGraph::Stores{stmtlst_stmt_, type_stmt_,
                                     stmtlst_parent_});
    next_ = std::make_unique<NextCalculator>(
            NextCalculator::Stores{stmtlst_stmt_, type_stmt_,
                                   *container_forest_, parent_store_, *cfg_});
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
void ProgramKnowledgeBase::ToName(QueryEntityType et,
                                  const std::vector<int> &index_list,
                                  std::list<std::string> &names) {
    assert(compiled);
    switch (et) {
        case QueryEntityType::kProc:
        case QueryEntityType::kVar:
        case QueryEntityType::kConst:
            std::for_each(
                    index_list.begin(), index_list.end(),
                    [this, et, &names](int i) {
                        names.emplace_back(name_value_.GetNameValue(i, et));
                    });
            return;
        case QueryEntityType::kStmt:
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(names),
                           [](int i) { return std::to_string(i); });
            return;
    }
    assert(false);
}
void ProgramKnowledgeBase::ToAttr(StmtType et,
                                  const std::vector<StmtNo> &index_list,
                                  std::list<std::string> &names) {
    switch (et) {
        case StmtType::kRead:
            return ExtractReadModifies(index_list, names);
        case StmtType::kPrint:
            return ExtractPrintUses(index_list, names);
        case StmtType::kCall:
            return ExtractCallProcNames(index_list, names);
        default:
            assert(false);
    }
    assert(false);
}
int ProgramKnowledgeBase::IdentToIndexValue(std::string_view name,
                                            QueryEntityType et) {
    assert(compiled);
    assert(et != QueryEntityType::kStmt);
    return name_value_.GetIndex(name.data(), et);
}
void ProgramKnowledgeBase::ExtractReadModifies(
        const std::vector<int> &read_stmts,
        std::list<std::string> &output) const noexcept {
    for (StmtNo read : read_stmts) {
        auto modify = *modifies_rel_.GetModifies(read).begin();
        auto var_name = name_value_.GetNameValue(modify, QueryEntityType::kVar);
        output.emplace_back(var_name);
    }
}
void ProgramKnowledgeBase::ExtractPrintUses(
        const std::vector<int> &print_stmts,
        std::list<std::string> &output) const noexcept {
    for (StmtNo print : print_stmts) {
        auto use = *uses_rel_.GetUses(print).begin();
        auto var_name = name_value_.GetNameValue(use, QueryEntityType::kVar);
        output.emplace_back(var_name);
    }
}
void ProgramKnowledgeBase::ExtractCallProcNames(
        const std::vector<StmtNo> &call_stmts,
        std::list<std::string> &output) const noexcept {
    for (StmtNo call : call_stmts) {
        auto proc = call_proc_.GetProc(call);
        auto proc_name = name_value_.GetNameValue(proc, QueryEntityType::kProc);
        output.emplace_back(proc_name);
    }
}
}  // namespace spa
