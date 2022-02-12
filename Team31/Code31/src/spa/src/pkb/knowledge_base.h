#ifndef SRC_SPA_SRC_PKB_KNOWLEDGE_BASE_H_
#define SRC_SPA_SRC_PKB_KNOWLEDGE_BASE_H_

#include <string>
#include <vector>

#include "container_node.h"
#include "polish_notation.h"

namespace spa {

using PN = spa::PolishNotation;
using CN = spa::ContainerNode;

enum PKBEntityType {
    kProc,
    kVar,
    kConst,
    kReadStmt,
    kPrintStmt,
    kCallStmt,
    kWhileStmt,
    kIfStmt,
    kAssignStmt,
    kStmt,
    kStmtLst,
    kNotation
};

// index starts from zero (direct mapping with vector index)
template <PKBEntityType e>
struct Index {
    int value;
    Index() = default;
    explicit Index(int i) : value(i){};
};

struct BasicEntities {
    std::vector<std::string> procedures;
    std::vector<std::string> variables;
    std::vector<std::string> constants;
    std::vector<Index<kStmt>> reads;
    std::vector<Index<kStmt>> prints;
    std::vector<Index<kStmt>> calls;
    std::vector<Index<kStmt>> whiles;
    std::vector<Index<kStmt>> ifs;
    std::vector<Index<kStmt>> assigns;
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
    virtual void SetIndex(Index<kProc> proc_index,
                          Index<kStmtLst> stmtlst_index) = 0;

    /**
     * Links stmt# of while statement with its statement list's index
     * containing stmt# (only stmt at the outermost same level) inside while
     * loop.
     */
    virtual void SetIndex(Index<kStmt> while_stmt,
                          Index<kStmtLst> stmtlst_index) = 0;

    /**
     * Links stmt# of if statement with its two statement lists' indices
     * containing stmt# (only stmt at the outermost same level) inside then
     * clause and else clause respectively.
     */
    virtual void SetIndex(Index<kStmt> if_stmt, Index<kStmtLst> then_index,
                          Index<kStmtLst> else_index) = 0;

    /**
     * Stores each statement list to its respective index.
     */
    virtual void SetLst(Index<kStmtLst> stmtlst_index,
                        std::vector<Index<kStmt>> stmtlst) = 0;

    /**
     * Sets direct Uses relationships between print stmt# and its variable's
     * index
     */
    virtual void SetRel(Index<kStmt> stmt_no, Index<kVar> var_index) = 0;

    /**
     * Gets all strings of procedures, variables or constants.
     * The strings should be proc_name, var_name or const_value respectively.
     */
    virtual std::vector<std::string> GetAllStringEntities(PKBEntityType et) = 0;

    /**
     * Gets all stmt# of the given statement type
     */
    virtual std::vector<Index<kStmt>> GetAllStmtEntities(PKBEntityType et) = 0;

    /**
     * Converts the entities to string according to their respective indices.
     * Procedure, variable and constant types are using the indices of their
     * internal data structure. The statement types are using stmt# directly as
     * their indices.
     */
    virtual std::vector<std::string> GetProcName(
            const std::vector<Index<kProc>> &index_list) = 0;
    virtual std::vector<std::string> GetVarName(
            const std::vector<Index<kVar>> &index_list) = 0;
    virtual std::vector<std::string> GetConstValue(
            const std::vector<Index<kConst>> &index_list) = 0;
    template <PKBEntityType T>
    std::vector<std::string> IndexToName(std::vector<Index<T>> index_list);

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
