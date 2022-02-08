#include "program_knowledge_base.h"

namespace spa {
ProgramKnowledgeBase::ProgramKnowledgeBase(std::shared_ptr<BasicEntities> init)
        : entities_ptr_(init),
          proc_stmtlst_(init->procedures.size()),
          while_stmtlst_(init->whiles.size()),
          if_stmtlst_(init->ifs.size() * 2),
          stmtlsts_(init->procedures.size() + init->whiles.size()
                            + init->ifs.size() * 2 - 3),
          containers_(init->whiles.size() + init->ifs.size() - 1),
          map_no_index_(init->reads.size() + init->prints.size()
                                + init->calls.size() + init->whiles.size()
                                + init->ifs.size() + init->assigns.size() - 5),
          map_no_type_(init->reads.size() + init->prints.size()
                               + init->calls.size()
                               + init->whiles.size() + init->ifs.size()
                               + init->assigns.size() - 5),
          map_no_lst_(init->reads.size() + init->prints.size()
                              + init->calls.size() + init->whiles.size()
                              + init->ifs.size() + init->assigns.size() - 5),
          stmt_size_(init->assigns.size() + init->ifs.size()
                             + init->whiles.size() + init->calls.size()
                             + init->reads.size() + init->prints.size()
                             - STMT_TYPE_COUNT) {
    //fill up vectors
    //map_no_index_
    //map_no_type_
    for (int i = 1; i < init->reads.size(); ++i) {
        int stmt_no = init->reads.at(i);
        map_no_index_.at(stmt_no) = i;
        map_no_type_.at(stmt_no) = kReadStmt;
    }
    for (int i = 1; i < init->prints.size(); ++i) {
        int stmt_no = init->prints.at(i);
        map_no_index_.at(stmt_no) = i;
        map_no_type_.at(stmt_no) = kPrintStmt;
    }
    for (int i = 1; i < init->calls.size(); ++i) {
        int stmt_no = init->calls.at(i);
        map_no_index_.at(stmt_no) = i;
        map_no_type_.at(stmt_no) = kCallStmt;
    }
    for (int i = 1; i < init->whiles.size(); ++i) {
        int stmt_no = init->whiles.at(i);
        map_no_index_.at(stmt_no) = i;
        map_no_type_.at(stmt_no) = kWhileStmt;
    }
    for (int i = 1; i < init->ifs.size(); ++i) {
        int stmt_no = init->ifs.at(i);
        map_no_index_.at(stmt_no) = i;
        map_no_type_.at(stmt_no) = kIfStmt;
    }
    for (int i = 1; i < init->assigns.size(); ++i) {
        int stmt_no = init->assigns.at(i);
        map_no_index_.at(stmt_no) = i;
        map_no_type_.at(stmt_no) = kAssignStmt;
    }
}

void ProgramKnowledgeBase::SetIndex(Index<kProc> proc_index,
                                    Index<kStmtLst> stmtlst_index) {
    proc_stmtlst_.at(proc_index.value) = stmtlst_index.value;
}
void ProgramKnowledgeBase::SetIndex(Index<kWhileStmt> stmt_no,
                                    Index<kStmtLst> stmtlst_index) {
    int index = map_no_index_.at(stmt_no.value);
    while_stmtlst_.at(index) = stmtlst_index.value;
}
void ProgramKnowledgeBase::SetIndex(Index<kIfStmt> stmt_no,
                                    Index<kStmtLst> stmtlst_index1,
                                    Index<kStmtLst> stmtlst_index2) {
    int index = map_no_index_.at(stmt_no.value);
    if_stmtlst_.at(index * 2) = stmtlst_index1.value;
    if_stmtlst_.at(index * 2 + 1) = stmtlst_index2.value;
}

void ProgramKnowledgeBase::SetLst(Index<kStmtLst> stmtlst_index,
                                  std::vector<STMT_NO> stmtlst) {
    stmtlsts_.at(stmtlst_index.value) = stmtlst;
}

void ProgramKnowledgeBase::SetRel(Index<kPrintStmt> stmt_no,
                                  Index<kVar> var_index) {

}

void ProgramKnowledgeBase::Compile() {
    //container vector <node> while +if  --> container index to construct
    //curr parent whiles_[i] {stmtlst}
    //whiles_ -->  -->
    for (int i = 1; i < stmtlsts_.size(); ++i) {
        //assume each stmtlst starts from index 1 as well
        for (int j = 1; j < stmtlsts_.at(i).size(); ++j) {
        }
    }

}

std::vector<std::string> ProgramKnowledgeBase::GetAllStringEntities(
        PKBEntityType et) {
    std::vector<std::string> results;
    switch (et) {
        case kProc:results.reserve((entities_ptr_->procedures.size()) - 1);
            std::copy(entities_ptr_->procedures.begin() + 1,
                      entities_ptr_->procedures.end(),
                      std::back_inserter(results));
            break;
        case kVar:results.reserve((entities_ptr_->variables.size()) - 1);
            std::copy(entities_ptr_->variables.begin() + 1,
                      entities_ptr_->variables.end(),
                      std::back_inserter(results));
            break;
        case kConst:results.reserve((entities_ptr_->constants.size()) - 1);
            std::copy(entities_ptr_->constants.begin() + 1,
                      entities_ptr_->constants.end(),
                      std::back_inserter(results));
            break;
        default:break;
    }

    return results;
}

std::vector<int> ProgramKnowledgeBase::GetAllStmtEntities(PKBEntityType et) {
    std::vector<int> results;
    switch (et) {
        case kReadStmt:results.reserve((entities_ptr_->reads.size()) - 1);
            std::copy(entities_ptr_->reads.begin() + 1,
                      entities_ptr_->reads.end(),
                      std::back_inserter(results));
            break;
        case kPrintStmt:results.reserve((entities_ptr_->prints.size()) - 1);
            std::copy(entities_ptr_->prints.begin() + 1,
                      entities_ptr_->prints.end(),
                      std::back_inserter(results));
            break;
        case kCallStmt:results.reserve((entities_ptr_->calls.size()) - 1);
            std::copy(entities_ptr_->calls.begin() + 1,
                      entities_ptr_->calls.end(),
                      std::back_inserter(results));
            break;
        case kWhileStmt:results.reserve((entities_ptr_->whiles.size()) - 1);
            std::copy(entities_ptr_->whiles.begin() + 1,
                      entities_ptr_->whiles.end(),
                      std::back_inserter(results));
            break;
        case kIfStmt:results.reserve((entities_ptr_->ifs.size()) - 1);
            std::copy(entities_ptr_->ifs.begin() + 1,
                      entities_ptr_->ifs.end(),
                      std::back_inserter(results));
            break;
        case kAssignStmt:results.reserve((entities_ptr_->assigns.size()) - 1);
            std::copy(entities_ptr_->assigns.begin() + 1,
                      entities_ptr_->assigns.end(),
                      std::back_inserter(results));
            break;
        case kStmt:results.reserve(stmt_size_);
            for (int i = 1; i <= stmt_size_; i++) {
                results.emplace_back(i); // initialise vector for stmt
            }
            break;
        default:break;
    }

    return results;
}

std::vector<std::string> ProgramKnowledgeBase::IndexToName(std::vector<int> index_list,
                                                           PKBEntityType et) {
    std::vector<std::string> results;
    results.reserve(index_list.size());
    switch (et) {
        case kProc:
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(results),
                           [this](int i) { return entities_ptr_->procedures[i]; });
            break;
        case kVar:
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(results),
                           [this](int i) { return entities_ptr_->variables[i]; });
            break;
        case kConst:
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(results),
                           [this](int i) { return entities_ptr_->constants[i]; });
            break;
        default:
            std::transform(index_list.begin(), index_list.end(),
                           std::back_inserter(results),
                           [](int i) { return std::to_string(i); });
            break;
    }
    return results;
}
}
