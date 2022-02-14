#ifndef SRC_SPA_SRC_PKB_KNOWLEDGE_BASE_H_
#define SRC_SPA_SRC_PKB_KNOWLEDGE_BASE_H_

#include <list>
#include <string>
#include <type_traits>
#include <vector>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/polish_notation.h"
#include "pkb/secondary_structure/container_node.h"

namespace spa {

using PN = spa::PolishNotation;

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
     * Check if Follows or Follows* relationships between two stmt# exist
     */
    virtual bool ExistFollows(bool transitive, int first_stmt,
                              int second_stmt) = 0;

    /**
     * Check if Parent or Parents* relationships between container stmt# and
     * another stmt# exist
     */
    virtual bool ExistParent(bool transitive, int parent_stmt,
                             int child_stmt) = 0;

    /**
     * Gets stmt# that is directly followed by given stmt#
     */
    virtual std::vector<int> GetFollows(
            bool transitive, Index<GetPos::kFirst> stmt_no,
            const std::vector<int> &filtered_stmt) = 0;

    /**
     * Gets stmt# that directly follow the given stmt#
     */
    virtual std::vector<int> GetFollows(
            bool transitive, Index<GetPos::kSecond> stmt_no,
            const std::vector<int> &filtered_stmt) = 0;

    /**
     * Gets a list of stmt# that
     * are direct or indirect parents of (contain)
     * the given stmt# if get_pos is kFirst
     */
    virtual std::vector<int> GetParent(
            bool transitive, Index<GetPos::kFirst> stmt_no,
            const std::vector<int> &filtered_stmt) = 0;

    /**
     * Gets a list of stmt# that are direct or indirect children of
     * (are nested in) the given stmt#.
     */
    virtual std::vector<int> GetParent(
            bool transitive, Index<GetPos::kSecond> stmt_no,
            const std::vector<int> &filtered_stmt) = 0;

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
