#ifndef SRC_SPA_SRC_PKB_KNOWLEDGE_BASE_H_
#define SRC_SPA_SRC_PKB_KNOWLEDGE_BASE_H_

#include <list>
#include <set>
#include <string>
#include <type_traits>
#include <vector>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/polish_notation.h"
#include "pkb/secondary_structure/container_node.h"
#include "pkb/store/stmtlst_parent_store.h"
#include "qps/query_token.h"

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
};

class KnowledgeBase {
  public:
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

    /**
     * Check if Follows or Follows* (first_stmt, second_stmt) exist
     */
    virtual bool ExistFollows(bool transitive, Index<ArgPos::kFirst> first_stmt,
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

    /**
     * Check if Parent or Parents* (parent_stmt, child_stmt) exists
     */
    virtual bool ExistParent(bool transitive, Index<ArgPos::kFirst> parent_stmt,
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
    /**
     * Gets a list of stmt# that appear after the given stmt#
     * at the same nesting level.
     */
    virtual std::set<int> GetFollows(bool transitive,
                                     Index<ArgPos::kFirst> stmt_no,
                                     StmtType return_type) = 0;
    std::set<int> GetFollows(bool transitive, Index<ArgPos::kFirst> stmt_no) {
        return GetFollows(transitive, stmt_no, StmtType::kAll);
    }

    /**
     * Gets a list of stmt# that appear before the given stmt#
     * at the same nesting level
     */
    virtual std::set<int> GetFollows(bool transitive,
                                     Index<ArgPos::kSecond> stmt_no,
                                     StmtType return_type) = 0;
    std::set<int> GetFollows(bool transitive, Index<ArgPos::kSecond> stmt_no) {
        return GetFollows(transitive, stmt_no, StmtType::kAll);
    }

    /**
     * Gets a list of stmt# pairs that exist in Follows relationship
     */
    virtual std::pair<std::vector<int>, std::vector<int>> GetFollowsPairs(
            bool transitive, StmtType first_type, StmtType second_type) = 0;

    /**
     * Gets a list of stmt# that satisfy Parent/Parent*(stmt#, _) or
     * Parent/Parent*(_, stmt#)
     */
    virtual std::set<int> GetParent(ArgPos return_pos,
                                    StmtType return_type) = 0;
    std::set<int> GetParent(ArgPos return_pos) {
        return GetParent(return_pos, StmtType::kAll);
    }
    /**
     * Gets a list of stmt# that
     * are direct or indirect children of (are nested in) the given stmt#
     */
    virtual std::set<int> GetParent(bool transitive,
                                    Index<ArgPos::kFirst> parent_stmt,
                                    StmtType return_type) = 0;
    std::set<int> GetParent(bool transitive,
                            Index<ArgPos::kFirst> parent_stmt) {
        return GetParent(transitive, parent_stmt, StmtType::kAll);
    }

    /**
     * Gets a list of stmt# that are direct or indirect parents of
     * (contain) the given stmt#.
     */
    virtual std::set<int> GetParent(bool transitive,
                                    Index<ArgPos::kSecond> child_stmt,
                                    StmtType return_type) = 0;
    std::set<int> GetParent(bool transitive,
                            Index<ArgPos::kSecond> child_stmt) {
        return GetParent(transitive, child_stmt, StmtType::kAll);
    }

    /**
     * Gets a list of stmt# pairs that exist in Parent/Parent* relationship
     */
    virtual std::pair<std::vector<int>, std::vector<int>> GetParentPairs(
            bool transitive, StmtType parent_type, StmtType child_type) = 0;

    /**
     * Check if modifies relationships between stmt# and its variable
     * exist
     */

    virtual bool ExistModifies(int stmt_no, int var_index) = 0;

    /**
     * Check if uses relationships between stmt# and its variable
     * exist
     */
    virtual bool ExistUses(int stmt_no, int var_index) = 0;

    /**
     * Gets a var_index that is modified in stmt#
     */
    virtual std::vector<int> GetModifies(
            Index<QueryEntityType::kStmt> stmt_no,
            const std::vector<int> &filtered_var) = 0;

    /**
     * Gets a list of stmt# that modifies var_index
     */
    virtual std::vector<int> GetModifies(
            Index<QueryEntityType::kVar> var_index,
            const std::vector<int> &filtered_stmt) = 0;

    /**
     * Gets a list of var_index that are used in stmt#
     */
    virtual std::vector<int> GetUses(Index<QueryEntityType::kStmt> stmt_no,
                                     const std::vector<int> &filtered_var) = 0;

    /**
     * Gets a list of stmt# that uses var_index
     */
    virtual std::vector<int> GetUses(Index<QueryEntityType::kVar> var_index,
                                     const std::vector<int> &filtered_stmt) = 0;

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
    virtual void IndexToName(QueryEntityType et,
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
