#ifndef SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
#define SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_

class ProgramKnowledgeBase {
  public:
    ProgramKnowledgeBase() = delete;
    ProgramKnowledgeBase(ProgramKnowledgeBase &&) = delete;
    ProgramKnowledgeBase(const ProgramKnowledgeBase &) = delete;
    // the only valid constructor is one that has all the init parameters
};

#endif //SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
