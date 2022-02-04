#include "program_knowledge_base.h"
using namespace spa;

ProgramKnowledgeBase::ProgramKnowledgeBase(std::vector<std::string> procedures,
                                           std::vector<std::string> variables,
                                           std::vector<std::string> constants,
                                           std::vector<READ_STMT_NO> reads,
                                           std::vector<PRINT_STMT_NO> prints,
                                           std::vector<CALL_STMT_NO> calls,
                                           std::vector<WHILE_STMT_NO> whiles,
                                           std::vector<IF_STMT_NO> ifs,
                                           std::vector<ASSIGN_STMT_NO> assigns,
                                           std::vector<PN> notations) {
    set_init(procedures, variables, constants, reads, prints,
             calls, whiles, ifs, assigns, notations);
}

void ProgramKnowledgeBase::set_init(std::vector<std::string> procedures,
                                    std::vector<std::string> variables,
                                    std::vector<std::string> constants,
                                    std::vector<READ_STMT_NO> reads,
                                    std::vector<PRINT_STMT_NO> prints,
                                    std::vector<CALL_STMT_NO> calls,
                                    std::vector<WHILE_STMT_NO> whiles,
                                    std::vector<IF_STMT_NO> ifs,
                                    std::vector<ASSIGN_STMT_NO> assigns,
                                    std::vector<PN> notations) {
    procedures = procedures;
    variables = variables;
    constants = constants;
    reads = reads;
    prints = prints;
    calls = calls;
    whiles = whiles;
    ifs = ifs;
    assigns = assigns;
    notations = notations;
}

