#ifndef SRC_SPA_SRC_PKB_KNOWLEDGE_BASE_H_
#define SRC_SPA_SRC_PKB_KNOWLEDGE_BASE_H_

#include "../polish_notation.h"
#include "container_node.h"

#include <vector>
#include <string>

namespace spa {

using STMT_NO = int;
using STMTLST_NO = int;
using PN = spa::PolishNotation;
using INDEX = int;
using CN = spa::ContainerNode;

struct BasicEntities {
    std::vector<std::string> procedures;
    std::vector<std::string> variables;
    std::vector<std::string> constants;
    std::vector<STMT_NO> reads;
    std::vector<STMT_NO> prints;
    std::vector<STMT_NO> calls;
    std::vector<STMT_NO> whiles;
    std::vector<STMT_NO> ifs;
    std::vector<STMT_NO> assigns;
    //each notation is unique
    //notations may not have the same size as assigns
    std::vector<PN> notations;
};

enum EntityType {
    kProc, kVar, kConst,
    kRead, kPrint, kCall,
    kWhile, kIf, kAssign,
    kStmt, kStmtLst, kNotation
};

//index starts from zero (direct mapping with vector index)
template<EntityType e>
struct Index {
    int value;
};

class KnowledgeBase {
  public:
    /**
     * Prevents any further updates to the KnowledgeBase and compile all
     * indirect relationships for faster lookup.
     */
    virtual void Compile() = 0;
    virtual ~KnowledgeBase() = 0;
};

}  // namespace spa

#endif //SRC_SPA_SRC_PKB_KNOWLEDGE_BASE_H_
