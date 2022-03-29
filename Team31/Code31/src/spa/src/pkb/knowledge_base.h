#ifndef SRC_SPA_SRC_PKB_KNOWLEDGE_BASE_H_
#define SRC_SPA_SRC_PKB_KNOWLEDGE_BASE_H_

#include <list>
#include <set>
#include <string>
#include <string_view>

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/name.h"
#include "common/polish_notation.h"
#include "pkb/store/stmtlst_parent_store.h"
#include "qps/query_token.h"
#include "qps/synonym.h"

namespace spa {

using PN = spa::PolishNotation;
using PType = spa::StmtlstParentStore::ParentType;

struct BasicEntities {
    std::vector<std::string> procedures;
    std::vector<std::string> variables;
    std::vector<std::string> constants;
    std::vector<int> reads;
    std::vector<int> prints;
    std::vector<int> calls;
    std::vector<int> whiles;
    std::vector<int> ifs;
    std::vector<int> assigns;
    // each notation is unique
    // notations may not have the same size as assigns
    std::vector<PN> notations;
    std::vector<std::set<int>> proc_call_graph;
};

class KnowledgeBase {
  public:
    static const int kWildCard = 0;
    /**
     * Links procedure index with its statement list's index
     * containing stmt# (only stmt at the outermost same level) inside the
     * procedure.
     */
    virtual void SetIndex(Index<SetEntityType::kProc> proc_index,
                          Index<SetEntityType::kStmtLst> stmtlst_index) = 0;

    /**
     * Links stmt# of while statement with its statement list's index
     * containing stmt# (only stmt at the outermost same level) inside while
     * loop.
     */
    virtual void SetIndex(Index<SetEntityType::kStmt> while_stmt,
                          Index<SetEntityType::kStmtLst> stmtlst_index) = 0;

    /**
     * Links stmt# of if statement with its two statement lists' indices
     * containing stmt# (only stmt at the outermost same level) inside then
     * clause and else clause respectively.
     */
    virtual void SetIndex(Index<SetEntityType::kStmt> if_stmt,
                          Index<SetEntityType::kStmtLst> then_index,
                          Index<SetEntityType::kStmtLst> else_index) = 0;

    /**
     * Links call statement to the procedure it calls via their indices.
     */
    virtual void SetIndex(Index<SetEntityType::kStmt> call_stmt,
                          Index<SetEntityType::kProc> proc_index) = 0;

    /**
     * Links assign statement to the polish_notation it calls via their indices.
     */
    virtual void SetIndex(Index<SetEntityType::kStmt> assign_stmt,
                          Index<SetEntityType::kNotation> notation_index) = 0;

    /**
     * Stores each statement list to its respective index.
     */
    virtual void SetLst(Index<SetEntityType::kStmtLst> stmtlst_index,
                        std::vector<int> stmtlst) = 0;

    /**
     * Sets direct Modifes relationships between stmt# and its variable's
     * index
     */
    virtual void SetRel(Index<SetEntityType::kStmt> stmt_no,
                        Index<SetEntityType::kVar> var_index) = 0;

    /**
     * Sets direct Uses relationships between stmt# and its variable
     * indices
     */
    virtual void SetRel(Index<SetEntityType::kStmt> stmt_no,
                        std::vector<int> var_indices) = 0;

    virtual bool ExistFollows(Index<ArgPos::kFirst> first_stmt,
                              Index<ArgPos::kSecond> second_stmt) = 0;
    virtual bool ExistFollowsT(Index<ArgPos::kFirst> first_stmt,
                               Index<ArgPos::kSecond> second_stmt) = 0;
    /**
     * Check if Follows or Follows* (first_stmt, _) exists
     */
    virtual bool ExistFollows(Index<ArgPos::kFirst> first_stmt) = 0;
    /**
     * Check if Follows or Follows* (_, second_stmt) exist
     */
    virtual bool ExistFollows(Index<ArgPos::kSecond> second_stmt) = 0;
    /**
     * Check if Follows or Follows* (_,_)
     */
    virtual bool ExistFollows() = 0;

