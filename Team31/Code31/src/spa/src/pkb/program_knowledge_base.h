#ifndef SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
#define SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_

#include <algorithm>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include "container_node.h"
#include "if_stmtlst_store.h"
#include "knowledge_base.h"
#include "polish_notation.h"
#include "polish_notation_store.h"
#include "procedure_name_store.h"
#include "procedure_stmtlst_store.h"
#include "variable_name_store.h"
#include "while_stmtlst_store.h"

namespace spa {

class ProgramKnowledgeBase : public KnowledgeBase {
  public:
    ProgramKnowledgeBase() = delete;

    ProgramKnowledgeBase(ProgramKnowledgeBase &&) = delete;

    ProgramKnowledgeBase(const ProgramKnowledgeBase &) = delete;

    explicit ProgramKnowledgeBase(std::shared_ptr<BasicEntities> init);

    // set stmtLst: useful for Parent and Follows relationships
    void SetIndex(Index<kProc> proc_index,
                  Index<kStmtLst> stmtlst_index) override;
    void SetIndex(Index<kWhileStmt> stmt_no,
                  Index<kStmtLst> stmtlst_index) override;
    void SetIndex(Index<kIfStmt> stmt_no, Index<kStmtLst> then_index,
                  Index<kStmtLst> else_index) override;
    void SetLst(Index<kStmtLst> stmtlst_index,
                std::vector<STMT_NO> stmtlst) override;

    // set direct Uses and Modifies relationships
    void SetRel(Index<kPrintStmt> stmt_no, Index<kVar> var_index) override;

    std::vector<std::string> GetAllStringEntities(
            PKBEntityType et) override;  // For procedures,variables,constants
    std::vector<int> GetAllStmtEntities(PKBEntityType et) override;  // For stmt
    std::vector<std::string> IndexToName(
            std::vector<int> index_list,
            PKBEntityType et) override;  // convert index to string

    // mark the end of source processor
    void Compile() override;

  private:
    const int STMT_TYPE_COUNT = 6;

    std::shared_ptr<BasicEntities>
            entities_ptr_;  // initial vectors of entities

    std::vector<std::vector<STMTLST_NO> > stmtlsts_;
    std::vector<CN> containers_;  // store relationships among containers
    std::vector<PKBEntityType> map_no_type_;  // map from stmt_no to stmt type
    std::vector<STMTLST_NO> map_no_lst_;  // map from stmt_no to stmtlst's index

    size_t stmt_size_;  // store number of stmt

    ProcedureNameStore proc_name_;
    VariableNameStore var_name_;
    // PolishNotationStore polish_notation_;

    ProcedureStmtlstStore proc_stmtlst_;
    WhileStmtlstStore while_stmtlst_;
    IfStmtlstStore if_stmtlst_;
};

}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
