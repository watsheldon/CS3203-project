#include "type_statements_store.h"

namespace spa {
TypeStatementsStore::TypeStatementsStore(size_t size, std::vector<int>&& reads,
                                         std::vector<int>&& prints,
                                         std::vector<int>&& calls,
                                         std::vector<int>&& whiles,
                                         std::vector<int>&& ifs,
                                         std::vector<int>&& assigns)
        : statement_to_type_(size), type_to_statements_(6) {
    type_to_statements_[0] = reads;
    type_to_statements_[1] = prints;
    type_to_statements_[2] = calls;
    type_to_statements_[3] = whiles;
    type_to_statements_[4] = ifs;
    type_to_statements_[5] = assigns;
    for (int i = 0; i < 6; ++i) {
        StmtType type = static_cast<StmtType>(i);
        for (auto j : type_to_statements_[i]) {
            statement_to_type_[j] = type;
        }
    }
}
std::vector<int> TypeStatementsStore::GetStatements(StmtType st) {
    return type_to_statements_[static_cast<int>(st)];
}

StmtType TypeStatementsStore::GetType(int stmt_no) {
    return statement_to_type_[stmt_no];
}
}  // namespace spa
