#ifndef SRC_SPA_SRC_QPS_EVALUATOR_QUERY_EVALUATOR_H_
#define SRC_SPA_SRC_QPS_EVALUATOR_QUERY_EVALUATOR_H_

#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>

#include "pkb/knowledge_base.h"
#include "qps/query_object.h"
#include "variable_column.h"

namespace spa {
class Synonym;
class QueryEvaluator {
  public:
    using SynonymDomains = std::map<const Synonym *, std::set<int>>;
    using ResultTables = std::vector<ResultTable>;
    using VarTableMap = std::map<const Synonym *, std::vector<int>>;
    struct ResultsView {
        const bool &has_valid_result;
        const SynonymDomains &synonym_domain;
        const ResultTables &result_tables;
        const VarTableMap &vartable_map;
    };
    QueryEvaluator() noexcept = default;
    void Use(KnowledgeBase *pkb) noexcept;
    void Evaluate(const QueryObject &query,
                  std::list<std::string> &list) noexcept;
    ResultsView GetResultsView() noexcept;

  private:
    bool has_result_ = false;
    KnowledgeBase *pkb_{};
    SynonymDomains domains_;
    ResultTables vartables_;
    VarTableMap vartable_map_;

    std::set<const Synonym *> update_queue_;
    bool UpdateResult(ResultTable &result_table) noexcept;
    bool MergeUnary(const VariableColumn &column) noexcept;
    bool Propagate() noexcept;
    bool MergeBinary(ResultTable &result_table) noexcept;
    void Clear() noexcept;
    void AddBinaryConstraintBothNew(ResultTable &result_table);
    bool AddBinaryConstraintOneNew(ResultTable &result_table,
                                   bool is_first_syn_old);
    bool UpdateExistingBinaryConstraint(ResultTable &result_table);
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_EVALUATOR_QUERY_EVALUATOR_H_
