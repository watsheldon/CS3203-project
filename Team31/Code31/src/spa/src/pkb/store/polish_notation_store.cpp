#include "polish_notation_store.h"
namespace spa {
PolishNotationStore::PolishNotationStore(size_t stmt_count,
                                         std::vector<PN>&& pn)
        : stmt_to_index_(stmt_count + 1),
          index_to_stmt_(pn.size()),
          index_to_pn_(std::move(pn)) {}

void PolishNotationStore::Set(int stmt_no, int polish_index) {
    stmt_to_index_[stmt_no] = polish_index;
    index_to_stmt_[polish_index] = stmt_no;
}

const PN& PolishNotationStore::GetNotation(int polish_index) const {
    return index_to_pn_[polish_index];
}

int PolishNotationStore::GetPolishStmt(int polish_index) const {
    return index_to_stmt_[polish_index];
}

int PolishNotationStore::GetPolishIndex(int stmt_no) const {
    return stmt_to_index_[stmt_no];
}

}  // namespace spa