    virtual bool ExistParent(Index<ArgPos::kFirst> parent_stmt,
                             Index<ArgPos::kSecond> child_stmt) = 0;
    virtual bool ExistParentT(Index<ArgPos::kFirst> parent_stmt,
                              Index<ArgPos::kSecond> child_stmt) = 0;
    /**
     * Check if Parent or Parent* (parent_stmt, _) exists
     */
    virtual bool ExistParent(Index<ArgPos::kFirst> parent_stmt) = 0;
    /**
     * Check if Parent or Parent* (_, child_stmt) exists
     */
    virtual bool ExistParent(Index<ArgPos::kSecond> child_stmt) = 0;
    /**
     * Check if Parent or Parent* (_,_)
     */
    virtual bool ExistParent() = 0;

    /**
     * Gets a list of stmt# that satisfy Follows/Follows*(stmt#, _) or
     * Follows/Follows*(_, stmt#)
     */
    virtual std::set<int> GetFollows(ArgPos return_pos,
                                     StmtType return_type) = 0;
    std::set<int> GetFollows(ArgPos return_pos) {
        return GetFollows(return_pos, StmtType::kAll);
    }
    virtual std::set<int> GetFollows(Index<ArgPos::kFirst> stmt_no,
                                     StmtType return_type) = 0;
    virtual std::set<int> GetFollowsT(Index<ArgPos::kFirst> stmt_no,
                                      StmtType return_type) = 0;
    std::set<int> GetFollows(Index<ArgPos::kFirst> stmt_no) {
        return GetFollows(stmt_no, StmtType::kAll);
    }

    virtual std::set<int> GetFollows(Index<ArgPos::kSecond> stmt_no,
                                     StmtType return_type) = 0;
    virtual std::set<int> GetFollowsT(Index<ArgPos::kSecond> stmt_no,
                                      StmtType return_type) = 0;

    virtual PairVec<int> GetFollowsPairs(StmtType first_type,
                                         StmtType second_type) = 0;
    virtual PairVec<int> GetFollowsTPairs(StmtType first_type,
                                          StmtType second_type) = 0;

    /**
     * Gets a list of stmt# that satisfy Parent/Parent*(stmt#, _) or
     * Parent/Parent*(_, stmt#)
     */
    virtual std::set<int> GetParent(ArgPos return_pos,
                                    StmtType return_type) = 0;
    std::set<int> GetParent(ArgPos return_pos) {
        return GetParent(return_pos, StmtType::kAll);
    }
    virtual std::set<int> GetParent(Index<ArgPos::kFirst> parent_stmt,
                                    StmtType return_type) = 0;
    virtual std::set<int> GetParentT(Index<ArgPos::kFirst> parent_stmt,
                                     StmtType return_type) = 0;

    virtual std::set<int> GetParent(Index<ArgPos::kSecond> child_stmt,
                                    StmtType return_type) = 0;
    virtual std::set<int> GetParentT(Index<ArgPos::kSecond> child_stmt,
                                     StmtType return_type) = 0;
    std::set<int> GetParent(Index<ArgPos::kSecond> child_stmt) {
        return GetParent(child_stmt, StmtType::kAll);
    }

    virtual PairVec<int> GetParentPairs(StmtType parent_type,
                                        StmtType child_type) = 0;
    virtual PairVec<int> GetParentTPairs(StmtType parent_type,
                                         StmtType child_type) = 0;

    /**
     * Check if modifies relationships between stmt# and its variable
     * exist
     */
    virtual bool ExistModifies(int stmt_no, std::string_view var_name) = 0;
    virtual bool ExistModifies(int stmt_no, int var_index) = 0;

    /**
     * Check if uses relationships between stmt# and its variable
     * exist
     */
    virtual bool ExistUses(int stmt_no, std::string_view var_name) = 0;
    virtual bool ExistUses(int stmt_no, int var_index) = 0;

    /**
     * Gets a var_index that is modified in stmt#
     */
    virtual std::set<int> GetModifies(
            Index<QueryEntityType::kStmt> stmt_no) = 0;

