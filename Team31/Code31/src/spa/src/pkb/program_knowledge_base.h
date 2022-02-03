#ifndef SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
#define SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_

//#include "source/polish_notation.h"

#include <vector>
#include <string>

using STMT_NO = int;
using READ_STMT_NO = STMT_NO;
using PRINT_STMT_NO = STMT_NO;
using CALL_STMT_NO = STMT_NO;
using WHILE_STMT_NO = STMT_NO;
using IF_STMT_NO = STMT_NO;
using ASSIGN_STMT_NO = STMT_NO;
//using PN = PolishNotation;

namespace pkb {

class ProgramKnowledgeBase {
  public:
    ProgramKnowledgeBase() = delete;

    ProgramKnowledgeBase(ProgramKnowledgeBase &&) = delete;

    ProgramKnowledgeBase(const ProgramKnowledgeBase &) = delete;
    // the only valid constructor is one that has all the init parameters

    void init(std::vector<std::string> procedures, std::vector<std::string> variables,
              std::vector<std::string> constants, std::vector<READ_STMT_NO> reads,
              std::vector<PRINT_STMT_NO> prints, std::vector<CALL_STMT_NO> calls,
              std::vector<WHILE_STMT_NO> whiles, std::vector<IF_STMT_NO> ifs,
              std::vector<ASSIGN_STMT_NO> assigns, std::vector<PN> notations);

  private:
    //vectors initialised by function init
    std::vector<std::string> procedures;
    std::vector<std::string> variables;
    std::vector<std::string> constants;
    std::vector<READ_STMT_NO> reads;
    std::vector<PRINT_STMT_NO> prints;
    std::vector<CALL_STMT_NO> calls;
    std::vector<WHILE_STMT_NO> whiles;
    std::vector<IF_STMT_NO> ifs;
    std::vector<ASSIGN_STMT_NO> assigns;
    std::vector<PN> notations;

};
}  // namespace pkb

#endif //SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
