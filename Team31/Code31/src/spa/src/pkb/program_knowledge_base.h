#ifndef SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
#define SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <utility>

#include "knowledge_base.h"
#include "pkb/store/call_procedure_store.h"
#include "pkb/store/modifies_relationship_store.h"
#include "pkb/store/name_value_store.h"
#include "pkb/store/polish_notation_store.h"
#include "pkb/store/stmtlst_statements_store.h"
#include "pkb/store/type_statements_store.h"
#include "pkb/store/uses_relationship_store.h"
#include "secondary_structure/container_forest.h"

namespace spa {

class ProgramKnowledgeBase : public KnowledgeBase {
  public:
    ProgramKnowledgeBase() = delete;

    ProgramKnowledgeBase(ProgramKnowledgeBase &&) = delete;

    ProgramKnowledgeBase(const ProgramKnowledgeBase &) = delete;

    explicit ProgramKnowledgeBase(BasicEntities init);

    // set stmtLst: useful for Parent and Follows relationships
    void SetIndex(Index<SetEntityType::kProc> proc_index,
                  Index<SetEntityType::kStmtLst> stmtlst_index) override;
    void SetIndex(Index<SetEntityType::kStmt> stmt_no,
                  Index<SetEntityType::kStmtLst> stmtlst_index) override;
    void SetIndex(Index<SetEntityType::kStmt> stmt_no,
                  Index<SetEntityType::kStmtLst> then_index,
                  Index<SetEntityType::kStmtLst> else_index) override;
    void SetIndex(Index<SetEntityType::kStmt> call_stmt,
                  Index<SetEntityType::kProc> proc_index) override;
    void SetIndex(Index<SetEntityType::kStmt> assign_stmt,
                  Index<SetEntityType::kNotation> notation_index) override;
    void SetLst(Index<SetEntityType::kStmtLst> stmtlst_index,
                std::vector<int> stmtlst) override;

    // set direct Uses and Modifies relationships
    void SetRel(Index<SetEntityType::kStmt> stmt_no,
                Index<SetEntityType::kVar> var_index) override;
    void SetRel(Index<SetEntityType::kStmt> stmt_no,
                std::vector<int> var_index) override;

    bool ExistFollows(bool transitive, Index<ArgPos::kFirst> first_stmt,
                      Index<ArgPos::kSecond> second_stmt) override;
    bool ExistFollows(Index<ArgPos::kFirst> first_stmt) override;
    bool ExistFollows(Index<ArgPos::kSecond> second_stmt) override;
    bool ExistFollows() override;

    bool ExistParent(bool transitive, Index<ArgPos::kFirst> parent_stmt,
                     Index<ArgPos::kSecond> child_stmt) override;
    bool ExistParent(Index<ArgPos::kFirst> parent_stmt) override;
    bool ExistParent(Index<ArgPos::kSecond> child_stmt) override;
    bool ExistParent() override;

    std::set<int> GetFollows(ArgPos return_pos, StmtType return_type) override;

    std::set<int> GetFollows(bool transitive, Index<ArgPos::kFirst> first_stmt,
                             StmtType return_type) override;

    std::set<int> GetFollows(bool transitive,
                             Index<ArgPos::kSecond> second_stmt,
                             StmtType return_type) override;
    PairVec<int> GetFollowsPairs(bool transitive, StmtType first_type,
                                 StmtType second_type) override;

    std::set<int> GetParent(ArgPos return_pos, StmtType return_type) override;
    std::set<int> GetParent(bool transitive, Index<ArgPos::kFirst> stmt_no,
                            StmtType return_type) override;

    std::set<int> GetParent(bool transitive, Index<ArgPos::kSecond> stmt_no,
                            StmtType return_type) override;
    PairVec<int> GetParentPairs(bool transitive, StmtType parent_type,
                                StmtType child_type) override;

    bool ExistModifies(int stmt_no, int var_index) override;
    bool ExistUses(int stmt_no, int var_index) override;

