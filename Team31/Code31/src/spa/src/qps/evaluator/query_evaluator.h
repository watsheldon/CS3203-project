#ifndef SRC_SPA_SRC_QPS_EVALUATOR_QUERY_EVALUATOR_H_
#define SRC_SPA_SRC_QPS_EVALUATOR_QUERY_EVALUATOR_H_

#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>

#include "pkb/knowledge_base.h"
#include "qps/query_object.h"

namespace spa {
class Synonym;
class QueryEvaluator {
  public:
    explicit QueryEvaluator(
            std::unique_ptr<KnowledgeBase> knowledge_base) noexcept;
    void Evaluate(const QueryObject &query,
                  std::list<std::string> &list) noexcept;

  private:
    const std::unique_ptr<KnowledgeBase> knowledge_base_;
    std::map<const Synonym *, std::set<int>> domains_;
    std::vector<ResultTable> vartables_;
    std::map<const Synonym *, std::vector<int>> vartable_map_;
    std::set<const Synonym *> update_queue_;

    void Populate(std::list<std::string> &list,
                  const Synonym *selected) noexcept;
    bool UpdateResult(ResultTable &result_table) noexcept;
    bool UpdateSingle(const VariableColumn &column) noexcept;
    bool Propagate() noexcept;
    bool UpdateDouble(ResultTable &i) noexcept;
    void Clear() noexcept;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_EVALUATOR_QUERY_EVALUATOR_H_