    /**
     * Gets a list of stmt# that modifies var_index
     */
    virtual std::set<int> GetModifies(std::string_view var_name,
                                      StmtType type) = 0;
    virtual std::set<int> GetModifies(Index<QueryEntityType::kVar> var_index,
                                      StmtType type) = 0;

    /**
     * Gets a list of stmt# that modifies any var_index (wildcard)
     */
    virtual std::set<int> GetModifies(StmtType type) = 0;

    /**
     * For Modifies (s,v)
     */
    virtual PairVec<int> GetModifiesStmtVar(StmtType type) = 0;

    /**
     * Gets a list of var_index that are used in stmt#
     */
    virtual std::set<int> GetUses(Index<QueryEntityType::kStmt> stmt_no) = 0;

    /**
     * Gets a list of stmt# that uses var_index
     */
    virtual std::set<int> GetUses(std::string_view var_name, StmtType type) = 0;
    virtual std::set<int> GetUses(Index<QueryEntityType::kVar> var_index,
                                  StmtType type) = 0;

    /**
     * Gets a list of stmt# that uses var_index
     */
    virtual std::set<int> GetUses(StmtType type) = 0;

    /**
     * For Uses (s,v)
     */
    virtual PairVec<int> GetUsesStmtVar(StmtType type) = 0;

    // Modifies and Uses for Procedure
    // Modifies ("", "")
    virtual bool ExistModifies(std::string_view proc_name,
                               std::string_view var_name) = 0;
    // Modifies ("", _)
    virtual bool ExistModifies(std::string_view proc_name) = 0;
    // Modifies ("", v)
    virtual std::set<int> GetModifies(Name<ArgPos::kFirst> proc_name) = 0;
    // Modifies (p, "")
    virtual std::set<int> GetModifies(Name<ArgPos::kSecond> var_name) = 0;
    //  Modifies (p, _)
    virtual std::set<int> GetModifiesProc() = 0;
    //  Modifies (p, v)
    virtual PairVec<int> GetModifiesProcVar() = 0;
    // Uses ("", "")
    virtual bool ExistUses(std::string_view proc_name,
                           std::string_view var_name) = 0;
    // Uses ("", _)
    virtual bool ExistUses(std::string_view proc_name) = 0;
    // Uses ("", v)
    virtual std::set<int> GetUses(Name<ArgPos::kFirst> proc_name) = 0;
    // Uses (p, "")
    virtual std::set<int> GetUses(Name<ArgPos::kSecond> var_name) = 0;
    //  Uses (p, _)
    virtual std::set<int> GetUsesProc() = 0;
    //  Uses (p, v)
    virtual PairVec<int> GetUsesProcVar() = 0;

    /**
     * For pattern related methods, a "P" at the end of the method name
     * indicates partial matching.
     */
    virtual std::set<int> GetPattern(std::vector<QueryToken> tokens) = 0;
    virtual std::set<int> GetPatternP(std::vector<QueryToken> tokens) = 0;

    //(" ", _)
    virtual std::set<int> GetPattern(std::string_view var_name) = 0;
    virtual std::set<int> GetPattern(int var_index) = 0;

    virtual std::set<int> GetPattern(std::string_view var_name,
                                     std::vector<QueryToken> second_tokens) = 0;
    virtual std::set<int> GetPatternP(
            std::string_view var_name,
            std::vector<QueryToken> second_tokens) = 0;
    virtual std::set<int> GetPattern(int var_index,
                                     std::vector<QueryToken> second_tokens) = 0;
    virtual std::set<int> GetPatternP(
            int var_index, std::vector<QueryToken> second_tokens) = 0;
    virtual PairVec<int> GetPatternPair(std::vector<QueryToken> tokens) = 0;
    virtual PairVec<int> GetPatternPairP(std::vector<QueryToken> tokens) = 0;

    // (v, _)
    virtual PairVec<int> GetPatternPair() = 0;

