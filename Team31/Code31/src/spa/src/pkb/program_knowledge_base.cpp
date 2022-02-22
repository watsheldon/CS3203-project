#include "program_knowledge_base.h"

#include <algorithm>
#include <cassert>
#include <list>
#include <set>
#include <vector>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "knowledge_base.h"
#include "secondary_structure/container_forest.h"

namespace spa {
ProgramKnowledgeBase::ProgramKnowledgeBase(BasicEntities init)
        : stmt_count_(init.assigns.size() + init.ifs.size() +
                      init.whiles.size() + init.calls.size() +
                      init.reads.size() + init.prints.size() - STMT_TYPE_COUNT),
          stmtlst_count_(init.procedures.size() + init.whiles.size() +
                         init.ifs.size() * 2 - 4),
          call_proc_(stmt_count_, init.procedures.size() - 1),
          stmtlst_parent_(init.procedures.size() - 1, stmt_count_,
                          stmtlst_count_),
          stmtlst_stmt_(stmtlst_count_, stmt_count_),
          modifies_rel_(stmt_count_, init.variables.size() - 1),
          uses_rel_(stmt_count_, init.variables.size() - 1),
          polish_notation_(stmt_count_, std::move(init.notations)),
          proc_name_(std::move(init.procedures)),
          var_name_(std::move(init.variables)),
          const_value_(std::move(init.constants)),
          type_stmt_(stmt_count_, std::move(init.reads), std::move(init.prints),
                     std::move(init.calls), std::move(init.whiles),
                     std::move(init.ifs), std::move(init.assigns)) {}

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
    return stmtlst_stmt_.ExistFollows(transitive, first_stmt, second_stmt);
}
bool ProgramKnowledgeBase::ExistFollows(Index<ArgPos::kFirst> first_stmt) {
    assert(compiled);
    return stmtlst_stmt_.ExistFollows(first_stmt);
}
bool ProgramKnowledgeBase::ExistFollows(Index<ArgPos::kSecond> second_stmt) {
    assert(compiled);
    return stmtlst_stmt_.ExistFollows(second_stmt);
}
bool ProgramKnowledgeBase::ExistFollows() {
    assert(compiled);
    return stmtlst_stmt_.ExistFollows();
}

bool ProgramKnowledgeBase::ExistParent(bool transitive,
                                       Index<ArgPos::kFirst> parent_stmt,
                                       Index<ArgPos::kSecond> child_stmt) {
    assert(compiled);
    int parent = parent_stmt.value;
    int child = child_stmt.value;
    if (child >= parent || parent > stmt_count_ - 1 || child > stmt_count_) {
        return false;
    }
    if (type_stmt_.GetType(parent) != StmtType::kWhile ||
        type_stmt_.GetType(parent) != StmtType::kIf) {
        return false;
    }

    if (transitive) {
        std::vector<int> parent_follower =
                stmtlst_stmt_.GetFollows(false, parent_stmt);
        return parent_follower.empty() || child_stmt.value < parent_follower[0];
    }
    int stmtlst_index = stmtlst_stmt_.GetStmtlst(child);
    return stmtlst_parent_.GetWhileStmtLst(parent) == stmtlst_index ||
           stmtlst_parent_.GetIfStmtLst(parent).then_index == stmtlst_index ||
           stmtlst_parent_.GetIfStmtLst(parent).else_index == stmtlst_index;
}
bool ProgramKnowledgeBase::ExistParent(Index<ArgPos::kFirst> parent_stmt) {
    assert(compiled);
    return parent_stmt.value < stmt_count_ &&
           (type_stmt_.GetType(parent_stmt.value) == StmtType::kIf ||
            type_stmt_.GetType(parent_stmt.value) == StmtType::kWhile);
}
bool ProgramKnowledgeBase::ExistParent(Index<ArgPos::kSecond> child_stmt) {
    assert(compiled);
    int stmtlst_index = stmtlst_stmt_.GetStmtlst(child_stmt.value);
    return child_stmt.value <= stmt_count_ &&
           (stmtlst_parent_.GetParent(stmtlst_index).type == PType::kIf ||
            stmtlst_parent_.GetParent(stmtlst_index).type == PType::kWhile);
}
bool ProgramKnowledgeBase::ExistParent() {
    assert(compiled);
    return !type_stmt_.GetStatements(StmtType::kWhile).empty() ||
           !type_stmt_.GetStatements(StmtType::kIf).empty();
}

