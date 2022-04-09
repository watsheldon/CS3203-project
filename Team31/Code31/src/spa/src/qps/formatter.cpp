#include "formatter.h"

#include <algorithm>
#include <iterator>
#include <list>
#include <string>
#include <vector>

#include "common/aliases.h"
#include "common/bitvec2d.h"
#include "evaluator/query_evaluator.h"
#include "evaluator/result_table.h"
#include "pkb/knowledge_base.h"
#include "synonym.h"

namespace spa {
Formatter::Formatter(QueryEvaluator::ResultsView results_view) noexcept
        : results_valid_(results_view.has_valid_result),
          synonym_domains_(results_view.synonym_domain),
          result_tables_(results_view.result_tables),
          vartable_map_(results_view.vartable_map) {}
void Formatter::Use(KnowledgeBase *pkb) noexcept { pkb_ = pkb; }
void Formatter::OutputResults(
        std::list<std::string> &results,
        const std::vector<SynonymWithAttr> &selected) noexcept {
    if (selected.front().synonym_ == nullptr) {
        results.emplace_back(results_valid_ ? kTrue : kFalse);
        return;
    }
    if (!results_valid_) return;
    Reset();
    return selected.size() > 1 ? OutputTuple(results, selected)
                               : OutputSingle(results, selected.front());
}
void Formatter::OutputSingle(std::list<std::string> &results,
                             const SynonymWithAttr &syn_attr) noexcept {
    free_syns_.emplace_back(syn_attr.synonym_);
    PopulateFreeSyns();
    output_columns_.emplace(syn_attr.synonym_,
                            std::move(free_values_[syn_attr.synonym_]));
    results.resize(output_columns_.begin()->second.size());
    ExportColumn(results, syn_attr);
}
void Formatter::OutputTuple(
        std::list<std::string> &results,
        const std::vector<SynonymWithAttr> &selected) noexcept {
    ExtractSynonyms(selected);
    ExpandConstrainedVarTables();
    PopulateFreeSyns();
    std::vector<int> row;
    ExpandOutputColumns(row);
    Export(results, selected);
}
void Formatter::ExpandConstrainedVarTables() noexcept {
    expanded_results_.resize(constrained_syns_.size());
    for (auto &row : expanded_results_) {
        row.resize(constrained_syns_.size());
    }
    for (int i = 0; i + 1 < constrained_syns_.size(); ++i) {
        const auto &table_indices = vartable_map_.at(constrained_syns_[i]);
        for (int idx : table_indices) {
            auto [first, second] = result_tables_[idx].GetSynonyms();
            auto other = first == constrained_syns_[i] ? second : first;
            auto it = std::find(constrained_syns_.begin() + i + 1,
                                constrained_syns_.end(), other);
            if (it == constrained_syns_.end()) continue;
            auto j = it - constrained_syns_.begin();
            ExpandResultTable({i, j}, idx);
        }
    }
}
bool Formatter::SatisfiableConstraints(std::vector<int> &row) {
    auto idx = row.size();
    if (idx == constrained_syns_.size()) return true;
    for (int v : synonym_domains_.at(constrained_syns_[idx])) {
        bool compatible = true;
        for (int j = 0; j < idx; ++j) {
            if (expanded_results_[j][idx] != nullptr &&
                !expanded_results_[j][idx]->At(row[j], v)) {
                compatible = false;
                break;
            }
        }
        if (!compatible) continue;
        row.emplace_back(v);
        bool satisfiable = SatisfiableConstraints(row);
        row.pop_back();
        if (satisfiable) return true;
    }
    return false;
}
void Formatter::ExpandOutputColumns(std::vector<int> &row) noexcept {
    auto idx = row.size();
    if (idx == num_constrained_selected_) {
        if (!SatisfiableConstraints(row)) return;
        for (int i = 0; i < row.size(); ++i) {
            output_columns_[constrained_syns_[i]].emplace_back(row[i]);
        }
        AddFreeSynsToOutput(row);
        return;
    }
    for (int v : synonym_domains_.at(constrained_syns_[idx])) {
        bool compatible = true;
        for (int j = 0; j < idx; ++j) {
            if (expanded_results_[j][idx] != nullptr &&
                !expanded_results_[j][idx]->At(row[j], v)) {
                compatible = false;
                break;
            }
        }
        if (!compatible) continue;
        row.emplace_back(v);
        ExpandOutputColumns(row);
        row.pop_back();
    }
}
void Formatter::ExpandResultTable(PairOf<size_t> positions,
                                  int table_idx) noexcept {
    const auto [i, j] = positions;
    expanded_results_[i][j] = std::make_unique<BitVec2D>(
            *synonym_domains_.at(constrained_syns_[i]).rbegin() + 1,
            *synonym_domains_.at(constrained_syns_[j]).rbegin() + 1);
    const auto first = result_tables_[table_idx].GetSynonyms().first;
    const auto &col_1 =
            first == constrained_syns_[j]
                    ? result_tables_[table_idx].GetSecondColumn()->column
                    : result_tables_[table_idx].GetFirstColumn()->column;
    const auto &col_2 =
            first == constrained_syns_[j]
                    ? result_tables_[table_idx].GetFirstColumn()->column
                    : result_tables_[table_idx].GetSecondColumn()->column;
    for (int k = 0; k < col_1.size(); ++k) {
        expanded_results_[i][j]->Set(col_1[k], col_2[k]);
    }
}
void Formatter::ExtractSynonyms(
        const std::vector<SynonymWithAttr> &selected) noexcept {
    std::list<const Synonym *> unique_synonyms;
    std::transform(selected.begin(), selected.end(),
                   std::back_inserter(unique_synonyms),
                   [](const SynonymWithAttr &s) { return s.synonym_; });
    unique_synonyms.sort();
    unique_synonyms.unique();
    for (auto it = unique_synonyms.begin(); it != unique_synonyms.end();) {
        if (synonym_domains_.find(*it) == synonym_domains_.end() ||
            vartable_map_.at(*it).empty()) {
            free_syns_.emplace_back(*it);
            it = unique_synonyms.erase(it);
        } else {
            ++it;
        }
    }
    num_constrained_selected_ = (int)unique_synonyms.size();
    auto &cs = constrained_syns_;
    cs.insert(cs.end(), unique_synonyms.begin(), unique_synonyms.end());
    std::sort(cs.begin(), cs.end(), [this](auto syn_a, auto syn_b) {
        return synonym_domains_.at(syn_a).size() <
               synonym_domains_.at(syn_b).size();
    });
    for (const auto &entry : vartable_map_) {
        auto &[syn, related_vartables] = entry;
        auto end = cs.begin() + num_constrained_selected_;
        auto it = std::find(cs.begin(), end, syn);
        if (it == end && !related_vartables.empty()) {
            cs.emplace_back(syn);
        }
    }
    std::sort(cs.begin() + num_constrained_selected_, cs.end(),
              [this](auto syn_a, auto syn_b) {
                  return synonym_domains_.at(syn_a).size() <
                         synonym_domains_.at(syn_b).size();
              });
}
std::vector<int> Formatter::GetAllIndices(Synonym::Type type) const noexcept {
    QueryEntityType element_type = ToNamedElementType(type);
    if (element_type == QueryEntityType::kStmt) {
        return pkb_->GetAllEntityIndices(static_cast<StmtType>(type));
    }
    return pkb_->GetAllEntityIndices(element_type);
}
void Formatter::PopulateFreeSyns() noexcept {
    for (auto syn : free_syns_) {
        auto domain = synonym_domains_.find(syn);
        auto values = domain == synonym_domains_.end()
                              ? GetAllIndices(syn->type)
                              : std::vector<int>(domain->second.begin(),
                                                 domain->second.end());
        free_values_.emplace(syn, std::move(values));
    }
    std::sort(free_syns_.begin(), free_syns_.end(),
              [this](auto syn_a, auto syn_b) {
                  return free_values_.at(syn_a).size() <
                         free_values_.at(syn_b).size();
              });
}
void Formatter::AddFreeSynsToOutput(std::vector<int> &row) noexcept {
    auto idx = row.size() - num_constrained_selected_;
    if (idx + 1 > free_syns_.size()) return;
    if (idx + 1 == free_syns_.size()) {
        const auto &values = free_values_.at(free_syns_[idx]);
        auto &target_column = output_columns_[free_syns_[idx]];
        target_column.insert(target_column.end(), values.begin(), values.end());
        for (int i = 0; i < num_constrained_selected_; ++i) {
            output_columns_[constrained_syns_[i]].resize(target_column.size(),
                                                         row[i]);
        }
        for (int i = 0; i < idx; ++i) {
            output_columns_[free_syns_[i]].resize(
                    target_column.size(), row[num_constrained_selected_ + i]);
        }
        return;
    }
    for (int val : free_values_.at(free_syns_[idx])) {
        row.emplace_back(val);
        AddFreeSynsToOutput(row);
        row.pop_back();
    }
}
void Formatter::Reset() noexcept {
    constrained_syns_.clear();
    free_syns_.clear();
    free_values_.clear();
    expanded_results_.clear();
    output_columns_.clear();
}
void Formatter::Export(
        std::list<std::string> &results,
        const std::vector<SynonymWithAttr> &selected) const noexcept {
    results.resize(output_columns_.begin()->second.size());
    for (const auto &syn_attr : selected) {
        ExportColumn(results, syn_attr);
    }
}
void Formatter::ExportColumn(std::list<std::string> &results,
                             const SynonymWithAttr &syn_attr) const noexcept {
    auto [syn, attr] = syn_attr;
    QueryEntityType element_type = ToNamedElementType(syn->type);
    const auto &column = output_columns_.at(syn);
    if (element_type == QueryEntityType::kStmt &&
        (attr == Attribute::kNone || attr == Attribute::kStmtNum)) {
        Append(results, column);
    } else if (element_type == QueryEntityType::kStmt) {
        auto attrs = pkb_->GetAttr(column, static_cast<StmtType>(syn->type));
        Append(results, attrs);
    } else {
        auto names = pkb_->GetNames(column, element_type);
        Append(results, names);
    }
}
void Formatter::Append(std::list<std::string> &results,
                       const std::vector<int> &indices) noexcept {
    int i = 0;
    assert(indices.size() == results.size());
    for (auto &entry : results) {
        if (!entry.empty()) entry.append(" ");
        entry.append(std::to_string(indices[i++]));
    }
}
void Formatter::Append(std::list<std::string> &results,
                       const std::vector<std::string_view> &names) {
    int i = 0;
    assert(names.size() == results.size());
    for (auto &entry : results) {
        if (!entry.empty()) entry.append(" ");
        entry.append(names[i++]);
    }
}
}  // namespace spa
