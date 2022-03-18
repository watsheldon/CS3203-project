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
          follows_parent_rel_(stmt_count_),
          modifies_rel_(stmt_count_, init.variables.size() - 1),
          uses_rel_(stmt_count_, init.variables.size() - 1),
          call_proc_(stmt_count_, init.procedures.size() - 1,
                     std::move(init.proc_call_graph)),
          polish_notation_(stmt_count_, std::move(init.notations)),
          name_value_(std::move(init.procedures), std::move(init.variables),
                      std::move(init.constants)),
          type_stmt_(std::move(init.reads), std::move(init.prints),
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
    StmtType type = type_stmt_.GetType(stmt_no);

    if (type == StmtType::kPrint || type == StmtType::kCall) {
        return false;
    }

    if (type == StmtType::kAssign || type == StmtType::kRead) {
        return var_index == kWildCard ||
               var_index == modifies_rel_.GetVarIndex(stmt_no);
    }

    int first_stmt = stmt_no + 1;
    int last_stmt = GetContainerLastStmt(type, stmt_no);

    if (var_index == kWildCard) {
        const auto &assign_stmts = type_stmt_.GetStatements(StmtType::kAssign);
        auto first_assign = std::lower_bound(assign_stmts.begin(),
                                             assign_stmts.end(), first_stmt);
        if (first_assign != assign_stmts.end() && *first_assign <= last_stmt) {
            return true;
        }

        const auto &read_stmts = type_stmt_.GetStatements(StmtType::kRead);
        auto first_read = std::lower_bound(read_stmts.begin(), read_stmts.end(),
                                           first_stmt);
        if (first_read != read_stmts.end() && *first_read <= last_stmt) {
            return true;
        }

        return false;
    }

    auto modifying_stmt = modifies_rel_.GetStmtNo(var_index);
    return std::any_of(modifying_stmt.begin(), modifying_stmt.end(),
                       [first_stmt, last_stmt](auto i) {
                           return first_stmt <= i && i <= last_stmt;
                       });
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

    StmtType type = type_stmt_.GetType(stmt_no);

    if (type == StmtType::kRead || type == StmtType::kCall) {
        return false;
    }

    if (type == StmtType::kAssign || type == StmtType::kPrint) {
        std::vector<int> var_indices;
        var_indices = uses_rel_.GetVarIndex(stmt_no);
        if (var_index == kWildCard) {
            return !var_indices.empty();
        }

        return std::binary_search(var_indices.begin(), var_indices.end(),
                                  var_index);
    }

    int first_stmt = stmt_no;
    int last_stmt = GetContainerLastStmt(type, stmt_no);

    if (var_index == kWildCard) {
        return true;
    }

    auto using_stmt = uses_rel_.GetStmtNo(var_index);
    return std::any_of(using_stmt.begin(), using_stmt.end(),
                       [first_stmt, last_stmt](auto i) {
                           return first_stmt <= i && i <= last_stmt;
                       });
}

bool ProgramKnowledgeBase::ExistUses(int stmt_no, std::string_view var_name) {
    auto var_index = IdentToIndex<QueryEntityType::kVar>(var_name);
    if (var_index == 0) return false;
    return ExistUses(stmt_no, var_index);
}

