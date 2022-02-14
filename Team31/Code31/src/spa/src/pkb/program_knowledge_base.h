#ifndef SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
#define SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_

#include <algorithm>
#include <iterator>
#include <list>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/polish_notation.h"
#include "pkb/store/constant_value_store.h"
#include "container_node.h"
#include "pkb/store/if_stmtlst_store.h"
#include "knowledge_base.h"
#include "pkb/store/modifies_relationship_store.h"
#include "pkb/store/procedure_name_store.h"
#include "pkb/store/procedure_stmtlst_store.h"
#include "pkb/store/stmtlst_statements_store.h"
#include "pkb/store/type_statements_store.h"
#include "pkb/store/uses_relationship_store.h"
#include "pkb/store/variable_name_store.h"
#include "pkb/store/while_stmtlst_store.h"

namespace spa {

class ProgramKnowledgeBase : public KnowledgeBase {
  public:
    ProgramKnowledgeBase() = delete;

    ProgramKnowledgeBase(ProgramKnowledgeBase &&) = delete;

    ProgramKnowledgeBase(const ProgramKnowledgeBase &) = delete;

    explicit ProgramKnowledgeBase(BasicEntities init);

    // set stmtLst: useful for Parent and Follows relationships
    void SetIndex(Index<SetEntityType::kProc> proc_index,
                  Index<SetEntityType::kStmtLst> stmtlst_index) override;
    void SetIndex(Index<SetEntityType::kStmt> stmt_no,
                  Index<SetEntityType::kStmtLst> stmtlst_index) override;
    void SetIndex(Index<SetEntityType::kStmt> stmt_no,
                  Index<SetEntityType::kStmtLst> then_index,
                  Index<SetEntityType::kStmtLst> else_index) override;
    void SetLst(Index<SetEntityType::kStmtLst> stmtlst_index,
                std::vector<int> stmtlst) override;

    // set direct Uses and Modifies relationships
    void SetRel(Index<SetEntityType::kStmt> stmt_no,
                Index<SetEntityType::kVar> var_index) override;
    void SetRel(Index<SetEntityType::kStmt> stmt_no,
                std::vector<int> var_index) override;

    bool ExistModifies(int stmt_no, int var_index) override;
    bool ExistUses(int stmt_no, int var_index) override;

    std::vector<int> GetModifies(Index<QueryEntityType::kStmt> stmt_no,
                                 const std::vector<int> &filtered_var) override;
    std::vector<int> GetModifies(
            Index<QueryEntityType::kVar> var_index,
            const std::vector<int> &filtered_stmt) override;
    std::vector<int> GetUses(Index<QueryEntityType::kStmt> stmt_no,
                             const std::vector<int> &filtered_var) override;
    std::vector<int> GetUses(Index<QueryEntityType::kVar> var_index,
                             const std::vector<int> &filtered_stmt) override;

    std::vector<int> GetAllEntityIndices(QueryEntityType et) override;
    std::vector<int> GetAllEntityIndices(StmtType st) override;

    void IndexToName(QueryEntityType et, const std::vector<int> &index_list,
                     std::list<std::string> &names) override;

    // mark the end of source processor
    void Compile() override;

  private:
    bool compiled = false;
    const int STMT_TYPE_COUNT = 6;

    std::vector<CN> containers_;  // store relationships among containers

    size_t stmt_count_;     // store number of stmt
    size_t stmtlst_count_;  // store number of stmtlst

    ProcedureNameStore proc_name_;
    VariableNameStore var_name_;
    ConstantValueStore const_value_;

    ProcedureStmtlstStore proc_stmtlst_;
    WhileStmtlstStore while_stmtlst_;
    IfStmtlstStore if_stmtlst_;
    StmtlstStatementsStore stmtlst_stmt_;
    TypeStatementsStore type_stmt_;
    ModifiesRelationshipStore modifies_rel_;
    UsesRelationshipStore uses_rel_;
};

}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
