#include "query_evaluator.h"

#include <algorithm>
#include <list>
#include <queue>
#include <string>

#include "pkb/knowledge_base.h"
#include "qps/conditions/condition_clause.h"
#include "qps/query_object.h"
#include "qps/synonym.h"

namespace spa {
void QueryEvaluator::Use(KnowledgeBase* pkb) noexcept { pkb_ = pkb; }
QueryEvaluator::ResultsView QueryEvaluator::GetResultsView() noexcept {
    return {has_result_, domains_, vartables_, vartable_map_};
}
void QueryEvaluator::Evaluate(const QueryObject& query,
                              std::list<std::string>& list) noexcept {
    Clear();
    std::queue<const ConditionClause*> queue;
    const auto& clauses = query.clauses;
    std::for_each(clauses.begin(), clauses.end(),
                  [&queue](const auto& c) { queue.emplace(c.get()); });
    // todo: make the clauses comparable by using the sum of the ref_count of
    // each synonym, overload the operator< since that's what stl uses by
    // default, then change to a priority queue
    while (!queue.empty()) {
        auto vartable = queue.front()->Execute(pkb_);
        if (!vartable.has_result || !UpdateResult(vartable)) {
            has_result_ = false;
            return;
        }
        queue.pop();
    }
}
bool QueryEvaluator::UpdateResult(ResultTable& result_table) noexcept {
    if (!result_table.has_result) {
        return false;
    }
    switch (result_table.type) {
        case ResultTable::kBool:
            return true;
        case ResultTable::kSingle: {
            if (!MergeUnary(*result_table.GetFirstColumn())) {
                return false;
            }
            break;
        }
        case ResultTable::kDouble: {
            if (!MergeBinary(result_table)) {
                return false;
            }
            break;
        }
    }
    return Propagate();
}
bool QueryEvaluator::MergeUnary(const VariableColumn& column) noexcept {
    const auto& [itr, inserted] =
            domains_.try_emplace(column.synonym, column.domain);
    if (inserted) {
        vartable_map_.emplace(column.synonym, std::vector<int>());
        return true;
    }
    auto& [syn, syn_domain] = *itr;
    auto intersect = ResultTable::Intersect(column.domain, syn_domain);
    if (syn_domain.size() == intersect.size()) {
        return true;
    }
    if (intersect.empty()) {
        return false;
    }
    syn_domain.swap(intersect);
    update_queue_.emplace(syn);
    return true;
}
bool QueryEvaluator::MergeBinary(ResultTable& result_table) noexcept {
    const auto [syn_a, syn_b] = result_table.GetSynonyms();
    bool exist_a = domains_.find(syn_a) != domains_.end();
    bool exist_b = domains_.find(syn_b) != domains_.end();
    if (!exist_a && !exist_b) {
        AddBinaryConstraintBothNew(result_table);
        return true;
    }
    if (!exist_a || !exist_b) {
        return AddBinaryConstraintOneNew(result_table, exist_a);
    }
    return UpdateExistingBinaryConstraint(result_table);
}
bool QueryEvaluator::UpdateExistingBinaryConstraint(ResultTable& result_table) {
    const auto [syn_a, syn_b] = result_table.GetSynonyms();
    bool a_uncommon = vartable_map_[syn_a].size() < vartable_map_[syn_b].size();
    auto syn_uncommon = a_uncommon ? syn_a : syn_b;
    auto syn_common = a_uncommon ? syn_b : syn_a;
    auto& uncommon_tables = vartable_map_[syn_uncommon];
    auto table_itr =
            std::find_if(uncommon_tables.begin(), uncommon_tables.end(),
                         [this, syn_common](int i) {
                             auto [x, y] = vartables_[i].GetSynonyms();
                             return x == syn_common || y == syn_common;
                         });
    const auto& [domain_a, domain_b] =
            table_itr != uncommon_tables.end()
                    ? vartables_[*table_itr].Update(result_table)
                    : result_table.Update(syn_a, domains_[syn_a], syn_b,
                                          domains_[syn_b]);
    if (domain_a.empty() || domain_b.empty()) {
        return false;
    }
    if (domain_a.size() < domains_[syn_a].size()) {
        domains_[syn_a] = domain_a;
        update_queue_.emplace(syn_a);
    }
    if (domain_b.size() < domains_[syn_b].size()) {
        domains_[syn_b] = domain_b;
        update_queue_.emplace(syn_b);
    }
    if (table_itr == uncommon_tables.end()) {
        vartable_map_[syn_a].emplace_back(vartables_.size());
        vartable_map_[syn_b].emplace_back(vartables_.size());
        vartables_.emplace_back(std::move(result_table));
    }
    return true;
}
bool QueryEvaluator::AddBinaryConstraintOneNew(ResultTable& result_table,
                                               bool is_first_syn_old) {
    const auto [syn_a, syn_b] = result_table.GetSynonyms();
    auto column_a = result_table.GetFirstColumn();
    auto column_b = result_table.GetSecondColumn();
    auto [syn_existing, syn_new, column_existing] =
            is_first_syn_old ? std::make_tuple(syn_a, syn_b, column_a)
                             : std::make_tuple(syn_b, syn_a, column_b);
    auto intersect = ResultTable::Intersect(domains_[syn_existing],
                                            column_existing->domain);
    domains_.emplace(syn_new, result_table.Update(syn_existing, intersect));
    if (intersect.size() < domains_[syn_existing].size()) {
        update_queue_.emplace(syn_existing);
    }
    domains_[syn_existing].swap(intersect);
    vartable_map_[syn_existing].emplace_back(vartables_.size());
    vartable_map_[syn_new].emplace_back(vartables_.size());
    vartables_.emplace_back(std::move(result_table));
    return !domains_[syn_existing].empty() && !domains_[syn_new].empty();
}
void QueryEvaluator::AddBinaryConstraintBothNew(ResultTable& result_table) {
    const auto [syn_a, syn_b] = result_table.GetSynonyms();
    auto domain_a = result_table.GetFirstColumn()->domain;
    auto domain_b = result_table.GetSecondColumn()->domain;
    domains_.emplace(syn_a, domain_a);
    domains_.emplace(syn_b, domain_b);
    vartable_map_[syn_a].emplace_back(vartables_.size());
    vartable_map_[syn_b].emplace_back(vartables_.size());
    vartables_.emplace_back(std::move(result_table));
}
bool QueryEvaluator::Propagate() noexcept {
    while (!update_queue_.empty()) {
        auto curr = update_queue_.begin();
        auto synonym = *curr;
        for (auto table_idx : vartable_map_[synonym]) {
            auto [syn_a, syn_b] = vartables_[table_idx].GetSynonyms();
            auto syn_other = synonym == syn_a ? syn_b : syn_a;
            auto domain_other =
                    vartables_[table_idx].Update(synonym, domains_[synonym]);
            if (domain_other.empty()) {
                return false;
            }
            if (domain_other.size() < domains_[syn_other].size()) {
                domains_[syn_other].swap(domain_other);
                update_queue_.emplace(syn_other);
            }
        }
        update_queue_.erase(curr);
    }
    return true;
}
void QueryEvaluator::Clear() noexcept {
    has_result_ = true;
    domains_.clear();
    vartables_.clear();
    vartable_map_.clear();
    update_queue_.clear();
}
}  // namespace spa
