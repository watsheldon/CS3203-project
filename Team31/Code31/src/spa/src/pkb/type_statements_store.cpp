#include "type_statements_store.h"

namespace spa {
TypeStatementsStore::TypeStatementsStore(size_t size,
                                         std::vector<Index<kStmt>>&& reads,
                                         std::vector<Index<kStmt>>&& prints,
                                         std::vector<Index<kStmt>>&& calls,
                                         std::vector<Index<kStmt>>&& whiles,
                                         std::vector<Index<kStmt>>&& ifs,
                                         std::vector<Index<kStmt>>&& assigns)
        : statement_to_type(size),
          type_to_statements({reads, prints, calls, whiles, ifs, assigns}) {
    for (auto& i : type_to_statements.reads) {
        statement_to_type[i.value] = kReadStmt;
    }
    for (auto& i : type_to_statements.prints) {
        statement_to_type[i.value] = kPrintStmt;
    }
    for (auto& i : type_to_statements.calls) {
        statement_to_type[i.value] = kCallStmt;
    }
    for (auto& i : type_to_statements.whiles) {
        statement_to_type[i.value] = kWhileStmt;
    }
    for (auto& i : type_to_statements.ifs) {
        statement_to_type[i.value] = kIfStmt;
    }
    for (auto& i : type_to_statements.assigns) {
        statement_to_type[i.value] = kAssignStmt;
    }
}
std::vector<Index<kStmt>> TypeStatementsStore::GetStatements(PKBEntityType et) {
    std::vector<Index<kStmt>> statements;
    switch (et) {
        case kReadStmt:
            statements.reserve((type_to_statements.reads.size()));
            std::copy(type_to_statements.reads.begin(),
                      type_to_statements.reads.end(),
                      std::back_inserter(statements));
            break;
        case kPrintStmt:
            statements.reserve((type_to_statements.prints.size()));
            std::copy(type_to_statements.prints.begin(),
                      type_to_statements.prints.end(),
                      std::back_inserter(statements));
            break;
        case kCallStmt:
            statements.reserve((type_to_statements.calls.size()));
            std::copy(type_to_statements.calls.begin(),
                      type_to_statements.calls.end(),
                      std::back_inserter(statements));
            break;
        case kWhileStmt:
            statements.reserve((type_to_statements.whiles.size()));
            std::copy(type_to_statements.whiles.begin(),
                      type_to_statements.whiles.end(),
                      std::back_inserter(statements));
            break;
        case kIfStmt:
            statements.reserve((type_to_statements.ifs.size()));
            std::copy(type_to_statements.ifs.begin(),
                      type_to_statements.ifs.end(),
                      std::back_inserter(statements));
            break;
        case kAssignStmt:
            statements.reserve((type_to_statements.assigns.size()));
            std::copy(type_to_statements.assigns.begin(),
                      type_to_statements.assigns.end(),
                      std::back_inserter(statements));
            break;
        case kStmt:
            statements.reserve(statement_to_type.size() - 1);
            for (int i = 1; i < statement_to_type.size(); i++) {
                statements.emplace_back(i);
            }
            break;
        default:
            break;
    }
    return statements;
}

PKBEntityType TypeStatementsStore::GetType(Index<kStmt> stmt_no) {
    return statement_to_type[stmt_no.value];
}
}  // namespace spa