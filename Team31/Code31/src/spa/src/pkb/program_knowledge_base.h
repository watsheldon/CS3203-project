#ifndef SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
#define SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <utility>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/polish_notation.h"
#include "common/polish_notation_node.h"
#include "knowledge_base.h"
#include "pkb/store/calls_relationship_store.h"
#include "pkb/store/follows_parent_rel_store.h"
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

    bool ExistModifies(int stmt_no, std::string_view var_name) override;
    bool ExistModifies(int stmt_no, int var_index) override;
    bool ExistUses(int stmt_no, std::string_view var_name) override;
    bool ExistUses(int stmt_no, int var_index) override;

    std::set<int> GetModifies(Index<QueryEntityType::kStmt> stmt_no) override;
    std::set<int> GetModifies(std::string_view var_name,
                              StmtType type) override;
    std::set<int> GetModifies(Index<QueryEntityType::kVar> var_index,
                              StmtType type) override;
    std::set<int> GetModifies(StmtType type) override;
    PairVec<int> GetModifiesStmtVar(StmtType type) override;
    std::set<int> GetUses(Index<QueryEntityType::kStmt> stmt_no) override;
    std::set<int> GetUses(std::string_view var_name, StmtType type) override;
    std::set<int> GetUses(Index<QueryEntityType::kVar> var_index,
                          StmtType type) override;
    std::set<int> GetUses(StmtType type) override;
    PairVec<int> GetUsesStmtVar(StmtType type) override;

    // Modifies and Uses for Procedure
    bool ExistModifies(std::string_view proc_name,
                       std::string_view var_name) override;
    bool ExistModifies(std::string_view proc_name) override;
    std::set<int> GetModifies(Name<ArgPos::kFirst> proc_name) override;
    std::set<int> GetModifies(Name<ArgPos::kSecond> var_name) override;
    std::set<int> GetModifiesProc() override;
    PairVec<int> GetModifiesProcVar() override;
    bool ExistUses(std::string_view proc_name,
                   std::string_view var_name) override;
    bool ExistUses(std::string_view proc_name) override;
    std::set<int> GetUses(Name<ArgPos::kFirst> proc_name) override;
    std::set<int> GetUses(Name<ArgPos::kSecond> var_name) override;
    std::set<int> GetUsesProc() override;
    PairVec<int> GetUsesProcVar() override;

    // ( _, " "), (_ , _" "_)
    std::set<int> GetPattern(std::vector<QueryToken> tokens,
                             bool partial_match) override;

    //(" ", _)
    std::set<int> GetPattern(std::string_view var_name) override;
    std::set<int> GetPattern(int var_index) override;

    // (" ", " ") , (" ", _" "_)
    std::set<int> GetPattern(std::string_view var_name,
                             std::vector<QueryToken> second_tokens,
                             bool partial_match) override;
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
    bool ExistCalls(std::string_view first_proc_name,
                    std::string_view second_proc_name) override;
    bool ExistCallsT(Index<ArgPos::kFirst> first_proc,
                     Index<ArgPos::kSecond> second_proc) override;
    bool ExistCallsT(std::string_view first_proc_name,
                     std::string_view second_proc_name) override;
    bool ExistCalls(Index<ArgPos::kFirst> first_proc) override;
    bool ExistCalls(Name<ArgPos::kFirst> proc_name) override;
    bool ExistCalls(Index<ArgPos::kSecond> proc) override;
    bool ExistCalls(Name<ArgPos::kSecond> proc_name) override;
    bool ExistCalls() override;
    std::set<int> GetCalls(ArgPos return_pos) override;
    std::set<int> GetCalls(Index<ArgPos::kFirst> proc) override;
    std::set<int> GetCalls(Name<ArgPos::kFirst> proc_name) override;
    std::set<int> GetCallsT(Index<ArgPos::kFirst> first_proc) override;
    std::set<int> GetCallsT(Name<ArgPos::kFirst> proc_name) override;
    std::set<int> GetCalls(Index<ArgPos::kSecond> proc) override;
    std::set<int> GetCalls(Name<ArgPos::kSecond> proc_name) override;
    std::set<int> GetCallsT(Index<ArgPos::kSecond> proc) override;
    std::set<int> GetCallsT(Name<ArgPos::kSecond> proc_name) override;
    PairVec<int> GetCallsPairs() override;
    PairVec<int> GetCallsTPairs() override;

    std::vector<int> GetAllEntityIndices(QueryEntityType et) override;
    std::vector<int> GetAllEntityIndices(StmtType st) override;
    std::vector<int> GetAllEntityIndices(Synonym::Type synType) override;

    void ToName(Synonym::Type syn_type, const std::vector<int> &index_list,
                std::list<std::string> &names) override;

    // mark the end of source processor
    void Compile() override;

  private:
    bool compiled = false;

    size_t stmt_count_;     // store number of stmt
    size_t stmtlst_count_;  // store number of stmtlst

    CallsRelationshipStore call_proc_;
    StmtlstParentStore stmtlst_parent_;
    StmtlstStatementsStore stmtlst_stmt_;
    ModifiesRelationshipStore modifies_rel_;
    UsesRelationshipStore uses_rel_;
    PolishNotationStore polish_notation_;
    NameValueStore name_value_;
    TypeStatementsStore type_stmt_;

    std::unique_ptr<ContainerForest> container_forest_;

    FollowsParentRelStore follows_parent_rel_;

    std::pair<PolishNotation, bool> ConvertFromQueryTokens(
            const std::vector<QueryToken> &tokens);

    int IdentToIndexValue(std::string_view name, QueryEntityType et);
    template <QueryEntityType et>
    Index<et> IdentToIndex(std::string_view name) {
        return Index<et>(IdentToIndexValue(name, et));
    }
    template <ArgPos pos>
    Index<pos> IdentToIndex(std::string_view name,
                            QueryEntityType et) noexcept {
        return Index<pos>(IdentToIndexValue(name, et));
    }
    template <ArgPos pos>
    Index<pos> IdentToIndex(Name<pos> identifier, QueryEntityType et) noexcept {
        return IdentToIndex<pos>(identifier.value, et);
    }
};

}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
