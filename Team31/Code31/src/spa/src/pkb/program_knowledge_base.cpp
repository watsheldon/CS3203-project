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
          + init->whiles.size()+ init->ifs.size() + init->assigns.size() - 5) {

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

    stmt_size_ = init->assigns.size() + init->ifs.size() + init->whiles.size() + init->calls.size() +
        init->reads.size() + init->prints.size() - 6; // minus 6 to remove the index 0 for each entity
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
        results = entities_ptr_->procedures;
        break;
    case kVar:
        results = entities_ptr_->variables;
        break;
    case kConst:
        results = entities_ptr_->constants;
        break;
    default:
        results = { "-1" }; // invalid value
        break;
    }
    
    if(results.size() >= 2) {
        results.erase(results.begin()); // remove index 0
    }

    return results;
}

std::vector<int> ProgramKnowledgeBase::get_all_stmt_entities(EntityType et) {
    std::vector<int> intResults;
    switch (et) {
    case kRead:
        intResults = entities_ptr_->reads;
        break;
    case kPrint:
        intResults = entities_ptr_->prints;
        break;
    case kCall:
        intResults = entities_ptr_->calls;
        break;
    case kWhile:
        intResults = entities_ptr_->whiles;
        break;
    case kIf:
        intResults = entities_ptr_->ifs;
        break;
    case kAssign:
        intResults = entities_ptr_->assigns;
        break;
    case kStmt:
        for (int i = 0; i <= stmt_size_; i++) {
            intResults.push_back(i); // initialise vector for stmt
        }
        break;
    default:
        intResults = { -1 }; // invalid value
        break;
    }

    if (intResults.size() >= 2) {
        intResults.erase(intResults.begin()); // remove index 0
    }

    return intResults;
}

std::vector<std::string> ProgramKnowledgeBase::to_name(std::vector<int> index_list, EntityType et) {
    std::vector<std::string> results;
    switch (et) {
    case kProc:
        for (int i : index_list) {
            results.push_back(entities_ptr_->procedures[i]);
        }
        break;
    case kVar:
        for (int i : index_list) {
            results.push_back(entities_ptr_->variables[i]);
        }
        break;
    case kConst:
        for (int i : index_list) {
            results.push_back(entities_ptr_->constants[i]);
        }
        break;
    default:
        for (int i = 0; i < index_list.size(); i++) {
            results.push_back(std::to_string(index_list[i])); // convert vector<int> to vector<str>
        }
        break;
    }
    return results;
}
