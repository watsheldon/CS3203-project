#ifndef SRC_SPA_SRC_PKB_KNOWLEDGE_BASE_H_
#define SRC_SPA_SRC_PKB_KNOWLEDGE_BASE_H_

#include <string>
#include <vector>

#include "container_node.h"
#include "polish_notation.h"

namespace spa {

using STMT_NO = int;
using STMTLST_NO = int;
using PN = spa::PolishNotation;
using INDEX = int;
using CN = spa::ContainerNode;

struct BasicEntities {
    std::vector<std::string> procedures;
    std::vector<std::string> variables;
    std::vector<std::string> constants;
    std::vector<STMT_NO> reads;
    std::vector<STMT_NO> prints;
    std::vector<STMT_NO> calls;
    std::vector<STMT_NO> whiles;
    std::vector<STMT_NO> ifs;
    std::vector<STMT_NO> assigns;
    // each notation is unique
    // notations may not have the same size as assigns
    std::vector<PN> notations;
};

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
    virtual void SetIndex(Index<kWhileStmt> stmt_no,
                          Index<kStmtLst> stmtlst_index) = 0;

    /**
     * Links stmt# of if statement with its two statement lists' indices
     * containing stmt# (only stmt at the outermost same level) inside then
     * clause and else clause respectively.
     */
    virtual void SetIndex(Index<kIfStmt> stmt_no, Index<kStmtLst> then_index,
                          Index<kStmtLst> else_index) = 0;

    /**
     * Stores each statement list to its respective index.
     */
    virtual void SetLst(Index<kStmtLst> stmtlst_index,
                        std::vector<STMT_NO> stmtlst) = 0;

    /**
     * Sets direct Uses relationships between print stmt# and its variable's
     * index
     */
    virtual void SetRel(Index<kPrintStmt> stmt_no, Index<kVar> var_index) = 0;

    /**
     * Gets all strings of procedures, variables or constants.
     * The strings should be proc_name, var_name or const_value respectively.
     */
    virtual std::vector<std::string> GetAllStringEntities(PKBEntityType et) = 0;

    /**
     * Gets all stmt# of the given statement type
     */
    virtual std::vector<int> GetAllStmtEntities(PKBEntityType et) = 0;

    /**
     * Converts the entities to string according to their respective indices.
     * Procedure, variable and constant types are using the indices of their
     * internal data structure. The statement types are using stmt# directly as
     * their indices.
     */
    virtual std::vector<std::string> IndexToName(std::vector<int> index_list,
                                                 PKBEntityType et) = 0;

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
