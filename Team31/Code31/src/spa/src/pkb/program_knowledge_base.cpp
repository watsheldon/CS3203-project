#include "program_knowledge_base.h"

namespace spa {
ProgramKnowledgeBase::ProgramKnowledgeBase(std::shared_ptr<BasicEntities> init)
        : entities_ptr_(init),
          proc_name_(ProcedureNameStore(init->procedures)),
          var_name_(VariableNameStore(init->variables)),
          // polish_notation_(PolishNotationStore(init->notations)),
          proc_stmtlst_(ProcedureStmtlstStore(init->procedures.size())),
          while_stmtlst_(WhileStmtlstStore()),
          if_stmtlst_(IfStmtlstStore()),
          stmtlsts_(init->procedures.size() + init->whiles.size() +
                    init->ifs.size() * 2 - 3),
          containers_(init->whiles.size() + init->ifs.size() - 1),
          map_no_type_(init->reads.size() + init->prints.size() +
                       init->calls.size() + init->whiles.size() +
                       init->ifs.size() + init->assigns.size() -
                       STMT_TYPE_COUNT + 1),
          map_no_lst_(init->reads.size() + init->prints.size() +
                      init->calls.size() + init->whiles.size() +
                      init->ifs.size() + init->assigns.size() -
                      STMT_TYPE_COUNT + 1),
          stmt_size_(init->assigns.size() + init->ifs.size() +
                     init->whiles.size() + init->calls.size() +
                     init->reads.size() + init->prints.size() -
                     STMT_TYPE_COUNT) {
    for (auto& i : init->reads) {
        map_no_type_[i] = kReadStmt;
    }
    for (auto& i : init->prints) {
        map_no_type_[i] = kPrintStmt;
    }
    for (auto& i : init->calls) {
        map_no_type_[i] = kCallStmt;
    }
    for (auto& i : init->whiles) {
        map_no_type_[i] = kWhileStmt;
    }
    for (auto& i : init->ifs) {
        map_no_type_[i] = kIfStmt;
    }
    for (auto& i : init->assigns) {
        map_no_type_[i] = kAssignStmt;
    }
}

void ProgramKnowledgeBase::SetIndex(Index<kProc> proc_index,
                                    Index<kStmtLst> stmtlst_index) {
    proc_stmtlst_.Set(proc_index.value, stmtlst_index.value);
}
void ProgramKnowledgeBase::SetIndex(Index<kWhileStmt> stmt_no,
                                    Index<kStmtLst> stmtlst_index) {
    while_stmtlst_.Set(stmt_no.value, stmtlst_index.value);
}
void ProgramKnowledgeBase::SetIndex(Index<kIfStmt> stmt_no,
                                    Index<kStmtLst> then_index,
                                    Index<kStmtLst> else_index) {
    if_stmtlst_.Set(stmt_no.value, then_index.value, else_index.value);
}

void ProgramKnowledgeBase::SetLst(Index<kStmtLst> stmtlst_index,
                                  std::vector<STMT_NO> stmtlst) {
    stmtlsts_.at(stmtlst_index.value) = stmtlst;
}

void ProgramKnowledgeBase::SetRel(Index<kPrintStmt> stmt_no,
                                  Index<kVar> var_index) {}

void ProgramKnowledgeBase::Compile() {}

std::vector<std::string> ProgramKnowledgeBase::GetAllStringEntities(
        PKBEntityType et) {
    std::vector<std::string> results;
    switch (et) {
        case kProc:
            results.reserve((entities_ptr_->procedures.size()) - 1);
            std::copy(entities_ptr_->procedures.begin() + 1,
                      entities_ptr_->procedures.end(),
                      std::back_inserter(results));
            break;
        case kVar:
            results.reserve((entities_ptr_->variables.size()) - 1);
            std::copy(entities_ptr_->variables.begin() + 1,
                      entities_ptr_->variables.end(),
                      std::back_inserter(results));
            break;
        case kConst:
            results.reserve((entities_ptr_->constants.size()) - 1);
            std::copy(entities_ptr_->constants.begin() + 1,
                      entities_ptr_->constants.end(),
                      std::back_inserter(results));
            break;
        default:
            break;
    }

    return results;
}

std::vector<int> ProgramKnowledgeBase::GetAllStmtEntities(PKBEntityType et) {
    std::vector<int> results;
    switch (et) {
        case kReadStmt:
            results.reserve((entities_ptr_->reads.size()) - 1);
            std::copy(entities_ptr_->reads.begin() + 1,
                      entities_ptr_->reads.end(), std::back_inserter(results));
            break;
        case kPrintStmt:
            results.reserve((entities_ptr_->prints.size()) - 1);
            std::copy(entities_ptr_->prints.begin() + 1,
                      entities_ptr_->prints.end(), std::back_inserter(results));
            break;
        case kCallStmt:
            results.reserve((entities_ptr_->calls.size()) - 1);
            std::copy(entities_ptr_->calls.begin() + 1,
                      entities_ptr_->calls.end(), std::back_inserter(results));
            break;
        case kWhileStmt:
            results.reserve((entities_ptr_->whiles.size()) - 1);
            std::copy(entities_ptr_->whiles.begin() + 1,
                      entities_ptr_->whiles.end(), std::back_inserter(results));
            break;
        case kIfStmt:
            results.reserve((entities_ptr_->ifs.size()) - 1);
            std::copy(entities_ptr_->ifs.begin() + 1, entities_ptr_->ifs.end(),
                      std::back_inserter(results));
            break;
        case kAssignStmt:
            results.reserve((entities_ptr_->assigns.size()) - 1);
            std::copy(entities_ptr_->assigns.begin() + 1,
                      entities_ptr_->assigns.end(),
                      std::back_inserter(results));
            break;
        case kStmt:
            results.reserve(stmt_size_);
            for (int i = 1; i <= stmt_size_; i++) {
                results.emplace_back(i);  // initialise vector for stmt
            }
            break;
        default:
            break;
    }

    return results;
}

std::vector<std::string> ProgramKnowledgeBase::IndexToName(
        std::vector<int> index_list, PKBEntityType et) {
    std::vector<std::string> results;
    results.reserve(index_list.size());
    switch (et) {
        case kProc:
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(results), [this](int i) {
                               return entities_ptr_->procedures[i];
                           });
            break;
        case kVar:
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(results), [this](int i) {
                               return entities_ptr_->variables[i];
                           });
            break;
        case kConst:
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(results), [this](int i) {
                               return entities_ptr_->constants[i];
                           });
            break;
        default:
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(results),
                           [](int i) { return std::to_string(i); });
            break;
    }
    return results;
}
}  // namespace spa
