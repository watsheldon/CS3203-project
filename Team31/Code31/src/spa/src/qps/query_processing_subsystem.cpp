#include <memory>
#include <string_view>
#include <list>
#include "query_processing_subsystem.h"
#include "../pkb/program_knowledge_base.h"
#include "evaluator/evaluator.h"

void QueryProcessingSubsystem::use(const std::shared_ptr<spa::ProgramKnowledgeBase> &pkb_ptr) {
    pkb_ = pkb_ptr;
}

void QueryProcessingSubsystem::evaluate(std::string_view query_string, std::list<std::string> &list) {
    //evaluator::evaluateQuery();
}