std::set<int> ProgramKnowledgeBase::GetFollows(ArgPos return_pos,
                                               StmtType return_type) {
    auto results = return_pos == ArgPos::kFirst
                           ? stmtlst_stmt_.GetFollowedByWildcard()
                           : stmtlst_stmt_.GetFollowsWildcard();
    std::set<int> filtered_results;
    std::copy_if(results.begin(), results.end(),
                 std::inserter(filtered_results, filtered_results.end()),
                 [this, return_type](int i) {
                     return type_stmt_.GetType(i) == return_type;
                 });
    return filtered_results;
}
std::set<int> ProgramKnowledgeBase::GetFollows(bool transitive,
                                               Index<ArgPos::kFirst> first_stmt,
                                               StmtType return_type) {
    assert(compiled);
    assert(first_stmt.value > 0);
    std::vector<int> results = stmtlst_stmt_.GetFollows(transitive, first_stmt);
    if (return_type == StmtType::kAll) {
        return {results.begin(), results.end()};
    }
    std::set<int> filtered_results;
    std::copy_if(results.begin(), results.end(),
                 std::inserter(filtered_results, filtered_results.end()),
                 [this, return_type](int i) {
                     return type_stmt_.GetType(i) == return_type;
                 });
    return filtered_results;
}

std::set<int> ProgramKnowledgeBase::GetFollows(
        bool transitive, Index<ArgPos::kSecond> second_stmt,
        StmtType return_type) {
    assert(compiled);
    assert(second_stmt.value > 0);
    std::vector<int> results =
            stmtlst_stmt_.GetFollows(transitive, second_stmt);
    if (return_type == StmtType::kAll) {
        return {results.begin(), results.end()};
    }
    std::set<int> filtered_results;
    std::copy_if(results.begin(), results.end(),
                 std::inserter(filtered_results, filtered_results.end()),
                 [this, return_type](int i) {
                     return type_stmt_.GetType(i) == return_type;
                 });
    return filtered_results;
}

std::pair<std::vector<int>, std::vector<int>>
ProgramKnowledgeBase::GetFollowsPairs(bool transitive, StmtType first_type,
                                      StmtType second_type) {
    assert(compiled);
    std::pair<std::vector<int>, std::vector<int>> results =
            stmtlst_stmt_.GetFollowsPairs(transitive);
    if (first_type == StmtType::kAll && second_type == StmtType::kAll) {
        return results;
    }
    std::pair<std::vector<int>, std::vector<int>> filtered_results;
    for (int i = 0; i < results.first.size(); ++i) {
        if ((first_type == StmtType::kAll ||
             type_stmt_.GetType(results.first[i]) == first_type) &&
            (second_type == StmtType::kAll ||
             type_stmt_.GetType(results.second[i]) == second_type)) {
            filtered_results.first.emplace_back(results.first[i]);
            filtered_results.second.emplace_back(results.second[i]);
        }
    }
    return filtered_results;
}

