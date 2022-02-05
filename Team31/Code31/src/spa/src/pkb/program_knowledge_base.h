#ifndef SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
#define SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_

#include "knowledge_base.h"
#include "polish_notation.h"
#include "container_node.h"

#include <vector>
#include <string>

using STMT_NO = int;
using STMTLST_NO = int;
using PN = spa::PolishNotation;
using INDEX = int;
using CN = spa::ContainerNode;

namespace spa {

struct Init {
    std::vector<std::string> proc_name;
    std::vector<std::string> var_name;
    std::vector<std::string> const_value;
    std::vector<STMT_NO> read_ref;
    std::vector<STMT_NO> print_ref;
    std::vector<STMT_NO> call_ref;
    std::vector<STMT_NO> while_ref;
    std::vector<STMT_NO> if_ref;
    std::vector<STMT_NO> assign_ref;
    std::vector<PN> assign_expr;
};

enum EntityType {
    kProc, kVar, kConst,
    kRead, kPrint, kCall,
    kWhile, kIf, kAssign,
    kStmt,kStmtLst
};

//index starts from zero (direct mapping with vector index)
template<EntityType e>
struct Index {
    int value;
};

class ProgramKnowledgeBase : public KnowledgeBase {
  public:
    ProgramKnowledgeBase() = delete;

    ProgramKnowledgeBase(ProgramKnowledgeBase &&) = delete;

    ProgramKnowledgeBase(const ProgramKnowledgeBase &) = delete;

    // the only valid constructor is one that has all the init parameters
    explicit ProgramKnowledgeBase(const Init& init);

    //set stmtLst: useful for Parent and Follows relationships
    void set_index(Index<kProc> proc_index, Index<kStmtLst> stmtlst_index);
    void set_index(Index<kWhile> stmt_no, Index<kStmtLst> stmtlst_index);
    void set_index(Index<kIf> stmt_no, Index<kStmtLst> stmtlst_index1, Index<kStmtLst> stmtlst_index2);
    void set_lst(Index<kStmtLst> stmtlst_index, std::vector<STMT_NO> stmtlst);

    //set direct Uses and Modifies relationships
    void set_rel(Index<kPrint> stmt_no, Index<kVar> var_index);

    //mark the end of source processor -> construct necessary data structures
    void set_end();

  private:
    //leaving index 0 empty for all vectors to preserve consistency

    //vectors initialised by constructor
    std::vector<std::string> procedures_;
    std::vector<std::string> variables_;
    std::vector<std::string> constants_;
    std::vector<STMT_NO> reads_;
    std::vector<STMT_NO> prints_;
    std::vector<STMT_NO> calls_;
    std::vector<STMT_NO> whiles_;
    std::vector<STMT_NO> ifs_;
    std::vector<STMT_NO> assigns_;
    std::vector<PN> notations_;
    std::vector<STMTLST_NO> proc_stmtlst_;
    std::vector<STMTLST_NO> while_stmtlst_;
    std::vector<STMTLST_NO> if_stmtlst_;
    std::vector<std::vector<STMTLST_NO>> stmtlsts_;
    std::vector<INDEX> map_no_index_;
    std::vector<EntityType> map_no_type_;
    std::vector<STMTLST_NO> map_no_lst_;

    //vectors constructed at the end
    std::vector<CN> containers_;

};

}  // namespace spa

#endif //SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
