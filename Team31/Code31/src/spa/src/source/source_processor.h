#ifndef SRC_SPA_SRC_SOURCE_SOURCE_PROCESSOR_H_
#define SRC_SPA_SRC_SOURCE_SOURCE_PROCESSOR_H_

#include <string>
#include <memory>
#include <fstream>

#include "pkb/program_knowledge_base.h"

class SourceProcessor {
  public:
    explicit SourceProcessor(const std::string &filename);
    std::shared_ptr<ProgramKnowledgeBase> parse();
  private:
    std::ifstream source_;
};

#endif //SRC_SPA_SRC_SOURCE_SOURCE_PROCESSOR_H_
