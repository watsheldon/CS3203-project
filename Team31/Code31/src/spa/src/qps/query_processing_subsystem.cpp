#include "query_processing_subsystem.h"

#include <list>
#include <memory>
#include <string_view>
#include <utility>

namespace spa {
void QueryProcessingSubsystem::use(std::unique_ptr<KnowledgeBase> pkb_ptr) {
    pkb_ = std::move(pkb_ptr);
}

void QueryProcessingSubsystem::evaluate(std::string_view query_string,
                                        std::list<std::string> &list) {}
}  // namespace spa