std::set<int> ProgramKnowledgeBase::GetAllParents(StmtType return_type) {
    std::vector<int> results;
    if (return_type == StmtType::kAll) {
        results = type_stmt_.GetStatements(StmtType::kWhile);
        std::vector<int> if_stmts = type_stmt_.GetStatements(StmtType::kIf);
        results.insert(results.end(), if_stmts.begin(), if_stmts.end());
    } else {
        results = type_stmt_.GetStatements(return_type);
    }
    return {results.begin(), results.end()};
}
std::set<int> ProgramKnowledgeBase::GetAllChildren(StmtType return_type) {
    if (return_type == StmtType::kAll) {
        std::vector<int> results;
        for (auto &while_stmt : type_stmt_.GetStatements(StmtType::kWhile)) {
            int stmtlst_index = stmtlst_parent_.GetWhileStmtLst(while_stmt);
            std::vector<int> stmtlst =
                    stmtlst_stmt_.GetStatements(stmtlst_index);
            results.insert(results.end(), stmtlst.begin(), stmtlst.end());
        }
        for (auto &if_stmt : type_stmt_.GetStatements(StmtType::kIf)) {
            int then_index = stmtlst_parent_.GetIfStmtLst(if_stmt).then_index;
            int else_index = stmtlst_parent_.GetIfStmtLst(if_stmt).else_index;
            std::vector<int> then_stmtlst =
                    stmtlst_stmt_.GetStatements(then_index);
            std::vector<int> else_stmtlst =
                    stmtlst_stmt_.GetStatements(else_index);
            results.insert(results.end(), then_stmtlst.begin(),
                           then_stmtlst.end());
            results.insert(results.end(), else_stmtlst.begin(),
                           else_stmtlst.end());
        }
        return {results.begin(), results.end()};
    }
    std::vector<int> results = type_stmt_.GetStatements(return_type);
    std::vector<int> filtered_results;
    for (auto &i : results) {
        int stmtlst_index = stmtlst_stmt_.GetStmtlst(i);
        PType type = stmtlst_parent_.GetParent(stmtlst_index).type;
        if (type != PType::kIf && type != PType::kWhile) continue;
        filtered_results.emplace_back(i);
    }
    return {filtered_results.begin(), filtered_results.end()};
}
std::set<int> ProgramKnowledgeBase::GetParent(ArgPos return_pos,
                                              StmtType return_type) {
    if (return_pos == ArgPos::kFirst) {
        return GetAllParents(return_type);
    }
    return GetAllChildren(return_type);
}
void ProgramKnowledgeBase::GetNonTransitiveParentFirst(
        StmtType parent_type, int parent, std::vector<int> &results) const {
    if (parent_type == StmtType::kWhile) {
        int stmtlst_index = stmtlst_parent_.GetWhileStmtLst(parent);
        results = stmtlst_stmt_.GetStatements(stmtlst_index);
    }
    int then_index = stmtlst_parent_.GetIfStmtLst(parent).then_index;
    int else_index = stmtlst_parent_.GetIfStmtLst(parent).else_index;
    results = stmtlst_stmt_.GetStatements(then_index);
    std::vector<int> else_vector = stmtlst_stmt_.GetStatements(else_index);
    results.insert(results.end(), else_vector.begin(), else_vector.end());
}
void ProgramKnowledgeBase::GetTransitiveParentFirst(
        std::vector<int> parent_follower, int parent,
        std::vector<int> &results) const {
    int size;
    if (parent_follower.empty()) {
        size = static_cast<int>(stmt_count_) - parent;
    } else {
        size = parent_follower[0] - parent - 1;
    }
    results.resize(size);
    std::iota(results.begin(), results.end(), parent + 1);
}
std::set<int> ProgramKnowledgeBase::GetParent(bool transitive,
                                              Index<ArgPos::kFirst> parent_stmt,
                                              StmtType return_type) {
    assert(compiled);
    assert(parent_stmt.value > 0);
    if (parent_stmt.value > stmt_count_ - 1) {
        return {};
    }
    StmtType parent_type = type_stmt_.GetType(parent_stmt.value);
    if (parent_type != StmtType::kWhile && parent_type != StmtType::kIf) {
        return {};
    }

    std::vector<int> results;
    if (transitive) {
        GetTransitiveParentFirst(stmtlst_stmt_.GetFollows(false, parent_stmt),
                                 parent_stmt.value, results);
    } else {
        GetNonTransitiveParentFirst(parent_type, parent_stmt.value, results);
    }
    if (return_type == StmtType::kAll) {
        return {results.begin(), results.end()};
    }
    std::vector<int> filtered_results;
    std::copy_if(results.begin(), results.end(),
                 std::back_inserter(filtered_results),
                 [this, return_type](int i) {
                     return type_stmt_.GetType(i) == return_type;
                 });
    return {filtered_results.begin(), filtered_results.end()};
}
std::set<int> ProgramKnowledgeBase::GetParent(bool transitive,
                                              Index<ArgPos::kSecond> child_stmt,
                                              StmtType return_type) {
    assert(compiled);
    assert(child_stmt.value > 0);
    assert(return_type == StmtType::kAll || return_type == StmtType::kWhile ||
           return_type == StmtType::kIf);
    if (child_stmt.value > stmt_count_) {
        return {};
    }

    int stmtlst_index = stmtlst_stmt_.GetStmtlst(child_stmt.value);
    PType type = stmtlst_parent_.GetParent(stmtlst_index).type;
    if (type != PType::kIf && type != PType::kWhile) {
        return {};
    }
    std::vector<int> results;
    int direct_parent = stmtlst_parent_.GetParent(stmtlst_index).index;
    if (transitive) {
        std::vector<int> stmtlsts =
                container_forest_->GetParents(stmtlst_index);
        for (auto &i : stmtlsts) {
            if (stmtlst_parent_.GetParent(i).type != PType::kWhile &&
                stmtlst_parent_.GetParent(i).type != PType::kIf)
                continue;
            int indirect_parent = stmtlst_parent_.GetParent(i).index;
            results.emplace_back(indirect_parent);
        }
        results.emplace_back(direct_parent);
    } else {
        results = {direct_parent};
    }

    if (return_type == StmtType::kAll) {
        return {results.begin(), results.end()};
    }
    std::vector<int> filtered_results;
    std::copy_if(results.begin(), results.end(),
                 std::back_inserter(filtered_results),
                 [this, return_type](int i) {
                     return type_stmt_.GetType(i) == return_type;
                 });
    return {filtered_results.begin(), filtered_results.end()};
}