    std::set<int> GetModifies(Index<QueryEntityType::kStmt> stmt_no) override;
    std::set<int> GetModifies(Index<QueryEntityType::kVar> var_index,
                              StmtType type) override;
    std::set<int> GetModifies(StmtType type) override;
    PairVec<int> GetModifiesStmtVar(StmtType type) override;
    std::set<int> GetUses(Index<QueryEntityType::kStmt> stmt_no) override;
    std::set<int> GetUses(Index<QueryEntityType::kVar> var_index,
                          StmtType type) override;
    std::set<int> GetUses(StmtType type) override;
    PairVec<int> GetUsesStmtVar(StmtType type) override;

    // ( _, " "), (_ , _" "_)
    std::set<int> GetPattern(std::vector<QueryToken> tokens,
                             bool partial_match) override;

    //(" ", _)
    std::set<int> GetPattern(int var_index) override;

    // (" ", " ") , (" ", _" "_)
    std::set<int> GetPattern(int var_index,
                             std::vector<QueryToken> second_tokens,
                             bool partial_match) override;
    // (v, " ")  , (v, _" "_)
    PairVec<int> GetPatternPair(std::vector<QueryToken> tokens,
                                bool partial_match) override;

    // (v, _)
    PairVec<int> GetPatternPair() override;

    bool ExistCalls(Index<ArgPos::kFirst> first_proc,
                    Index<ArgPos::kSecond> second_proc) override;
    bool ExistCallsT(Index<ArgPos::kFirst> first_proc,
                     Index<ArgPos::kSecond> second_proc) override;
    bool ExistCalls(Index<ArgPos::kFirst> first_proc) override;
    bool ExistCalls(Index<ArgPos::kSecond> second_proc) override;
    bool ExistCalls() override;
    std::set<int> GetCalls(ArgPos return_pos) override;
    std::set<int> GetCalls(Index<ArgPos::kFirst> first_proc) override;
    std::set<int> GetCallsT(Index<ArgPos::kFirst> first_proc) override;
    std::set<int> GetCalls(Index<ArgPos::kSecond> second_proc) override;
    std::set<int> GetCallsT(Index<ArgPos::kSecond> second_proc) override;
    PairVec<int> GetCallsPairs() override;
    PairVec<int> GetCallsTPairs() override;

    std::vector<int> GetAllEntityIndices(QueryEntityType et) override;
    std::vector<int> GetAllEntityIndices(StmtType st) override;

    void IndexToName(QueryEntityType et, const std::vector<int> &index_list,
                     std::list<std::string> &names) override;
    int NameToIndex(QueryEntityType et, const std::string &name) override;

    // mark the end of source processor
    void Compile() override;

  private:
    bool compiled = false;

    size_t stmt_count_;     // store number of stmt
    size_t stmtlst_count_;  // store number of stmtlst

    CallProcedureStore call_proc_;
    StmtlstParentStore stmtlst_parent_;
    StmtlstStatementsStore stmtlst_stmt_;
    ModifiesRelationshipStore modifies_rel_;
    UsesRelationshipStore uses_rel_;
    PolishNotationStore polish_notation_;
    NameValueStore name_value_;
    TypeStatementsStore type_stmt_;

    std::unique_ptr<ContainerForest> container_forest_;

    std::pair<PolishNotation, bool> ConvertFromQueryTokens(
            const std::vector<QueryToken> &tokens);

    int GetContainerLastStmt(StmtType type, int stmt_no);
    Pair<std::vector<int>::const_iterator> GetStmtBound(StmtType type,
                                                        int first_stmt,
                                                        int second_stmt);
    void AppendVarIndicesModifies(Pair<std::vector<int>::const_iterator> bound,
                                  std::set<int> &result);
    void AppendVarIndicesUses(Pair<std::vector<int>::const_iterator> bound,
                              std::set<int> &result);

    std::set<int> GetAllParents(StmtType return_type);
    std::set<int> GetAllChildren(StmtType return_type);
    void GetNonTransitiveParentFirst(StmtType parent_type, int parent,
                                     std::vector<int> &results) const;
    void GetTransitiveParentFirst(StmtType parent_type, int parent,
                                  std::vector<int> &results);
    void GetTransitiveParentPairs(PairVec<int> &results);
    void GetNonTransitiveParentPairs(PairVec<int> &results);
};

}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
