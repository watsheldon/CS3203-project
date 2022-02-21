#include "type_statements_store.h"

#include <cassert>
#include <vector>

namespace spa {
TypeStatementsStore::TypeStatementsStore(
        size_t stmt_count, std::vector<int>&& reads, std::vector<int>&& prints,
        std::vector<int>&& calls, std::vector<int>&& whiles,
        std::vector<int>&& ifs, std::vector<int>&& assigns)
        : statement_to_type_(stmt_count + 1), type_to_statements_(7) {
    type_to_statements_[1] = reads;
    type_to_statements_[2] = prints;
    type_to_statements_[3] = calls;
    type_to_statements_[4] = whiles;
    type_to_statements_[5] = ifs;
    type_to_statements_[6] = assigns;
    for (int i = 1; i < 7; ++i) {
        auto type = static_cast<StmtType>(i);
        for (auto j : type_to_statements_[i]) {
            statement_to_type_[j] = type;
        }
    }
}

const std::vector<int>& TypeStatementsStore::GetStatements(StmtType st) const {
    assert(st != StmtType::kAll);
    return type_to_statements_[static_cast<int>(st)];
}

StmtType TypeStatementsStore::GetType(int stmt_no) const {
    return statement_to_type_[stmt_no];
}
}  // namespace spa
