#ifndef SRC_SPA_SRC_SOURCE_SOURCE_PROCESSOR_H_
#define SRC_SPA_SRC_SOURCE_SOURCE_PROCESSOR_H_

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

#include "pkb/knowledge_base.h"

namespace spa {
class SourceProcessor {
  public:
    explicit SourceProcessor(const std::string &filename);
    std::shared_ptr<spa::KnowledgeBase> Parse();

  private:
    std::filesystem::path source_path_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_SOURCE_PROCESSOR_H_
