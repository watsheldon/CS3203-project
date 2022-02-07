#include "program_knowledge_base.h"
using namespace spa;

ProgramKnowledgeBase::ProgramKnowledgeBase(std::shared_ptr<Init> init)
        : entities_ptr_(init),
          proc_stmtlst_(init->procedures.size()),
          while_stmtlst_(init->whiles.size()),
          if_stmtlst_(init->ifs.size() * 2),
          stmtlsts_(init->procedures.size() + init->whiles.size() + init->ifs.size() * 2 - 3),
          containers_(init->whiles.size() + init->ifs.size() - 1),
          map_no_index_(init->reads.size() + init->prints.size() + init->calls.size()
          + init->whiles.size()+ init->ifs.size() + init->assigns.size() - 5),
          map_no_type_(init->reads.size() + init->prints.size() + init->calls.size()
          + init->whiles.size()+ init->ifs.size() + init->assigns.size() - 5),
          map_no_lst_(init->reads.size() + init->prints.size() + init->calls.size()
          + init->whiles.size()+ init->ifs.size() + init->assigns.size() - 5),
          stmt_size_(init->assigns.size() + init->ifs.size() + init->whiles.size() 
          + init->calls.size() + init->reads.size() + init->prints.size() - REMOVE_INDEX_0_FOR_STMT_ENTITY) {

    //fill up vectors
    //map_no_index_
    //map_no_type_
    for ( int i = 1; i < init->reads.size(); ++i ) {
        int stmt_no = init->reads.at(i);
        map_no_index_.at(stmt_no) = i;
        map_no_type_.at(stmt_no) = kRead;
    }
    for ( int i = 1; i < init->prints.size(); ++i ) {
        int stmt_no = init->prints.at(i);
        map_no_index_.at(stmt_no) = i;
        map_no_type_.at(stmt_no) = kPrint;
    }
    for ( int i = 1; i < init->calls.size(); ++i ) {
        int stmt_no = init->calls.at(i);
        map_no_index_.at(stmt_no) = i;
        map_no_type_.at(stmt_no) = kCall;
    }
    for ( int i = 1; i < init->whiles.size(); ++i ) {
        int stmt_no = init->whiles.at(i);
        map_no_index_.at(stmt_no) = i;
        map_no_type_.at(stmt_no) = kWhile;
    }
    for ( int i = 1; i <init->ifs.size(); ++i ) {
        int stmt_no = init->ifs.at(i);
        map_no_index_.at(stmt_no) = i;
        map_no_type_.at(stmt_no) = kIf;
    }
    for ( int i = 1; i < init->assigns.size(); ++i ) {
        int stmt_no = init->assigns.at(i);
        map_no_index_.at(stmt_no) = i;
        map_no_type_.at(stmt_no) = kAssign;
    }
}

void ProgramKnowledgeBase::set_index(Index<kProc> proc_index, Index<kStmtLst> stmtlst_index) {
    proc_stmtlst_.at(proc_index.value) = stmtlst_index.value;
}
void ProgramKnowledgeBase::set_index(Index<kWhile> stmt_no, Index<kStmtLst> stmtlst_index) {
    int index = map_no_index_.at(stmt_no.value);
    while_stmtlst_.at(index) = stmtlst_index.value;
}
void ProgramKnowledgeBase::set_index(Index<kIf> stmt_no, Index<kStmtLst> stmtlst_index1, Index<kStmtLst> stmtlst_index2) {
    int index = map_no_index_.at(stmt_no.value);
    if_stmtlst_.at(index * 2) = stmtlst_index1.value;
    if_stmtlst_.at(index * 2 + 1) = stmtlst_index2.value;
}

void ProgramKnowledgeBase::set_lst(Index<kStmtLst> stmtlst_index, std::vector<STMT_NO> stmtlst) {
    stmtlsts_.at(stmtlst_index.value) = stmtlst;
}

