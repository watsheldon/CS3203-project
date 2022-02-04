#ifndef SRC_SPA_SRC_PKB_KNOWLEDGE_BASE_H_
#define SRC_SPA_SRC_PKB_KNOWLEDGE_BASE_H_

namespace spa {

class KnowledgeBase {
  public:
    virtual void set() = 0;
    virtual void set_end() = 0;
};

}  // namespace pkb

#endif //SRC_SPA_SRC_PKB_KNOWLEDGE_BASE_H_
