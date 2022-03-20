#include "query_processing_subsystem.h"

#include <list>
#include <memory>
#include <string_view>
#include <utility>

#include "pkb/knowledge_base.h"
#include "qps/evaluator/query_evaluator.h"

namespace spa {
void QueryProcessingSubsystem::Use(
        std::unique_ptr<KnowledgeBase> pkb_ptr) noexcept {
    evaluator_ = std::make_unique<QueryEvaluator>(std::move(pkb_ptr));
}

void QueryProcessingSubsystem::Evaluate(
        std::string_view query_string,
        std::list<std::string> &result) noexcept {
    if (!evaluator_) return;
    auto tokens = validator_.Validate(query_string);

    // there are only tokens if the query is syntactically correct
    if (tokens.empty()) return;

    auto query = generator_.Generate(tokens);
    if (query) {
        evaluator_->Evaluate(*query, result);
    }
}
}  // namespace spa