void ProgramKnowledgeBase::GetTransitiveParentPairs(
        std::pair<std::vector<int>, std::vector<int>> &results) {
    for (auto &while_stmt : type_stmt_.GetStatements(StmtType::kWhile)) {
        std::vector<int> follower = stmtlst_stmt_.GetFollows(
                false, Index<ArgPos::kFirst>(while_stmt));
        int end;
        (follower.empty()) ? (end = static_cast<int>(stmt_count_) + 1)
                           : (end = follower[0]);
        for (int i = while_stmt + 1; i < end; ++i) {
            results.first.emplace_back(while_stmt);
            results.second.emplace_back(i);
        }
    }
    for (auto &if_stmt : type_stmt_.GetStatements(StmtType::kIf)) {
        std::vector<int> follower =
                stmtlst_stmt_.GetFollows(false, Index<ArgPos::kFirst>(if_stmt));
        int end;
        (follower.empty()) ? (end = static_cast<int>(stmt_count_) + 1)
                           : (end = follower[0]);
        for (int i = if_stmt + 1; i < end; ++i) {
            results.first.emplace_back(if_stmt);
            results.second.emplace_back(i);
        }
    }
}
void ProgramKnowledgeBase::GetNonTransitiveParentPairs(
        std::pair<std::vector<int>, std::vector<int>> &results) {
    for (auto &while_stmt : type_stmt_.GetStatements(StmtType::kWhile)) {
        int stmtlst_index = stmtlst_parent_.GetWhileStmtLst(while_stmt);
        std::vector<int> stmtlst = stmtlst_stmt_.GetStatements(stmtlst_index);
        for (auto &child_stmt : stmtlst) {
            results.first.emplace_back(while_stmt);
            results.second.emplace_back(child_stmt);
        }
    }
    for (auto &if_stmt : type_stmt_.GetStatements(StmtType::kIf)) {
        int then_index = stmtlst_parent_.GetIfStmtLst(if_stmt).then_index;
        int else_index = stmtlst_parent_.GetIfStmtLst(if_stmt).else_index;
        std::vector<int> then_stmtlst = stmtlst_stmt_.GetStatements(then_index);
        std::vector<int> else_stmtlst = stmtlst_stmt_.GetStatements(else_index);
        for (auto &child_stmt : then_stmtlst) {
            results.first.emplace_back(if_stmt);
            results.second.emplace_back(child_stmt);
        }
        for (auto &child_stmt : else_stmtlst) {
            results.first.emplace_back(if_stmt);
            results.second.emplace_back(child_stmt);
        }
    }
}
std::pair<std::vector<int>, std::vector<int>>
ProgramKnowledgeBase::GetParentPairs(bool transitive, StmtType parent_type,
                                     StmtType child_type) {
    std::pair<std::vector<int>, std::vector<int>> results;
    if (transitive) {
        GetTransitiveParentPairs(results);
    } else {
        GetNonTransitiveParentPairs(results);
    }
    if (parent_type == StmtType::kAll && child_type == StmtType::kAll) {
        return results;
    }
    std::pair<std::vector<int>, std::vector<int>> filtered_results;
    for (int i = 0; i < results.first.size(); ++i) {
        if ((parent_type == StmtType::kAll ||
             type_stmt_.GetType(results.first[i]) == parent_type) &&
            (child_type == StmtType::kAll ||
             type_stmt_.GetType(results.second[i]) == child_type)) {
            filtered_results.first.emplace_back(results.first[i]);
            filtered_results.second.emplace_back(results.second[i]);
        }
    }
    return filtered_results;
}

