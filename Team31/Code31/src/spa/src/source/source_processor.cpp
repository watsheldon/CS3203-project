#include "source_processor.h"
#include "pkb/program_knowledge_base.h"

SourceProcessor::SourceProcessor(const std::string &filename) : source_(filename) {}

std::shared_ptr<ProgramKnowledgeBase> SourceProcessor::parse() {
    return nullptr;
}
