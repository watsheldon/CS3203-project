#include "program_knowledge_base.h"

namespace spa {
ProgramKnowledgeBase::ProgramKnowledgeBase(
        const std::shared_ptr<BasicEntities> &init)
        : stmt_size_(init->assigns.size() + init->ifs.size() +
                     init->whiles.size() + init->calls.size() +
                     init->reads.size() + init->prints.size() -
                     STMT_TYPE_COUNT),
          stmtlst_size_(init->procedures.size() + init->whiles.size() +
                        init->ifs.size() * 2 - 4),
          proc_stmtlst_(ProcedureStmtlstStore(init->procedures.size(),
                                              stmtlst_size_ + 1)),
          while_stmtlst_(WhileStmtlstStore(stmt_size_ + 1, stmtlst_size_ + 1)),
          if_stmtlst_(IfStmtlstStore(stmt_size_ + 1, stmtlst_size_ + 1)),
          stmtlst_stmt_(
                  StmtlstStatementsStore(stmtlst_size_ + 1, stmt_size_ + 1)),
          containers_(init->whiles.size() + init->ifs.size() - 1),
          proc_name_(ProcedureNameStore(std::move(init->procedures))),
          var_name_(VariableNameStore(std::move(init->variables))),
          const_value_(ConstantValueStore(std::move(init->constants))),
          // polish_notation_(PolishNotationStore(init->notations)),
          type_stmt_(stmt_size_, std::move(init->reads),
                     std::move(init->prints), std::move(init->calls),
                     std::move(init->whiles), std::move(init->ifs),
                     std::move(init->assigns)) {}

void ProgramKnowledgeBase::SetIndex(Index<kProc> proc_index,
                                    Index<kStmtLst> stmtlst_index) {
    assert(!compiled);
    proc_stmtlst_.Set(proc_index, stmtlst_index);
}
void ProgramKnowledgeBase::SetIndex(Index<kStmt> while_stmt,
                                    Index<kStmtLst> stmtlst_index) {
    assert(!compiled);
    while_stmtlst_.Set(while_stmt, stmtlst_index);
}
void ProgramKnowledgeBase::SetIndex(Index<kStmt> if_stmt,
                                    Index<kStmtLst> then_index,
                                    Index<kStmtLst> else_index) {
    assert(!compiled);
    if_stmtlst_.Set(if_stmt, then_index, else_index);
}

void ProgramKnowledgeBase::SetLst(Index<kStmtLst> stmtlst_index,
                                  std::vector<Index<kStmt>> stmtlst) {
    assert(!compiled);
    stmtlst_stmt_.Set(stmtlst_index, std::move(stmtlst));
}

void ProgramKnowledgeBase::SetRel(Index<kStmt> stmt_no, Index<kVar> var_index) {
    assert(!compiled);
}

void ProgramKnowledgeBase::Compile() {
    assert(!compiled);

    compiled = true;
}

std::vector<std::string> ProgramKnowledgeBase::GetAllStringEntities(
        PKBEntityType et) {
    assert(compiled);
    switch (et) {
        case kProc:
            return proc_name_.GetAllName();
        case kVar:
            return var_name_.GetAllName();
        case kConst:
            return const_value_.GetAllValue();
        default:
            std::vector<std::string> results;
            return results;
    }
}

std::vector<Index<kStmt>> ProgramKnowledgeBase::GetAllStmtEntities(
        PKBEntityType et) {
    assert(compiled);
    return type_stmt_.GetStatements(et);
}

std::vector<std::string> ProgramKnowledgeBase::GetProcName(
        const std::vector<Index<kProc>> &index_list) {
    assert(compiled);
    std::vector<std::string> results;
    results.reserve(index_list.size());
    std::transform(index_list.begin(), index_list.end(),
                   std::back_inserter(results),
                   [this](Index<kProc> i) { return proc_name_.GetName(i); });
    return results;
}

std::vector<std::string> ProgramKnowledgeBase::GetVarName(
        const std::vector<Index<kVar>> &index_list) {
    assert(compiled);
    std::vector<std::string> results;
    results.reserve(index_list.size());
    std::transform(index_list.begin(), index_list.end(),
                   std::back_inserter(results),
                   [this](Index<kVar> i) { return var_name_.GetName(i); });
    return results;
}
std::vector<std::string> ProgramKnowledgeBase::GetConstValue(
        const std::vector<Index<kConst>> &index_list) {
    assert(compiled);
    std::vector<std::string> results;
    results.reserve(index_list.size());
    std::transform(
            index_list.begin(), index_list.end(), std::back_inserter(results),
            [this](Index<kConst> i) { return const_value_.GetValue(i); });
    return results;
}

template <PKBEntityType T>
std::vector<std::string> ProgramKnowledgeBase::IndexToName(
        const std::vector<Index<T>> &index_list) {
    assert(compiled);
    switch (T) {
        case kProc:
            return ProgramKnowledgeBase::GetProcName(index_list);
        case kVar:
            return ProgramKnowledgeBase::GetVarName(index_list);
        case kConst:
            return ProgramKnowledgeBase::GetConstValue(index_list);
        default:
            std::vector<std::string> results;
            results.reserve(index_list.size());
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(results),
                           [](int i) { return std::to_string(i); });
            return results;
    }
}

}  // namespace spa
