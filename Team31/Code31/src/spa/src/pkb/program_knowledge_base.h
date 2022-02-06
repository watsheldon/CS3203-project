#ifndef SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
#define SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_

#include "knowledge_base.h"
#include "../polish_notation.h"
#include "container_node.h"

#include <memory>
#include <vector>
#include <string>

namespace spa {

class ProgramKnowledgeBase : public KnowledgeBase {
  public:
    ProgramKnowledgeBase() = delete;

    ProgramKnowledgeBase(ProgramKnowledgeBase &&) = delete;

    ProgramKnowledgeBase(const ProgramKnowledgeBase &) = delete;

    // the only valid constructor is one that has all the init parameters
    explicit ProgramKnowledgeBase(std::shared_ptr<Init> init);

    //set stmtLst: useful for Parent and Follows relationships
    void set_index(Index<kProc> proc_index, Index<kStmtLst> stmtlst_index);
    void set_index(Index<kWhile> stmt_no, Index<kStmtLst> stmtlst_index);
    void set_index(Index<kIf> stmt_no, Index<kStmtLst> stmtlst_index1, Index<kStmtLst> stmtlst_index2);
    void set_lst(Index<kStmtLst> stmtlst_index, std::vector<STMT_NO> stmtlst);

    //set direct Uses and Modifies relationships
    void set_rel(Index<kPrint> stmt_no, Index<kVar> var_index);

    //mark the end of source processor -> construct necessary data structures
    void set_end();

    std::vector<std::string> getAll(EntityType);


  private:
    //leaving index 0 empty for all vectors to preserve consistency

    //a pointer to store all the vectors of entities - as the parameter of class constructor
    std::shared_ptr<Init> entities_ptr_;

    //vectors resized by constructor
    std::vector<STMTLST_NO> proc_stmtlst_;
    std::vector<STMTLST_NO> while_stmtlst_;
    std::vector<STMTLST_NO> if_stmtlst_;
    std::vector<std::vector<STMTLST_NO> > stmtlsts_;
    std::vector<STMT_NO> all_stmt; // store list of all statements

    //vector to store the nesting relationships among containers
    std::vector<CN> containers_;

    //vector to map from stmt_no to internal index of different stmt types
    std::vector<INDEX> map_no_index_;
    //vector to map from stmt_no to its stmt type
    std::vector<EntityType> map_no_type_;
    //vector to map from stmt_no to its stmtlst's index
    std::vector<STMTLST_NO> map_no_lst_;

};

}  // namespace spa

#endif //SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