void ProgramKnowledgeBase::set_rel(Index<kPrint> stmt_no, Index<kVar> var_index) {

}

void ProgramKnowledgeBase::set_end() {
    //container vector <node> while +if  --> container index to construct
    //curr parent whiles_[i] {stmtlst}
    //whiles_ -->  -->
    for (int i = 1; i < stmtlsts_.size(); ++i) {
        //assume each stmtlst starts from index 1 as well
        for (int j = 1; j < stmtlsts_.at(i).size(); ++j) {
        }
    }

}

void ProgramKnowledgeBase::set() {

}

std::vector<std::string> ProgramKnowledgeBase::get_all_string_entities(EntityType et) {
    std::vector<std::string> results;
    switch(et) {
    case kProc:
        results.resize((entities_ptr_->procedures.size()) - 1);
        std::copy(entities_ptr_->procedures.begin() + 1, entities_ptr_->procedures.end(), results.begin());
        break;
    case kVar:
        results.resize((entities_ptr_->variables.size()) - 1);
        std::copy(entities_ptr_->variables.begin() + 1, entities_ptr_->variables.end(), results.begin());
        break;
    case kConst:
        results.resize((entities_ptr_->constants.size()) - 1);
        std::copy(entities_ptr_->constants.begin() + 1, entities_ptr_->constants.end(), results.begin());
        break;
    default:
        break;
    }

    return results;
}

std::vector<int> ProgramKnowledgeBase::get_all_stmt_entities(EntityType et) {
    std::vector<int> results;
    switch (et) {
    case kRead:
        results.resize((entities_ptr_->reads.size()) - 1);
        std::copy(entities_ptr_->reads.begin() + 1, entities_ptr_->reads.end(), results.begin());
        break;
    case kPrint:
        results.resize((entities_ptr_->prints.size()) - 1);
        std::copy(entities_ptr_->prints.begin() + 1, entities_ptr_->prints.end(), results.begin());
        break;
    case kCall:
        results.resize((entities_ptr_->calls.size()) - 1);
        std::copy(entities_ptr_->calls.begin() + 1, entities_ptr_->calls.end(), results.begin());
        break;
    case kWhile:
        results.resize((entities_ptr_->whiles.size()) - 1);
        std::copy(entities_ptr_->whiles.begin() + 1, entities_ptr_->whiles.end(), results.begin());
        break;
    case kIf:
        results.resize((entities_ptr_->ifs.size()) - 1);
        std::copy(entities_ptr_->ifs.begin() + 1, entities_ptr_->ifs.end(), results.begin());
        break;
    case kAssign:
        results.resize((entities_ptr_->assigns.size()) - 1);
        std::copy(entities_ptr_->assigns.begin() + 1, entities_ptr_->assigns.end(), results.begin());
        break;
    case kStmt:
        results.reserve(stmt_size_);
        for (int i = 1; i <= stmt_size_; i++) {
            results.emplace_back(i); // initialise vector for stmt
        }
        break;
    default:
        break;
    }

    return results;
}

std::vector<std::string> ProgramKnowledgeBase::to_name(std::vector<int> index_list, EntityType et) {
    std::vector<std::string> results;
    results.reserve(index_list.size());
    switch (et) {
    case kProc:
        results.reserve(index_list.size());
        std::transform(index_list.begin(), index_list.end(), std::back_inserter(results), [this](int i) { return entities_ptr_->procedures[i]; });
        break;
    case kVar:
        results.reserve(index_list.size());
        std::transform(index_list.begin(), index_list.end(), std::back_inserter(results), [this](int i) { return entities_ptr_->variables[i]; });
        break;
    case kConst:
        std::transform(index_list.begin(), index_list.end(), std::back_inserter(results), [this](int i) { return entities_ptr_->constants[i]; });
        break;
    default:
        for (int i = 0; i < index_list.size(); i++) {
            results.emplace_back(std::to_string(index_list[i])); // convert vector<int> to vector<str>
        }
        break;
    }
    return results;
}