bool ProgramKnowledgeBase::ExistModifies(int stmt_no, int var_index) {
    assert(compiled);
    return modifies_rel_.GetVarIndex(stmt_no) == var_index;
}

bool ProgramKnowledgeBase::ExistUses(int stmt_no, int var_index) {
    assert(compiled);
    const std::vector<int> &used_vars = uses_rel_.GetVarIndex(stmt_no);
    return std::binary_search(used_vars.begin(), used_vars.end(), var_index);
}

std::vector<int> ProgramKnowledgeBase::GetModifies(
        Index<QueryEntityType::kStmt> stmt_no,
        const std::vector<int> &filtered_var) {
    assert(compiled);
    std::vector<int> results;
    int modified_var = modifies_rel_.GetVarIndex(stmt_no.value);
    if (filtered_var.empty() ||
        std::binary_search(filtered_var.begin(), filtered_var.end(),
                           modified_var)) {
        results.emplace_back(modified_var);
    }
    return results;
}

std::vector<int> ProgramKnowledgeBase::GetModifies(
        Index<QueryEntityType::kVar> var_index,
        const std::vector<int> &filtered_stmt) {
    assert(compiled);
    auto stmts = modifies_rel_.GetStmtNo(var_index.value);
    if (filtered_stmt.empty()) {
        return stmts;
    }
    std::vector<int> results;
    std::set_intersection(stmts.begin(), stmts.end(), filtered_stmt.begin(),
                          filtered_stmt.end(), std::back_inserter(results));
    return results;
}

std::vector<int> ProgramKnowledgeBase::GetUses(
        Index<QueryEntityType::kStmt> stmt_no,
        const std::vector<int> &filtered_var) {
    assert(compiled);
    const std::vector<int> &used_vars = uses_rel_.GetVarIndex(stmt_no.value);
    if (filtered_var.empty()) {
        return used_vars;
    }
    std::vector<int> results;
    std::set_intersection(used_vars.begin(), used_vars.end(),
                          filtered_var.begin(), filtered_var.end(),
                          std::back_inserter(results));
    return results;
}

