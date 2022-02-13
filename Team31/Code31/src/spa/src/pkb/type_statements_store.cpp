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
    for (auto& i : type_to_statements_[0]) {
        statement_to_type_[i] = StmtType::kRead;
    }
    for (auto& i : type_to_statements_[1]) {
        statement_to_type_[i] = StmtType::kPrint;
    }
    for (auto& i : type_to_statements_[2]) {
        statement_to_type_[i] = StmtType::kCall;
    }
    for (auto& i : type_to_statements_[3]) {
        statement_to_type_[i] = StmtType::kWhile;
    }
    for (auto& i : type_to_statements_[4]) {
        statement_to_type_[i] = StmtType::kIf;
    }
    for (auto& i : type_to_statements_[5]) {
        statement_to_type_[i] = StmtType::kAssign;
    }
}
std::vector<int> TypeStatementsStore::GetStatements(StmtType st) {
    std::vector<int> statements(type_to_statements_[static_cast<int>(st)]);
    return statements;
}

StmtType TypeStatementsStore::GetType(int stmt_no) {
    return statement_to_type_[stmt_no];
}
}  // namespace spa
