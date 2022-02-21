#include "result_table.h"

#include <algorithm>
#include <cassert>
#include <iterator>

#include "qps/synonym.h"

namespace spa {
ResultTable::ResultTable(bool found_result)
        : has_result(found_result), type(kBool) {}
ResultTable::ResultTable(Synonym *synonym, Domain &&domain)
        : has_result(!domain.empty()),
          first_(std::make_unique<VariableColumn>(synonym, std::move(domain))),
          type(kSingle) {}
ResultTable::ResultTable(Synonym *synonym_1, Column &&column_1,
                         Synonym *synonym_2, Column &&column_2)
        : has_result(!column_1.empty()),
          first_(std::make_unique<VariableColumn>(synonym_1,
                                                  std::move(column_1))),
          type(kDouble) {}
ResultTable::DomainPair ResultTable::Update(const ResultTable &other) {
    assert(type != kDouble);

    // align the first and second column according to other
    const auto &[this_1st, this_2nd] =
            first_->synonym == other.first_->synonym
                    ? std::make_pair(first_.get(), second_.get())
                    : std::make_pair(second_.get(), first_.get());
    assert(other.first_->synonym == this_1st->synonym &&
           other.second_->synonym == this_2nd->synonym);
    Domain common_1st = Intersect(other.first_->domain, this_1st->domain),
           common_2nd = Intersect(other.second_->domain, this_2nd->domain);
    if (common_1st.empty() || common_2nd.empty()) return {{}, {}};

    const auto n_row = 1 + *common_1st.rbegin(),
               n_col = 1 + *common_2nd.rbegin();
    Bitmap bitmap(n_row, std::vector<bool>(n_col, false));
    for (int i = 0; i < this_1st->column.size(); ++i) {
        const auto a = this_1st->column[i], b = this_2nd->column[i];
        if (common_1st.find(a) == common_1st.end() ||
            common_2nd.find(b) == common_2nd.end()) {
            continue;
        }
        bitmap[a][b] = true;
    }

    Domain domain_1st, domain_2nd;
    Column column_1st, column_2nd;
    for (int i = 0; i < other.first_->column.size(); ++i) {
        const auto a = other.first_->column[i];
        const auto b = other.second_->column[i];
        if (common_1st.find(a) == common_1st.end() ||
            common_2nd.find(b) == common_2nd.end() || !bitmap[a][b]) {
            continue;
        }
        domain_1st.emplace(a), domain_2nd.emplace(b);
        column_1st.emplace_back(a), column_2nd.emplace_back(b);
    }
    this_1st->Assign(std::move(domain_1st), std::move(column_1st));
    this_2nd->Assign(std::move(domain_2nd), std::move(column_2nd));
    return {this_1st->domain, this_2nd->domain};
}
ResultTable::DomainPair ResultTable::Update(const Synonym *syn_a,
                                            const Domain &domain_a,
                                            const Synonym *syn_b,
                                            const Domain &domain_b) {
    auto [this_a, this_b] =
            first_->synonym == syn_a
                    ? std::make_pair(first_.get(), second_.get())
                    : std::make_pair(second_.get(), first_.get());
    assert(this_a->synonym == syn_a && this_b->synonym == syn_b);
    auto common_a = Intersect(this_a->domain, domain_a),
         common_b = Intersect(this_b->domain, domain_b);
    if (common_a.empty() || common_b.empty()) {
        return {{}, {}};
    }
    Column column_a, column_b;
    Domain domain_a_new, domain_b_new;
    for (int i = 0; i < this_a->column.size(); ++i) {
        const auto x = this_a->column[i], y = this_b->column[i];
        if (common_a.find(x) == common_a.end() ||
            common_b.find(y) == common_b.end()) {
            continue;
        }
        domain_a_new.emplace(x), domain_b_new.emplace(y);
        column_a.emplace_back(x), column_b.emplace_back(y);
    }
    this_a->domain.swap(domain_a_new), this_b->domain.swap(domain_b_new);
    this_a->column.swap(column_a), this_b->column.swap(column_b);
    return {this_a->domain, this_b->domain};
}
const ResultTable::Domain &ResultTable::Update(const Synonym *syn_a,
                                               const Domain &domain_a) {
    auto [this_a, this_b] =
            first_->synonym == syn_a
                    ? std::make_pair(first_.get(), second_.get())
                    : std::make_pair(second_.get(), first_.get());
    assert(this_a->synonym == syn_a);
    auto common_a = Intersect(this_a->domain, domain_a);
    if (common_a.empty()) {
        return std::move(Domain());
    }
    Column column_a, column_b;
    Domain domain_a_new, domain_b_new;
    for (int i = 0; i < this_a->column.size(); ++i) {
        const auto x = this_a->column[i], y = this_b->column[i];
        if (common_a.find(x) == common_a.end()) continue;
        domain_a_new.emplace(x), domain_b_new.emplace(y);
        column_a.emplace_back(x), column_b.emplace_back(y);
    }
    this_a->domain.swap(domain_a_new), this_b->domain.swap(domain_b_new);
    this_a->column.swap(column_a), this_b->column.swap(column_b);
    return this_b->domain;
}
ResultTable::Domain ResultTable::Intersect(const Domain &first,
                                           const Domain &second) {
    Domain intersection;
    std::set_intersection(first.begin(), first.end(), second.begin(),
                          second.end(),
                          std::inserter(intersection, intersection.begin()));
    return intersection;
}
const VariableColumn *ResultTable::GetFirstColumn() const {
    assert(type != kBool);
    return first_.get();
}
const VariableColumn *ResultTable::GetSecondColumn() const {
    assert(type == kDouble);
    return second_.get();
}
ResultTable::SynonymPair ResultTable::GetSynonyms() const {
    assert(type == kDouble);
    return {first_->synonym, second_->synonym};
}
}  // namespace spa