std::vector<int> ProgramKnowledgeBase::GetUses(
        Index<QueryEntityType::kVar> var_index,
        const std::vector<int> &filtered_stmt) {
    assert(compiled);
    const std::vector<int> &stmts = uses_rel_.GetStmtNo(var_index.value);
    if (filtered_stmt.empty()) {
        return stmts;
    }
    std::vector<int> results;
    std::set_intersection(stmts.begin(), stmts.end(), filtered_stmt.begin(),
                          filtered_stmt.end(), std::back_inserter(results));
    return results;
}

void ProgramKnowledgeBase::Compile() {
    assert(!compiled);
    container_forest_ = std::make_unique<ContainerForest>(
            stmtlst_parent_, stmtlst_stmt_, stmt_count_);
    compiled = true;
}

std::vector<int> ProgramKnowledgeBase::GetAllEntityIndices(QueryEntityType et) {
    assert(compiled);
    std::vector<int> results;
    switch (et) {
        case QueryEntityType::kProc:
            results.resize(proc_name_.size());
        case QueryEntityType::kVar:
            results.resize(var_name_.size());
        case QueryEntityType::kConst:
            results.resize(const_value_.size());
        case QueryEntityType::kStmt:
            results.resize(stmt_count_);
        default:
            results.resize(0);
    }
    std::iota(results.begin(), results.end(), 1);
    return results;
}
std::vector<int> ProgramKnowledgeBase::GetAllEntityIndices(StmtType st) {
    assert(compiled);
    return type_stmt_.GetStatements(st);
}

void ProgramKnowledgeBase::IndexToName(QueryEntityType et,
                                       const std::vector<int> &index_list,
                                       std::list<std::string> &names) {
    assert(compiled);
    switch (et) {
        case QueryEntityType::kProc:
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(names),
                           [this](int i) { return proc_name_.GetName(i); });

        case QueryEntityType::kVar:
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(names),
                           [this](int i) { return var_name_.GetName(i); });
        case QueryEntityType::kConst:
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(names),
                           [this](int i) { return const_value_.GetValue(i); });
        default:
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(names),
                           [](int i) { return std::to_string(i); });
    }
}

bool ProgramKnowledgeBase::ContainsUnseenVarConst(
        const std::vector<QueryToken> &tokens) {
    for (const auto &token : tokens) {
        switch (token.type) {
            case QueryTokenType::WORD: {
                if (var_name_.GetIndex(token.value) == 0) {
                    return true;
                }
                break;
            }
            case QueryTokenType::INTEGER: {
                if (const_value_.GetIndex(token.value) == 0) {
                    return true;
                }
                break;
            }
            default:
                break;
        }
    }
    return false;
}

PolishNotation ProgramKnowledgeBase::ConvertFromQueryTokens(
        const std::vector<QueryToken> &tokens) {
    std::vector<PolishNotationNode> expr;
    for (const auto &token : tokens) {
        switch (token.type) {
            case QueryTokenType::WORD: {
                int var_index = var_name_.GetIndex(token.value);
                assert(var_index > 0);
                PolishNotationNode node(ExprNodeType::kVariable, var_index);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::INTEGER: {
                int const_index = const_value_.GetIndex(token.value);
                assert(const_index > 0);
                PolishNotationNode node(ExprNodeType::kConstant, const_index);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::PLUS: {
                PolishNotationNode node(OperatorType::kPlus);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::MINUS: {
                PolishNotationNode node(OperatorType::kMinus);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::TIMES: {
                PolishNotationNode node(OperatorType::kTimes);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::DIVIDE: {
                PolishNotationNode node(OperatorType::kDivide);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::MODULO: {
                PolishNotationNode node(OperatorType::kModulo);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::LEFTBRACKET: {
                PolishNotationNode node(ExprNodeType::kBracketL);
                expr.emplace_back(node);
                break;
            }
            case QueryTokenType::RIGHTBRACKET: {
                PolishNotationNode node(ExprNodeType::kBracketR);
                expr.emplace_back(node);
                break;
            }
            default:
                assert(false);
        }
    }
    return PolishNotation(expr);
}

}  // namespace spa
