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

struct BasicEntities {
    std::vector<std::string> procedures;
    std::vector<std::string> variables;
    std::vector<std::string> constants;
    std::vector<StmtNo> reads;
    std::vector<StmtNo> prints;
    std::vector<StmtNo> calls;
    std::vector<StmtNo> whiles;
    std::vector<StmtNo> ifs;
    std::vector<StmtNo> assigns;
    // each notation is unique
    // notations may not have the same size as assigns
    std::vector<PN> notations;
    std::vector<std::set<ProcIndex>> proc_call_graph;
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
                        std::vector<StmtNo> stmtlst) = 0;

    /**
     * Sets direct Modifies relationships between stmt# and its variable's
     * index
     */
    virtual void SetRel(Index<SetEntityType::kStmt> stmt_no,
                        Index<SetEntityType::kVar> var_index) = 0;

    /**
     * Sets direct Uses relationships between stmt# and its variable
     * indices
     */
    virtual void SetRel(Index<SetEntityType::kStmt> stmt_no,
                        std::vector<VarIndex> var_indices) = 0;

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
    virtual std::set<StmtNo> GetFollows(ArgPos return_pos,
                                        StmtType return_type) = 0;
    virtual std::set<StmtNo> GetFollows(Index<ArgPos::kFirst> stmt_no,
                                        StmtType return_type) = 0;
    virtual std::set<StmtNo> GetFollowsT(Index<ArgPos::kFirst> stmt_no,
                                         StmtType return_type) = 0;

    virtual std::set<StmtNo> GetFollows(Index<ArgPos::kSecond> stmt_no,
                                        StmtType return_type) = 0;
    virtual std::set<StmtNo> GetFollowsT(Index<ArgPos::kSecond> stmt_no,
                                         StmtType return_type) = 0;

    virtual PairVec<StmtNo> GetFollowsPairs(StmtType first_type,
                                            StmtType second_type) = 0;
    virtual PairVec<StmtNo> GetFollowsTPairs(StmtType first_type,
                                             StmtType second_type) = 0;

    /**
     * Gets a list of stmt# that satisfy Parent/Parent*(stmt#, _) or
     * Parent/Parent*(_, stmt#)
     */
    virtual std::set<StmtNo> GetParent(ArgPos return_pos,
                                       StmtType return_type) = 0;
    std::set<StmtNo> GetParent(ArgPos return_pos) {
        return GetParent(return_pos, StmtType::kAll);
    }
    virtual std::set<StmtNo> GetParent(Index<ArgPos::kFirst> parent_stmt,
                                       StmtType return_type) = 0;
    virtual std::set<StmtNo> GetParentT(Index<ArgPos::kFirst> parent_stmt,
                                        StmtType return_type) = 0;

    virtual std::set<StmtNo> GetParent(Index<ArgPos::kSecond> child_stmt,
                                       StmtType return_type) = 0;
    virtual std::set<StmtNo> GetParentT(Index<ArgPos::kSecond> child_stmt,
                                        StmtType return_type) = 0;
    std::set<StmtNo> GetParent(Index<ArgPos::kSecond> child_stmt) {
        return GetParent(child_stmt, StmtType::kAll);
    }

    virtual PairVec<StmtNo> GetParentPairs(StmtType parent_type,
                                           StmtType child_type) = 0;
    virtual PairVec<StmtNo> GetParentTPairs(StmtType parent_type,
                                            StmtType child_type) = 0;

    /**
     * Check if modifies relationships between stmt# and its variable
     * exist
     */
    virtual bool ExistModifies(StmtNo stmt_no, std::string_view var_name) = 0;
    virtual bool ExistModifies(StmtNo stmt_no, VarIndex var_index) = 0;

    /**
     * Check if uses relationships between stmt# and its variable
     * exist
     */
    virtual bool ExistUses(StmtNo stmt_no, std::string_view var_name) = 0;
    virtual bool ExistUses(StmtNo stmt_no, VarIndex var_index) = 0;

    /**
     * Gets a var_index that is modified in stmt#
     */
    virtual std::set<VarIndex> GetModifies(
            Index<QueryEntityType::kStmt> stmt_no) = 0;

    /**
     * Gets a list of stmt# that modifies var_index
     */
    virtual std::set<StmtNo> GetModifies(std::string_view var_name,
                                         StmtType type) = 0;
    virtual std::set<StmtNo> GetModifies(Index<QueryEntityType::kVar> var_index,
                                         StmtType type) = 0;

    /**
     * Gets a list of stmt# that modifies any var_index (wildcard)
     */
    virtual std::set<StmtNo> GetModifies(StmtType type) = 0;

    /**
     * For Modifies (s,v)
     */
    virtual PairVec<StmtNo, VarIndex> GetModifiesStmtVar(StmtType type) = 0;

    /**
     * Gets a list of var_index that are used in stmt#
     */
    virtual std::set<VarIndex> GetUses(
            Index<QueryEntityType::kStmt> stmt_no) = 0;

    /**
     * Gets a list of stmt# that uses var_index
     */
    virtual std::set<StmtNo> GetUses(std::string_view var_name,
                                     StmtType type) = 0;
    virtual std::set<StmtNo> GetUses(Index<QueryEntityType::kVar> var_index,
                                     StmtType type) = 0;

    /**
     * Gets a list of stmt# that uses var_index
     */
    virtual std::set<StmtNo> GetUses(StmtType type) = 0;

    /**
     * For Uses (s,v)
     */
    virtual PairVec<StmtNo, VarIndex> GetUsesStmtVar(StmtType type) = 0;

    // Modifies and Uses for Procedure
    // Modifies ("", "")
    virtual bool ExistModifies(std::string_view proc_name,
                               std::string_view var_name) = 0;
    // Modifies ("", _)
    virtual bool ExistModifies(std::string_view proc_name) = 0;
    // Modifies ("", v)
    virtual std::set<VarIndex> GetModifies(Name<ArgPos::kFirst> proc_name) = 0;
    // Modifies (p, "")
    virtual std::set<ProcIndex> GetModifies(Name<ArgPos::kSecond> var_name) = 0;
    //  Modifies (p, _)
    virtual std::set<ProcIndex> GetModifiesProc() = 0;
    //  Modifies (p, v)
    virtual PairVec<ProcIndex, VarIndex> GetModifiesProcVar() = 0;
    // Uses ("", "")
    virtual bool ExistUses(std::string_view proc_name,
                           std::string_view var_name) = 0;
    // Uses ("", _)
    virtual bool ExistUses(std::string_view proc_name) = 0;
    // Uses ("", v)
    virtual std::set<VarIndex> GetUses(Name<ArgPos::kFirst> proc_name) = 0;
    // Uses (p, "")
    virtual std::set<ProcIndex> GetUses(Name<ArgPos::kSecond> var_name) = 0;
    //  Uses (p, _)
    virtual std::set<ProcIndex> GetUsesProc() = 0;
    //  Uses (p, v)
    virtual PairVec<ProcIndex, VarIndex> GetUsesProcVar() = 0;

    /**
     * For pattern related methods, a "P" at the end of the method name
     * indicates partial matching.
     */
    // a(_, expr)
    virtual std::set<StmtNo> GetPattern(std::vector<QueryToken> tokens) = 0;
    virtual std::set<StmtNo> GetPatternP(std::vector<QueryToken> tokens) = 0;
    // a("v", _)
    virtual std::set<StmtNo> GetPattern(std::string_view var_name) = 0;
    virtual std::set<StmtNo> GetPattern(VarIndex var_index) = 0;
    // a("v", expr)
    virtual std::set<StmtNo> GetPattern(
            std::string_view var_name,
            std::vector<QueryToken> second_tokens) = 0;
    virtual std::set<StmtNo> GetPatternP(
            std::string_view var_name,
            std::vector<QueryToken> second_tokens) = 0;
    virtual std::set<StmtNo> GetPattern(
            VarIndex var_index, std::vector<QueryToken> second_tokens) = 0;
    virtual std::set<StmtNo> GetPatternP(
            VarIndex var_index, std::vector<QueryToken> second_tokens) = 0;
    // a(v, expr)
    virtual PairVec<StmtNo, VarIndex> GetPatternPair(
            std::vector<QueryToken> tokens) = 0;
    virtual PairVec<StmtNo, VarIndex> GetPatternPairP(
            std::vector<QueryToken> tokens) = 0;
    // a(v, _)
    virtual PairVec<StmtNo, VarIndex> GetPatternPair() = 0;

    // while("v",_) or if("v",_,_)
    virtual std::set<StmtNo> GetPattern(StmtType container_type,
                                        std::string_view var_name) = 0;
    virtual std::set<StmtNo> GetPattern(StmtType container_type,
                                        VarIndex var_index) = 0;
    // while(v,_) or if(v,_,_)
    virtual PairVec<StmtNo, VarIndex> GetPatternPairs(
            StmtType container_type) = 0;
    // while(_,_) or if(_,_,_)
    virtual std::set<StmtNo> GetPattern(StmtType container_type) = 0;

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
    virtual std::set<ProcIndex> GetCalls(Index<ArgPos::kFirst> first_proc) = 0;
    virtual std::set<ProcIndex> GetCalls(
            Name<ArgPos::kFirst> first_proc_name) = 0;
    virtual std::set<ProcIndex> GetCallsT(Index<ArgPos::kFirst> first_proc) = 0;
    virtual std::set<ProcIndex> GetCallsT(
            Name<ArgPos::kFirst> first_proc_name) = 0;
    /**
     * Gets a list of procedures that call the given procedure
     */
    virtual std::set<ProcIndex> GetCalls(
            Index<ArgPos::kSecond> second_proc) = 0;
    virtual std::set<ProcIndex> GetCalls(
            Name<ArgPos::kSecond> second_proc_name) = 0;
    virtual std::set<ProcIndex> GetCallsT(
            Index<ArgPos::kSecond> second_proc) = 0;
    virtual std::set<ProcIndex> GetCallsT(
            Name<ArgPos::kSecond> second_proc_name) = 0;
    /**
     * Gets a list of procedure pairs that exist in Calls/Calls*
     */
    virtual PairVec<ProcIndex> GetCallsPairs() = 0;
    virtual PairVec<ProcIndex> GetCallsTPairs() = 0;

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
    virtual std::set<StmtNo> GetNext(ArgPos return_pos,
                                     StmtType return_type) = 0;
    // Next("", s)
    virtual std::set<StmtNo> GetNext(Index<ArgPos::kFirst> stmt,
                                     StmtType return_type) = 0;
    // Next*("", s)
    virtual std::set<StmtNo> GetNextT(Index<ArgPos::kFirst> stmt,
                                      StmtType return_type) = 0;
    // Next(s,"")
    virtual std::set<StmtNo> GetNext(Index<ArgPos::kSecond> stmt,
                                     StmtType return_type) = 0;
    // Next*(s,"")
    virtual std::set<StmtNo> GetNextT(Index<ArgPos::kSecond> stmt,
                                      StmtType return_type) = 0;
    // Next(s1,s2)
    virtual PairVec<StmtNo> GetNextPairs(StmtType first_type,
                                         StmtType second_type) = 0;
    // Next*(s1,s2)
    virtual PairVec<StmtNo> GetNextTPairs(StmtType first_type,
                                          StmtType second_type) = 0;
    virtual std::set<StmtNo> GetNextTSelf(StmtType type) = 0;

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
    virtual std::set<StmtNo> GetAffects(ArgPos return_pos) = 0;
    // Affects("", a)
    virtual std::set<StmtNo> GetAffects(Index<ArgPos::kFirst> assign) = 0;
    // Affects*("", a)
    virtual std::set<StmtNo> GetAffectsT(Index<ArgPos::kFirst> assign) = 0;
    // Affects(a,"")
    virtual std::set<StmtNo> GetAffects(Index<ArgPos::kSecond> assign) = 0;
    // Affects*(a,"")
    virtual std::set<StmtNo> GetAffectsT(Index<ArgPos::kSecond> assign) = 0;
    // Affects(a,a)
    virtual PairVec<StmtNo> GetAffectsPairs() = 0;
    // Affects*(a,a)
    virtual PairVec<StmtNo> GetAffectsTPairs() = 0;

    /**
     * Gets all indices of the given entity type or stmt type
     */
    virtual std::vector<int> GetAllEntityIndices(QueryEntityType et) = 0;
    virtual std::vector<int> GetAllEntityIndices(StmtType st) = 0;

    /**
     * Converts the entities to string according to their respective indices.
     * Procedure, variable and constant types are using the indices of their
     * internal data structure. The statement types are using stmt# directly as
     * their indices.
     */
    virtual std::vector<std::string_view> GetNames(
            const std::vector<int> &indices, QueryEntityType entity_type) = 0;
    virtual std::vector<std::string_view> GetAttr(
            const std::vector<int> &indices, StmtType et) = 0;
    virtual void ToName(QueryEntityType et, const std::vector<int> &index_list,
                        std::list<std::string> &names) = 0;
    virtual void ToAttr(StmtType et, const std::vector<int> &index_list,
                        std::list<std::string> &names) = 0;
    virtual int IdentToIndexValue(std::string_view name,
                                  QueryEntityType et) = 0;

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
