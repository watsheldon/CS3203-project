#include "query_evaluator.h"

#include <algorithm>
#include <cassert>
#include <list>
#include <queue>
#include <string>

#include "pkb/knowledge_base.h"
#include "qps/conditions/condition_clause.h"
#include "qps/query_object.h"
#include "qps/synonym.h"

namespace spa {
QueryEvaluator::QueryEvaluator(std::unique_ptr<KnowledgeBase> knowledge_base_)
        : knowledge_base_(std::move(knowledge_base_)) {}
void QueryEvaluator::Evaluate(const QueryObject& query,
                              std::list<std::string>& list) {
    Clear();
    std::queue<const ConditionClause*> queue;
    const auto& clauses = query.clauses;
    std::for_each(clauses.begin(), clauses.end(),
                  [&queue](const auto& c) { queue.emplace(c.get()); });
    // todo: make the clauses comparable by using the sum of the ref_count of
    // each synonym, overload the operator< since that's what stl uses by
    // default, then change to a priority queue
    while (!queue.empty()) {
        auto vartable = queue.front()->Execute(knowledge_base_.get());
        if (!vartable.has_result || !UpdateResult(vartable)) {
            return;
        }
        queue.pop();
    }
    Populate(list, query.select);
}
bool QueryEvaluator::UpdateResult(ResultTable& result_table) {
    if (!result_table.has_result) {
        return false;
    }
    switch (result_table.type) {
        case ResultTable::kBool:
            return true;
        case ResultTable::kSingle: {
            if (!UpdateSingle(*result_table.GetFirstColumn())) {
                return false;
            }
            break;
        }
        case ResultTable::kDouble: {
            if (!UpdateDouble(result_table)) {
                return false;
            }
            break;
        }
    }
    return Propagate();
}
bool QueryEvaluator::UpdateSingle(const VariableColumn& column) {
    const auto& [itr, inserted] =
            domains_.try_emplace(column.synonym, column.domain);
    if (inserted) {
        vartable_map_.emplace(column.synonym, std::vector<ResultTable*>());
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
bool QueryEvaluator::UpdateDouble(ResultTable& result_table) {
    const auto [syn_a, syn_b] = result_table.GetSynonyms();
    bool exist_a = domains_.find(syn_a) != domains_.end();
    bool exist_b = domains_.find(syn_b) != domains_.end();
    auto column_a = result_table.GetFirstColumn();
    auto column_b = result_table.GetSecondColumn();
    if (!exist_a && !exist_b) {
        domains_.emplace(syn_a, column_a->domain);
        domains_.emplace(syn_b, column_b->domain);
        vartables_.emplace_back(std::move(result_table));
        return true;
    }

    if (!exist_a || !exist_b) {
        auto [syn_existing, syn_new, column_existing] =
                exist_a ? std::make_tuple(syn_a, syn_b, column_a)
                        : std::make_tuple(syn_b, syn_a, column_b);
        auto intersect = ResultTable::Intersect(domains_[syn_existing],
                                                column_existing->domain);
        domains_.emplace(syn_new, result_table.Update(syn_existing, intersect));
        domains_[syn_existing].swap(intersect);
    }
    auto [syn_rare, syn_more] =
            vartable_map_[syn_a].size() < vartable_map_[syn_b].size()
                    ? std::make_pair(syn_a, syn_b)
                    : std::make_pair(syn_b, syn_a);
    auto& rare_tables = vartable_map_[syn_rare];
    auto other = syn_more;
    auto table_itr = std::find_if(rare_tables.begin(), rare_tables.end(),
                                  [other](ResultTable* vt) {
                                      auto [x, y] = vt->GetSynonyms();
                                      return x == other || y == other;
                                  });
    const auto& [domain_a, domain_b] =
            table_itr != rare_tables.end()
                    ? (**table_itr).Update(result_table)
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
    if (table_itr == rare_tables.end()) {
        vartables_.emplace_back(std::move(result_table));
    }

    return true;
}
bool QueryEvaluator::Propagate() {
    while (!update_queue_.empty()) {
        auto curr = update_queue_.begin();
        auto synonym = *curr;
        for (auto table : vartable_map_[synonym]) {
            auto [syn_a, syn_b] = table->GetSynonyms();
            auto syn_other = synonym == syn_a ? syn_b : syn_a;
            auto domain_other = table->Update(synonym, domains_[synonym]);
            if (domain_other.empty()) {
                return false;
            }
            if (domain_other.size() < domains_[syn_other].size()) {
                domains_[synonym].swap(domain_other);
                update_queue_.emplace(syn_other);
            }
        }
        update_queue_.erase(curr);
    }
    return true;
}
void QueryEvaluator::Populate(std::list<std::string>& list,
                              const Synonym* selected) {
    auto domain = domains_.find(selected);
    switch (selected->type) {
        case Synonym::kStmtAny: {
            auto stmt_nos = domain == domains_.end()
                                    ? knowledge_base_->GetAllEntityIndices(
                                              StmtType::kAll)
                                    : std::vector<int>(domain->second.begin(),
                                                       domain->second.end());
            knowledge_base_->IndexToName(QueryEntityType::kStmt, stmt_nos,
                                         list);
            return;
        }
        case Synonym::kStmtRead: {
            auto reads = domain == domains_.end()
                                 ? knowledge_base_->GetAllEntityIndices(
                                           StmtType::kRead)
                                 : std::vector<int>(domain->second.begin(),
                                                    domain->second.end());
            knowledge_base_->IndexToName(QueryEntityType::kStmt, reads, list);
            return;
        }
        case Synonym::kStmtPrint: {
            auto prints = domain == domains_.end()
                                  ? knowledge_base_->GetAllEntityIndices(
                                            StmtType::kPrint)
                                  : std::vector<int>(domain->second.begin(),
                                                     domain->second.end());
            knowledge_base_->IndexToName(QueryEntityType::kStmt, prints, list);
            return;
        }
        case Synonym::kStmtCall: {
            auto calls = domain == domains_.end()
                                 ? knowledge_base_->GetAllEntityIndices(
                                           StmtType::kCall)
                                 : std::vector<int>(domain->second.begin(),
                                                    domain->second.end());
            knowledge_base_->IndexToName(QueryEntityType::kStmt, calls, list);
            return;
        }
        case Synonym::kStmtWhile: {
            auto whiles = domain == domains_.end()
                                  ? knowledge_base_->GetAllEntityIndices(
                                            StmtType::kWhile)
                                  : std::vector<int>(domain->second.begin(),
                                                     domain->second.end());
            knowledge_base_->IndexToName(QueryEntityType::kStmt, whiles, list);
            return;
        }
        case Synonym::kStmtIf: {
            auto ifs = domain == domains_.end()
                               ? knowledge_base_->GetAllEntityIndices(
                                         StmtType::kIf)
                               : std::vector<int>(domain->second.begin(),
                                                  domain->second.end());
            knowledge_base_->IndexToName(QueryEntityType::kStmt, ifs, list);
            return;
        }
        case Synonym::kStmtAssign: {
            auto assigns = domain == domains_.end()
                                   ? knowledge_base_->GetAllEntityIndices(
                                             StmtType::kAssign)
                                   : std::vector<int>(domain->second.begin(),
                                                      domain->second.end());
            knowledge_base_->IndexToName(QueryEntityType::kStmt, assigns, list);
            return;
        }
        case Synonym::kVar: {
            auto vars = domain == domains_.end()
                                ? knowledge_base_->GetAllEntityIndices(
                                          QueryEntityType::kVar)
                                : std::vector<int>(domain->second.begin(),
                                                   domain->second.end());
            knowledge_base_->IndexToName(QueryEntityType::kVar, vars, list);
            return;
        }
        case Synonym::kConst: {
            auto constants = domain == domains_.end()
                                     ? knowledge_base_->GetAllEntityIndices(
                                               QueryEntityType::kConst)
                                     : std::vector<int>(domain->second.begin(),
                                                        domain->second.end());
            knowledge_base_->IndexToName(QueryEntityType::kConst, constants,
                                         list);
            return;
        }
        case Synonym::kProc: {
            auto procs = domain == domains_.end()
                                 ? knowledge_base_->GetAllEntityIndices(
                                           QueryEntityType::kProc)
                                 : std::vector<int>(domain->second.begin(),
                                                    domain->second.end());
            knowledge_base_->IndexToName(QueryEntityType::kProc, procs, list);
            return;
        }
        default:
            assert(false);
    }
}
void QueryEvaluator::Clear() {
    domains_.clear();
    vartables_.clear();
    vartable_map_.clear();
    update_queue_.clear();
}
}  // namespace spa