    // if("v",_,_)
    virtual std::set<int> GetPatternIf(std::string_view var_name) = 0;
    virtual std::set<int> GetPatternIf(int var_index) = 0;
    // if(v,_,_)
    virtual PairVec<int> GetPatternPairIf() = 0;
    // if(_,_,_)
    virtual std::set<int> GetPatternIf() = 0;
    // while("v",_,_)
    virtual std::set<int> GetPatternWhile(std::string_view var_name) = 0;
    virtual std::set<int> GetPatternWhile(int var_index) = 0;
    // while(v,_,_)
    virtual PairVec<int> GetPatternPairWhile() = 0;
    // while(_,_,_)
    virtual std::set<int> GetPatternWhile() = 0;

    /**
     * Check if Calls (first_proc, second_proc) exist
     */
    virtual bool ExistCalls(Index<ArgPos::kFirst> first_proc,
                            Index<ArgPos::kSecond> second_proc) = 0;
    virtual bool ExistCalls(std::string_view first_proc_name,
                            std::string_view second_proc_name) = 0;
    /**
     * Check if Calls* (first_proc, second_proc) exist
     */
    virtual bool ExistCallsT(Index<ArgPos::kFirst> first_proc,
                             Index<ArgPos::kSecond> second_proc) = 0;
    virtual bool ExistCallsT(std::string_view first_proc_name,
                             std::string_view second_proc_name) = 0;
    /**
     * Check if Calls or Calls* (first_proc, _) exist
     */
    virtual bool ExistCalls(Index<ArgPos::kFirst> first_proc) = 0;
    virtual bool ExistCalls(Name<ArgPos::kFirst> first_proc_name) = 0;
    /**
     * Check if Calls or Calls* (_, second_proc) exist
     */
    virtual bool ExistCalls(Index<ArgPos::kSecond> second_proc) = 0;
    virtual bool ExistCalls(Name<ArgPos::kSecond> second_proc_name) = 0;
    /**
     * Check if Calls or Calls* (_, _) exist
     */
    virtual bool ExistCalls() = 0;

    /**
     * Gets a list of procedures that satisfy Calls/Calls*(procedure, _) or
     * Calls/Calls*(_, procedure)
     */
    virtual std::set<int> GetCalls(ArgPos return_pos) = 0;
    /**
     * Gets a list of procedures that is called by the given procedure
     */
    virtual std::set<int> GetCalls(Index<ArgPos::kFirst> first_proc) = 0;
    virtual std::set<int> GetCalls(Name<ArgPos::kFirst> first_proc_name) = 0;
    virtual std::set<int> GetCallsT(Index<ArgPos::kFirst> first_proc) = 0;
    virtual std::set<int> GetCallsT(Name<ArgPos::kFirst> first_proc_name) = 0;
    /**
     * Gets a list of procedures that call the given procedure
     */
    virtual std::set<int> GetCalls(Index<ArgPos::kSecond> second_proc) = 0;
    virtual std::set<int> GetCalls(Name<ArgPos::kSecond> second_proc_name) = 0;
    virtual std::set<int> GetCallsT(Index<ArgPos::kSecond> second_proc) = 0;
    virtual std::set<int> GetCallsT(Name<ArgPos::kSecond> second_proc_name) = 0;
    /**
     * Gets a list of procedure pairs that exist in Calls/Calls*
     */
    virtual PairVec<int> GetCallsPairs() = 0;
    virtual PairVec<int> GetCallsTPairs() = 0;

