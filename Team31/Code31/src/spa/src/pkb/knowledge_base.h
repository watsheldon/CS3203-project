#ifndef SRC_SPA_SRC_PKB_KNOWLEDGE_BASE_H_
#define SRC_SPA_SRC_PKB_KNOWLEDGE_BASE_H_

#include <list>
#include <string>
#include <vector>

#include "common/entity_type_enum.h"
#include "container_node.h"
#include "common/polish_notation.h"

namespace spa {

using PN = spa::PolishNotation;
using CN = spa::ContainerNode;

template <EntityType>
struct Index {
    int value = 0;
};

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
    virtual void SetIndex(Index<EntityType::kProc> proc_index,
                          Index<EntityType::kStmtLst> stmtlst_index) = 0;

    /**
     * Links stmt# of while statement with its statement list's index
     * containing stmt# (only stmt at the outermost same level) inside while
     * loop.
     */
    virtual void SetIndex(Index<EntityType::kStmt> while_stmt,
                          Index<EntityType::kStmtLst> stmtlst_index) = 0;

    /**
     * Links stmt# of if statement with its two statement lists' indices
     * containing stmt# (only stmt at the outermost same level) inside then
     * clause and else clause respectively.
     */
    virtual void SetIndex(Index<EntityType::kStmt> if_stmt,
                          Index<EntityType::kStmtLst> then_index,
                          Index<EntityType::kStmtLst> else_index) = 0;

    /**
     * Stores each statement list to its respective index.
     */
    virtual void SetLst(Index<EntityType::kStmtLst> stmtlst_index,
                        std::vector<int> stmtlst) = 0;

    /**
     * Sets direct Uses relationships between print stmt# and its variable's
     * index
     */
    virtual void SetRel(Index<EntityType::kStmt> stmt_no,
                        Index<EntityType::kVar> var_index) = 0;

    /**
     * Gets all indices of the given entity type or stmt type
     */
    virtual std::vector<int> GetAllEntityIndices(EntityType et) = 0;
    virtual std::vector<int> GetAllEntityIndices(StmtType st) = 0;

    /**
     * Converts the entities to string according to their respective indices.
     * Procedure, variable and constant types are using the indices of their
     * internal data structure. The statement types are using stmt# directly as
     * their indices.
     */
    virtual void IndexToName(EntityType et, const std::vector<int> &index_list,
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
