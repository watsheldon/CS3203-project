#include "result_table.h"

#include <algorithm>
#include <cassert>
#include <iterator>

#include "qps/synonym.h"

namespace spa {
ResultTable::ResultTable(bool found_result) : has_result_(found_result) {}
ResultTable::ResultTable(Synonym *synonym, std::set<int> &&domain)
        : has_result_(!domain.empty()),
          first_(std::make_unique<VariableColumn>(synonym, std::move(domain))) {
}
ResultTable::ResultTable(Synonym *synonym_1, std::vector<int> &&column_1,
                         Synonym *synonym_2, std::vector<int> &&column_2)
        : has_result_(!column_1.empty()),
          first_(std::make_unique<VariableColumn>(synonym_1,
                                                  std::move(column_1))) {}
const VariableColumn *ResultTable::Update(const VariableColumn *other) {
    assert(second_);
    assert(other->synonym == first_->synonym ||
           other->synonym == second_->synonym);
    const auto &[changed, affected] =
            other->synonym == first_->synonym
                    ? std::make_pair(first_.get(), second_.get())
                    : std::make_pair(second_.get(), first_.get());

    std::set<int> intersect;
    std::set_intersection(changed->domain.begin(), changed->domain.end(),
                          other->domain.begin(), other->domain.end(),
                          std::inserter(intersect, intersect.begin()));
    if (changed->domain == intersect) {
        return affected;
    }

    std::vector<int> changed_col, affected_col;
    std::set<int> affected_domain;
    for (int i = 0; i < changed->column.size(); ++i) {
        if (intersect.find(changed->column[i]) != intersect.end()) {
            changed_col.emplace_back(changed->column[i]);
            affected_col.emplace_back(affected->column[i]);
            affected_domain.emplace(affected->column[i]);
        }
    }
    changed->Update(std::move(intersect), std::move(changed_col));
    affected->Update(std::move(affected_domain), std::move(affected_col));
    return affected;
}
void ResultTable::Update(const ResultTable &other) {
    const auto &[other_first, other_second] =
            other.first_->synonym == first_->synonym
                    ? std::make_pair(other.first_.get(), other.second_.get())
                    : std::make_pair(other.second_.get(), other.first_.get());
    assert(other_first->synonym == first_->synonym &&
           other_second->synonym == second_->synonym);
    std::set<int> first_intersect =
                          Intersect(first_->domain, other_first->domain),
                  second_intersect =
                          Intersect(second_->domain, other_second->domain);
    if (first_intersect.empty() || second_intersect.empty()) {
        Clear();
        return;
    }

    const auto n_row = 1 + *std::max_element(first_intersect.begin(),
                                             first_intersect.end());
    const auto n_col = 1 + *std::max_element(second_intersect.begin(),
                                             second_intersect.end());
    Bitmap bitmap(n_row, std::vector<bool>(n_col, false));
    const auto first_min = *first_intersect.begin(),
               first_max = *first_intersect.rbegin(),
               second_min = *second_intersect.begin(),
               second_max = *second_intersect.rbegin();
    for (int i = 0; i < first_->column.size(); ++i) {
        const auto first = first_->column[i];
        const auto second = second_->column[i];
        if (first < first_min || first > first_max || second < second_min ||
            second > second_max) {
            continue;
        }
        bitmap[first][second] = true;
    }

    std::vector<int> first_col, second_col;
    std::set<int> first_dom, second_dom;
    for (int i = 0; i < other_first->column.size(); ++i) {
        const auto first = other_first->column[i];
        const auto second = other_second->column[i];
        if (first < first_min || first > first_max || second < second_min ||
            second > second_max) {
            continue;
        }
        if (bitmap[first][second]) {
            first_col.emplace_back(first);
            first_dom.emplace(first);
            second_col.emplace_back(second);
            second_dom.emplace(second);
        }
    }
    first_->Update(std::move(first_dom), std::move(first_col));
    second_->Update(std::move(second_dom), std::move(second_col));
}
std::set<int> ResultTable::Intersect(const std::set<int> &first,
                                     const std::set<int> &second) {
    std::set<int> intersection;
    std::set_intersection(first.begin(), first.end(), second.begin(),
                          second.end(),
                          std::inserter(intersection, intersection.begin()));
    return intersection;
}
void ResultTable::Clear() {
    first_->Clear();
    second_->Clear();
}
}  // namespace spa