    // Next("", "")
    virtual bool ExistNext(Index<ArgPos::kFirst> first_stmt,
                           Index<ArgPos::kSecond> second_stmt) = 0;
    // Next*("", "")
    virtual bool ExistNextT(Index<ArgPos::kFirst> first_stmt,
                            Index<ArgPos::kSecond> second_stmt) = 0;
    // Next/Next*("", _)
    virtual bool ExistNext(Index<ArgPos::kFirst> first_stmt) = 0;
    // Next/Next*(_, "")
    virtual bool ExistNext(Index<ArgPos::kSecond> second_stmt) = 0;
    // Next/Next*(_, _)
    virtual bool ExistNext() = 0;
    // Next/Next*(s, _) or (_, s)
    virtual std::set<int> GetNext(ArgPos return_pos, StmtType return_type) = 0;
    std::set<int> GetNext(ArgPos return_pos) {
        return GetNext(return_pos, StmtType::kAll);
    }
    // Next("", s)
    virtual std::set<int> GetNext(Index<ArgPos::kFirst> stmt,
                                  StmtType return_type) = 0;
    std::set<int> GetNext(Index<ArgPos::kFirst> stmt) {
        return GetNext(stmt, StmtType::kAll);
    }
    // Next*("", s)
    virtual std::set<int> GetNextT(Index<ArgPos::kFirst> stmt,
                                   StmtType return_type) = 0;
    std::set<int> GetNextT(Index<ArgPos::kFirst> stmt) {
        return GetNextT(stmt, StmtType::kAll);
    }
    // Next(s,"")
    virtual std::set<int> GetNext(Index<ArgPos::kSecond> stmt,
                                  StmtType return_type) = 0;
    std::set<int> GetNext(Index<ArgPos::kSecond> stmt) {
        return GetNext(stmt, StmtType::kAll);
    }
    // Next*(s,"")
    virtual std::set<int> GetNextT(Index<ArgPos::kSecond> stmt,
                                   StmtType return_type) = 0;
    std::set<int> GetNextT(Index<ArgPos::kSecond> stmt) {
        return GetNextT(stmt, StmtType::kAll);
    }
    // Next(s,s)
    virtual PairVec<int> GetNextPairs(StmtType first_type,
                                      StmtType second_type) = 0;
    // Next*(s,s)
    virtual PairVec<int> GetNextTPairs(StmtType first_type,
                                       StmtType second_type) = 0;

    // Affects("", "")
    virtual bool ExistAffects(Index<ArgPos::kFirst> first_assign,
                              Index<ArgPos::kSecond> second_assign) = 0;
    // Affects*("", "")
    virtual bool ExistAffectsT(Index<ArgPos::kFirst> first_assign,
                               Index<ArgPos::kSecond> second_assign) = 0;
    // Affects("", _)
    virtual bool ExistAffects(Index<ArgPos::kFirst> first_assign) = 0;
    // Affects(_, "")
    virtual bool ExistAffects(Index<ArgPos::kSecond> second_assign) = 0;
    // Affects(_, _)
    virtual bool ExistAffects() = 0;
    // Affects/Affects*(a, _) or (_, a)
    virtual std::set<int> GetAffects(ArgPos return_pos) = 0;
    // Affects("", a)
    virtual std::set<int> GetAffects(Index<ArgPos::kFirst> assign) = 0;
    // Affects*("", a)
    virtual std::set<int> GetAffectsT(Index<ArgPos::kFirst> assign) = 0;
    // Affects(a,"")
    virtual std::set<int> GetAffects(Index<ArgPos::kSecond> assign) = 0;
    // Affects*(a,"")
    virtual std::set<int> GetAffectsT(Index<ArgPos::kSecond> assign) = 0;
    // Affects(a,a)
    virtual PairVec<int> GetAffectsPairs() = 0;
    // Affects*(a,a)
    virtual PairVec<int> GetAffectsTPairs() = 0;

    /**
     * Gets all indices of the given entity type or stmt type
     */
    virtual std::vector<int> GetAllEntityIndices(QueryEntityType et) = 0;
    virtual std::vector<int> GetAllEntityIndices(StmtType st) = 0;
    virtual std::vector<int> GetAllEntityIndices(Synonym::Type synType) = 0;

    /**
     * Converts the entities to string according to their respective indices.
     * Procedure, variable and constant types are using the indices of their
     * internal data structure. The statement types are using stmt# directly as
     * their indices.
     */
    virtual void ToName(Synonym::Type syn_type,
                        const std::vector<int> &index_list,
                        std::list<std::string> &names) = 0;

    /**
     * Prevents any further updates to the KnowledgeBase and compile all
     * indirect relationships for faster lookup.
     */
    virtual void Compile() = 0;
    virtual ~KnowledgeBase() = 0;
};

inline KnowledgeBase::~KnowledgeBase() = default;

}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_KNOWLEDGE_BASE_H_