std::set<int> ProgramKnowledgeBase::GetModifies(
        Index<QueryEntityType::kStmt> stmt_no) {
    assert(compiled);
    assert(stmt_no.value != 0);
    if (stmt_no.value > stmt_count_) {
        return {};
    }

    StmtType type = type_stmt_.GetType(stmt_no.value);

    if (type == StmtType::kPrint || type == StmtType::kCall) {
        return {};
    }

    if (type == StmtType::kAssign || type == StmtType::kRead) {
        return {modifies_rel_.GetVarIndex(stmt_no.value)};
    }

    std::set<int> results;
    int first_stmt = stmt_no.value + 1;
    int last_stmt = GetContainerLastStmt(type, stmt_no.value);
    auto assign_bound = GetStmtBound(StmtType::kAssign, first_stmt, last_stmt);
    AppendVarIndicesModifies(assign_bound, results);
    auto read_bound = GetStmtBound(StmtType::kRead, first_stmt, last_stmt);
    AppendVarIndicesModifies(read_bound, results);

    return results;
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
    if (type == StmtType::kPrint || type == StmtType::kCall) {
        return {};
    }

    auto direct_modifying_stmt = modifies_rel_.GetStmtNo(var_index.value);

    if (type == StmtType::kAssign) {
        std::set<int> assign_stmt;
        std::copy_if(direct_modifying_stmt.begin(), direct_modifying_stmt.end(),
                     std::inserter(assign_stmt, assign_stmt.begin()),
                     [this](int i) {
                         return type_stmt_.GetType(i) == StmtType::kAssign;
                     });
        return assign_stmt;
    }

    if (type == StmtType::kRead) {
        std::set<int> read_stmt;
        std::copy_if(direct_modifying_stmt.begin(), direct_modifying_stmt.end(),
                     std::inserter(read_stmt, read_stmt.begin()),
                     [this](int i) {
                         return type_stmt_.GetType(i) == StmtType::kRead;
                     });
        return read_stmt;
    }

    std::set<int> container_stmt;

    for (int i : direct_modifying_stmt) {
        auto parents =
                GetParent(true, Index<ArgPos::kSecond>(i), StmtType::kAll);
        container_stmt.insert(parents.begin(), parents.end());
    }

    if (type == StmtType::kAll) {
        container_stmt.insert(direct_modifying_stmt.begin(),
                              direct_modifying_stmt.end());
        return container_stmt;
    }

    if (type == StmtType::kIf) {
        std::set<int> if_stmt;
        std::copy_if(container_stmt.begin(), container_stmt.end(),
                     std::inserter(if_stmt, if_stmt.begin()), [this](int i) {
                         return type_stmt_.GetType(i) == StmtType::kIf;
                     });
        return if_stmt;
    }

    assert(type == StmtType::kWhile);

    std::set<int> while_stmt;
    std::copy_if(container_stmt.begin(), container_stmt.end(),
                 std::inserter(while_stmt, while_stmt.begin()), [this](int i) {
                     return type_stmt_.GetType(i) == StmtType::kWhile;
                 });

    return while_stmt;
}

std::set<int> ProgramKnowledgeBase::GetModifies(StmtType type) {
    assert(compiled);

    if (type == StmtType::kPrint || type == StmtType::kCall) {
        return {};
    }

    if (type == StmtType::kAssign) {
        auto assign_stmt = type_stmt_.GetStatements(StmtType::kAssign);
        return {assign_stmt.begin(), assign_stmt.end()};
    }

    if (type == StmtType::kRead) {
        auto read_stmt = type_stmt_.GetStatements(StmtType::kRead);
        return {read_stmt.begin(), read_stmt.end()};
    }

    std::set<int> container_stmt;
    std::vector<int> direct_modifying_stmt;

    direct_modifying_stmt = type_stmt_.GetStatements(StmtType::kAssign);
    auto read_stmt = type_stmt_.GetStatements(StmtType::kRead);
    direct_modifying_stmt.insert(direct_modifying_stmt.end(), read_stmt.begin(),
                                 read_stmt.end());

    for (auto &i : direct_modifying_stmt) {
        auto parents =
                GetParent(true, Index<ArgPos::kSecond>(i), StmtType::kAll);
        container_stmt.insert(parents.begin(), parents.end());
    }

    if (type == StmtType::kAll) {
        container_stmt.insert(direct_modifying_stmt.begin(),
                              direct_modifying_stmt.end());
        return container_stmt;
    }

    if (type == StmtType::kIf) {
        std::set<int> if_stmt;
        std::copy_if(container_stmt.begin(), container_stmt.end(),
                     std::inserter(if_stmt, if_stmt.begin()), [this](int i) {
                         return type_stmt_.GetType(i) == StmtType::kIf;
                     });
        return if_stmt;
    }

    assert(type == StmtType::kWhile);
    std::set<int> while_stmt;
    std::copy_if(container_stmt.begin(), container_stmt.end(),
                 std::inserter(while_stmt, while_stmt.begin()), [this](int i) {
                     return type_stmt_.GetType(i) == StmtType::kWhile;
                 });
    return while_stmt;
}

