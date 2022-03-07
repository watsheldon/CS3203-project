#include "type_statements_store.h"

#include <cassert>

namespace spa {
TypeStatementsStore::TypeStatementsStore(std::vector<int>&& reads,
                                         std::vector<int>&& prints,
                                         std::vector<int>&& calls,
                                         std::vector<int>&& whiles,
                                         std::vector<int>&& ifs,
                                         std::vector<int>&& assigns)
        : statement_to_type_(reads.size() + prints.size() + calls.size() +
                             whiles.size() + ifs.size() + assigns.size() + 1),
          type_to_statements_{{std::move(reads), std::move(prints),
                               std::move(calls), std::move(whiles),
                               std::move(ifs), std::move(assigns)}} {
    for (int i = 1; i <= type_to_statements_.size(); ++i) {
        auto type = static_cast<StmtType>(i);
        for (auto j : type_to_statements_[i - 1]) {
            statement_to_type_[j] = type;
        }
    }
}

const std::vector<int>& TypeStatementsStore::GetStatements(StmtType st) const {
    assert(st != StmtType::kAll);
    return type_to_statements_[static_cast<int>(st) - 1];
}

StmtType TypeStatementsStore::GetType(int stmt_no) const {
    return statement_to_type_[stmt_no];
}
}  // namespace spa
