#ifndef SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
#define SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_

#include <algorithm>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include "container_node.h"
#include "knowledge_base.h"
#include "polish_notation.h"

namespace spa {

class ProgramKnowledgeBase : public KnowledgeBase {
  public:
    ProgramKnowledgeBase() = delete;

    ProgramKnowledgeBase(ProgramKnowledgeBase &&) = delete;

    ProgramKnowledgeBase(const ProgramKnowledgeBase &) = delete;

    // the only valid constructor is one that has all the init parameters
    explicit ProgramKnowledgeBase(std::shared_ptr<BasicEntities> init);

    // set stmtLst: useful for Parent and Follows relationships
    void SetIndex(Index<kProc> proc_index,
                  Index<kStmtLst> stmtlst_index) override;
    void SetIndex(Index<kWhileStmt> stmt_no,
                  Index<kStmtLst> stmtlst_index) override;
    void SetIndex(Index<kIfStmt> stmt_no, Index<kStmtLst> stmtlst_index1,
                  Index<kStmtLst> stmtlst_index2) override;
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

    const int STMT_TYPE_COUNT = 6;

    // mark the end of source processor -> construct necessary data structures
    void Compile() override;

  private:
    // leaving index 0 empty for all vectors to preserve consistency

    // a pointer to store all the vectors of entities - as the parameter of
    // class constructor
    std::shared_ptr<BasicEntities> entities_ptr_;

    // vectors resized by constructor
    std::vector<STMTLST_NO> proc_stmtlst_;
    std::vector<STMTLST_NO> while_stmtlst_;
    std::vector<STMTLST_NO> if_stmtlst_;
    std::vector<std::vector<STMTLST_NO> > stmtlsts_;
    size_t stmt_size_;  // store number of stmt

    // vector to store the nesting relationships among containers
    std::vector<CN> containers_;

    // vector to map from stmt_no to internal index of different stmt types
    std::vector<INDEX> map_no_index_;
    // vector to map from stmt_no to its stmt type
    std::vector<PKBEntityType> map_no_type_;
    // vector to map from stmt_no to its stmtlst's index
    std::vector<STMTLST_NO> map_no_lst_;
};

}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
