#include "program_knowledge_base.h"
using namespace spa;

ProgramKnowledgeBase::ProgramKnowledgeBase(const Init& init) {
    procedures_ = init.proc_name;
    variables_ = init.var_name;
    constants_ = init.const_value;
    reads_ = init.read_ref;
    prints_ = init.print_ref;
    calls_ = init.call_ref;
    whiles_ = init.while_ref;
    ifs_ = init.if_ref;
    assigns_ = init.assign_ref;
    notations_ = init.assign_expr;
    //set the size of vectors accordingly
    proc_stmtlst_.resize(init.proc_name.size());
    while_stmtlst_.resize(init.while_ref.size());
    if_stmtlst_.resize(init.if_ref.size() * 2);
    stmtlsts_.resize(init.proc_name.size() + init.while_ref.size() + init.if_ref.size() * 2 - 3);
    containers_.resize(init.while_ref.size() + init.if_ref.size() - 1);
    map_no_index_.resize(reads_.size() + prints_.size() + calls_.size() + whiles_.size()
                         + ifs_.size() + assigns_.size() - 5);
    map_no_type_.resize(reads_.size() + prints_.size() + calls_.size() + whiles_.size()
                        + ifs_.size() + assigns_.size() - 5);
    map_no_lst_.resize(reads_.size() + prints_.size() + calls_.size() + whiles_.size()
                                + ifs_.size() + assigns_.size() - 5);

    //fill up vectors
    //map_no_index_
    //map_no_type_
    for ( int i = 1; i < reads_.size(); ++i ) {
        int stmt_no = reads_.at(i);
        map_no_index_.at(stmt_no) = i;
        map_no_type_.at(stmt_no) = kRead;
    }
    for ( int i = 1; i < prints_.size(); ++i ) {
        int stmt_no = prints_.at(i);
        map_no_index_.at(stmt_no) = i;
        map_no_type_.at(stmt_no) = kPrint;
    }
    for ( int i = 1; i < calls_.size(); ++i ) {
        int stmt_no = calls_.at(i);
        map_no_index_.at(stmt_no) = i;
        map_no_type_.at(stmt_no) = kCall;
    }
    for ( int i = 1; i < whiles_.size(); ++i ) {
        int stmt_no = whiles_.at(i);
        map_no_index_.at(stmt_no) = i;
        map_no_type_.at(stmt_no) = kWhile;
    }
    for ( int i = 1; i < ifs_.size(); ++i ) {
        int stmt_no = ifs_.at(i);
        map_no_index_.at(stmt_no) = i;
        map_no_type_.at(stmt_no) = kIf;
    }
    for ( int i = 1; i < assigns_.size(); ++i ) {
        int stmt_no = assigns_.at(i);
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

}
