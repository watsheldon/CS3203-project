#ifndef SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
#define SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_

#include <algorithm>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include "constant_value_store.h"
#include "container_node.h"
#include "if_stmtlst_store.h"
#include "knowledge_base.h"
#include "polish_notation.h"
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
    void SetIndex(Index<kProc> proc_index,
                  Index<kStmtLst> stmtlst_index) override;
    void SetIndex(Index<kStmt> stmt_no, Index<kStmtLst> stmtlst_index) override;
    void SetIndex(Index<kStmt> stmt_no, Index<kStmtLst> then_index,
                  Index<kStmtLst> else_index) override;
    void SetLst(Index<kStmtLst> stmtlst_index,
                std::vector<Index<kStmt>> stmtlst) override;

    // set direct Uses and Modifies relationships
    void SetRel(Index<kStmt> stmt_no, Index<kVar> var_index) override;

    std::vector<std::string> GetAllStringEntities(
            PKBEntityType et) override;  // For procedures,variables,constants
    std::vector<Index<kStmt>> GetAllStmtEntities(
            PKBEntityType et) override;  // For stmt

    std::vector<std::string> GetProcName(
            const std::vector<Index<kProc>> &index_list) override;
    std::vector<std::string> GetVarName(
            const std::vector<Index<kVar>> &index_list) override;
    std::vector<std::string> GetConstValue(
            const std::vector<Index<kConst>> &index_list) override;
    template <PKBEntityType T>
    std::vector<std::string> IndexToName(
            const std::vector<Index<T>> &index_list);

    // mark the end of source processor
    void Compile() override;

  private:
    bool compiled = false;
    const int STMT_TYPE_COUNT = 6;

    std::vector<CN> containers_;  // store relationships among containers

    size_t stmt_size_;     // store number of stmt
    size_t stmtlst_size_;  // store number of stmtlst

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