PairVec<int> ProgramKnowledgeBase::GetModifiesStmtVar(StmtType type) {
    assert(compiled);

    if (type == StmtType::kPrint || type == StmtType::kCall) {
        return {{}, {}};
    }

    if (type == StmtType::kAssign) {
        std::vector<int> assign_stmt =
                type_stmt_.GetStatements(StmtType::kAssign);
        std::vector<int> assign_var;

        assign_var.reserve(assign_stmt.size());
        for (int i : assign_stmt) {
            assign_var.emplace_back(modifies_rel_.GetVarIndex(i));
        }

        return {assign_stmt, assign_var};
    }

    if (type == StmtType::kRead) {
        std::vector<int> read_stmt = type_stmt_.GetStatements(StmtType::kRead);
        std::vector<int> read_var;
        read_var.resize(read_stmt.size());
        for (int i : read_stmt) {
            read_var.emplace_back(modifies_rel_.GetVarIndex(i));
        }

        return {read_stmt, read_var};
    }

    std::set<int> container_stmt;
    std::vector<int> direct_modifying_stmt;

    direct_modifying_stmt = type_stmt_.GetStatements(StmtType::kAssign);
    auto read_stmt = type_stmt_.GetStatements(StmtType::kRead);
    direct_modifying_stmt.insert(direct_modifying_stmt.end(), read_stmt.begin(),
                                 read_stmt.end());

    if (type == StmtType::kIf) {
        std::vector<int> if_stmt;
        std::vector<int> if_var;

        for (int i : direct_modifying_stmt) {
            int var_index = modifies_rel_.GetVarIndex(i);
            int stmtlst = stmtlst_stmt_.GetStmtlst(i);
            auto parents = container_forest_->GetAncestryTrace(stmtlst);
            for (int j : parents) {
                auto stmt = stmtlst_parent_.GetParent(j);
                if (stmt.type == StmtlstParentStore::kIf) {
                    if_stmt.emplace_back(stmt.index);
                    if_var.emplace_back(var_index);
                }
            }
        }
        return {if_stmt, if_var};
    }

    if (type == StmtType::kWhile) {
        std::vector<int> while_stmt;
        std::vector<int> while_var;

        for (auto i : direct_modifying_stmt) {
            int var_index = modifies_rel_.GetVarIndex(i);
            int stmtlst = stmtlst_stmt_.GetStmtlst(i);
            auto parents = container_forest_->GetAncestryTrace(stmtlst);
            for (auto &j : parents) {
                auto stmt = stmtlst_parent_.GetParent(j);
                if (stmt.type == StmtlstParentStore::kWhile) {
                    while_stmt.emplace_back(stmt.index);
                    while_var.emplace_back(var_index);
                }
            }
        }
        return {while_stmt, while_var};
    }

    assert(type == StmtType::kAll);

    std::vector<int> all_stmt;
    std::vector<int> all_var;

    for (auto i : direct_modifying_stmt) {
        all_stmt.emplace_back(i);
        int var_index = modifies_rel_.GetVarIndex(i);
        all_var.emplace_back(var_index);
        int stmtlst = stmtlst_stmt_.GetStmtlst(i);

        auto parents = container_forest_->GetAncestryTrace(stmtlst);

        for (auto j : parents) {
            auto stmt = stmtlst_parent_.GetParent(j);
            if (stmt.type != PType::kWhile && stmt.type != PType::kIf) {
                continue;
            }
            all_stmt.emplace_back(stmt.index);
            all_var.emplace_back(var_index);
        }
    }
    return {all_stmt, all_var};
}

