#ifndef SRC_SPA_SRC_SOURCE_SOURCE_PROCESSOR_H_
#define SRC_SPA_SRC_SOURCE_SOURCE_PROCESSOR_H_

#include <filesystem>
#include <memory>
#include <string>

#include "ast/abstract_syntax_tree.h"

namespace spa {
class SourceProcessor {
  public:
    explicit SourceProcessor(const std::string &filename);
    std::unique_ptr<AbstractSyntaxTree> Parse();

  private:
    std::filesystem::path source_path_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_SOURCE_PROCESSOR_H_
