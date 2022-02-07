#include "source_processor.h"
#include "pkb/program_knowledge_base.h"

namespace spa {
SourceProcessor::SourceProcessor(const std::string &filename)
        : source_path_(filename), tokenizer_(source_path_) {}

std::shared_ptr<spa::ProgramKnowledgeBase> SourceProcessor::Parse() {
    return nullptr;
}
}
