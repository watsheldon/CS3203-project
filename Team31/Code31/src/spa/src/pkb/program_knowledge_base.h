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
#include "constant_value_store.h"
#include "common/polish_notation.h"
#include "container_node.h"
#include "if_stmtlst_store.h"
#include "knowledge_base.h"
#include "procedure_name_store.h"
#include "procedure_stmtlst_store.h"
#include "stmtlst_statements_store.h"
#include "type_statements_store.h"
#include "variable_name_store.h"
#include "while_stmtlst_store.h"

namespace spa {

class ProgramKnowledgeBase : public KnowledgeBase {
  public:
    ProgramKnowledgeBase() = delete;

    ProgramKnowledgeBase(ProgramKnowledgeBase &&) = delete;

    ProgramKnowledgeBase(const ProgramKnowledgeBase &) = delete;

    explicit ProgramKnowledgeBase(const std::shared_ptr<BasicEntities> &init);

    // set stmtLst: useful for Parent and Follows relationships
    void SetIndex(Index<EntityType::kProc> proc_index,
                  Index<EntityType::kStmtLst> stmtlst_index) override;
    void SetIndex(Index<EntityType::kStmt> stmt_no,
                  Index<EntityType::kStmtLst> stmtlst_index) override;
    void SetIndex(Index<EntityType::kStmt> stmt_no,
                  Index<EntityType::kStmtLst> then_index,
                  Index<EntityType::kStmtLst> else_index) override;
    void SetLst(Index<EntityType::kStmtLst> stmtlst_index,
                std::vector<int> stmtlst) override;

    // set direct Uses and Modifies relationships
    void SetRel(Index<EntityType::kStmt> stmt_no,
                Index<EntityType::kVar> var_index) override;

    std::vector<int> GetAllEntityIndices(EntityType et) override;
    std::vector<int> GetAllEntityIndices(StmtType st) override;

    void IndexToName(EntityType et, const std::vector<int> &index_list,
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
};

}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
