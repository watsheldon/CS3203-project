#ifndef SRC_SPA_SRC_SOURCE_SOURCE_PROCESSOR_H_
#define SRC_SPA_SRC_SOURCE_SOURCE_PROCESSOR_H_

#include <string>
#include <memory>
#include <fstream>
#include <filesystem>

#include "pkb/program_knowledge_base.h"
#include "tokenizer.h"

namespace spa {
class SourceProcessor {
  public:
    explicit SourceProcessor(const std::string &filename);
    std::shared_ptr<spa::ProgramKnowledgeBase> Parse();
  private:
    std::filesystem::path source_path_;
    Tokenizer tokenizer_;
};
}

#endif //SRC_SPA_SRC_SOURCE_SOURCE_PROCESSOR_H_