std::set<int> ProgramKnowledgeBase::GetUses(
        Index<QueryEntityType::kStmt> stmt_no) {
    assert(compiled);
    assert(stmt_no.value != 0);
    if (stmt_no.value > stmt_count_) {
        return {};
    }
    StmtType type = type_stmt_.GetType(stmt_no.value);

    if (type == StmtType::kRead || type == StmtType::kCall) {
        return {};
    }

    if (type == StmtType::kAssign || type == StmtType::kPrint) {
        std::vector<int> var_indices;
        var_indices = uses_rel_.GetVarIndex(stmt_no.value);

        return {var_indices.begin(), var_indices.end()};
    }

    std::set<int> results;
    auto conditional_var = uses_rel_.GetVarIndex(stmt_no.value);
    results.insert(conditional_var.begin(), conditional_var.end());
    std::vector<StmtType> stmtType = {StmtType::kAssign, StmtType::kPrint,
                                      StmtType::kWhile, StmtType::kIf};
    int first_stmt = stmt_no.value + 1;
    int last_stmt = GetContainerLastStmt(type, stmt_no.value);

    for (auto i : stmtType) {
        auto bound = GetStmtBound(i, first_stmt, last_stmt);
        AppendVarIndicesUses(bound, results);
    }

    return results;
}
std::set<int> ProgramKnowledgeBase::GetUses(
        Index<QueryEntityType::kVar> var_index, StmtType type) {
    assert(compiled);
    assert(var_index.value != 0);

    if (type == StmtType::kRead || type == StmtType::kCall) {
        return {};
    }

    auto direct_uses_stmt = uses_rel_.GetAllStmt(var_index.value);
    if (type == StmtType::kAll) {
        return direct_uses_stmt;
    }

    for (auto it = direct_uses_stmt.begin(); it != direct_uses_stmt.end();) {
        if (type_stmt_.GetType(*it) != type) {
            it = direct_uses_stmt.erase(it);
        } else {
            ++it;
        }
    }
    return direct_uses_stmt;
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
    uses_rel_.Compile(type_stmt_, *container_forest_, stmtlst_parent_,
                      stmtlst_stmt_);
    follows_parent_rel_.Compile(
            std::make_unique<TypeStatementsStore>(type_stmt_),
            std::make_unique<ContainerForest>(*container_forest_),
            std::make_unique<StmtlstParentStore>(stmtlst_parent_),
            std::make_unique<StmtlstStatementsStore>(stmtlst_stmt_));
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

int ProgramKnowledgeBase::GetContainerLastStmt(StmtType type, int stmt_no) {
    int last_stmt;
    auto follow_vec =
            GetFollows(false, Index<ArgPos::kFirst>(stmt_no), StmtType::kAll);

    if (follow_vec.size() == 1) {
        last_stmt = *follow_vec.begin() - 1;
        return last_stmt;
    }

    int stmtlst = type == StmtType::kWhile
                          ? stmtlst_parent_.GetWhileStmtLst(stmt_no)
                          : stmtlst_parent_.GetIfStmtLst(stmt_no).else_index;
    int grandchild = container_forest_->GetRightmostGrandchild(stmtlst);
    auto grandchild_stmt = stmtlst_stmt_.GetStatements(grandchild);
    last_stmt = grandchild_stmt.back();

    return last_stmt;
}

Pair<std::vector<int>::const_iterator> ProgramKnowledgeBase::GetStmtBound(
        StmtType type, int first_stmt, int last_stmt) {
    std::vector<int>::const_iterator first, second;

    const auto &type_stmts = type_stmt_.GetStatements(type);
    first = std::lower_bound(type_stmts.begin(), type_stmts.end(), first_stmt);
    second = std::upper_bound(first, type_stmts.end(), last_stmt);

    return {first, second};
}

void ProgramKnowledgeBase::AppendVarIndicesModifies(
        Pair<std::vector<int>::const_iterator> bound, std::set<int> &results) {
    for (auto i = bound.first; i != bound.second; i++) {
        results.emplace(modifies_rel_.GetVarIndex(*i));
    }
}
void ProgramKnowledgeBase::AppendVarIndicesUses(
        std::pair<std::vector<int>::const_iterator,
                  std::vector<int>::const_iterator>
                bound,
        std::set<int> &results) {
    for (auto i = bound.first; i != bound.second; i++) {
        auto var_indices = uses_rel_.GetVarIndex(*i);
        results.insert(var_indices.begin(), var_indices.end());
    }
}

}  // namespace spa
