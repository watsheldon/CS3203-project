#ifndef SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
#define SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <utility>
#include <vector>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/polish_notation.h"
#include "common/polish_notation_node.h"
#include "knowledge_base.h"
#include "pkb/secondary_structure/affects_calculator.h"
#include "pkb/secondary_structure/next_calculator.h"
#include "pkb/store/calls_relationship_store.h"
#include "pkb/store/follows_relationship_store.h"
#include "pkb/store/modifies_relationship_store.h"
#include "pkb/store/name_value_store.h"
#include "pkb/store/parent_relationship_store.h"
#include "pkb/store/polish_notation_store.h"
#include "pkb/store/stmtlst_statements_store.h"
#include "pkb/store/type_statements_store.h"
#include "pkb/store/uses_relationship_store.h"
#include "secondary_structure/container_forest.h"
#include "secondary_structure/control_flow_graph.h"

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
                std::vector<StmtNo> stmtlst) override;

    // set direct Uses and Modifies relationships
    void SetRel(Index<SetEntityType::kStmt> stmt_no,
                Index<SetEntityType::kVar> var_index) override;
    void SetRel(Index<SetEntityType::kStmt> stmt_no,
                std::vector<VarIndex> var_index) override;

    bool ExistFollows(Index<ArgPos::kFirst> first_stmt,
                      Index<ArgPos::kSecond> second_stmt) override;
    bool ExistFollowsT(Index<ArgPos::kFirst> first_stmt,
                       Index<ArgPos::kSecond> second_stmt) override;
    bool ExistFollows(Index<ArgPos::kFirst> first_stmt) override;
    bool ExistFollows(Index<ArgPos::kSecond> second_stmt) override;
    bool ExistFollows() override;

    bool ExistParent(Index<ArgPos::kFirst> parent_stmt,
                     Index<ArgPos::kSecond> child_stmt) override;
    bool ExistParentT(Index<ArgPos::kFirst> parent_stmt,
                      Index<ArgPos::kSecond> child_stmt) override;
    bool ExistParent(Index<ArgPos::kFirst> parent_stmt) override;
    bool ExistParent(Index<ArgPos::kSecond> child_stmt) override;
    bool ExistParent() override;

    std::set<StmtNo> GetFollows(ArgPos return_pos,
                                StmtType return_type) override;

    std::set<StmtNo> GetFollows(Index<ArgPos::kFirst> stmt_no,
                                StmtType return_type) override;
    std::set<StmtNo> GetFollowsT(Index<ArgPos::kFirst> stmt_no,
                                 StmtType return_type) override;

    std::set<StmtNo> GetFollows(Index<ArgPos::kSecond> stmt_no,
                                StmtType return_type) override;
    std::set<StmtNo> GetFollowsT(Index<ArgPos::kSecond> stmt_no,
                                 StmtType return_type) override;
    PairVec<StmtNo> GetFollowsPairs(StmtType first_type,
                                    StmtType second_type) override;
    PairVec<StmtNo> GetFollowsTPairs(StmtType first_type,
                                     StmtType second_type) override;

    std::set<StmtNo> GetParent(ArgPos return_pos,
                               StmtType return_type) override;
    std::set<StmtNo> GetParent(Index<ArgPos::kFirst> parent_stmt,
                               StmtType return_type) override;
    std::set<StmtNo> GetParentT(Index<ArgPos::kFirst> parent_stmt,
                                StmtType return_type) override;

    std::set<StmtNo> GetParent(Index<ArgPos::kSecond> child_stmt,
                               StmtType return_type) override;
    std::set<StmtNo> GetParentT(Index<ArgPos::kSecond> child_stmt,
                                StmtType return_type) override;
    PairVec<StmtNo> GetParentPairs(StmtType parent_type,
                                   StmtType child_type) override;
    PairVec<StmtNo> GetParentTPairs(StmtType parent_type,
                                    StmtType child_type) override;

    bool ExistModifies(StmtNo stmt_no, std::string_view var_name) override;
    bool ExistModifies(StmtNo stmt_no, VarIndex var_index) override;
    bool ExistUses(StmtNo stmt_no, std::string_view var_name) override;
    bool ExistUses(StmtNo stmt_no, VarIndex var_index) override;

    std::set<VarIndex> GetModifies(
            Index<QueryEntityType::kStmt> stmt_no) override;
    std::set<StmtNo> GetModifies(std::string_view var_name,
                                 StmtType type) override;
    std::set<StmtNo> GetModifies(Index<QueryEntityType::kVar> var_index,
                                 StmtType type) override;
    std::set<StmtNo> GetModifies(StmtType type) override;
    PairVec<StmtNo, VarIndex> GetModifiesStmtVar(StmtType type) override;
    std::set<VarIndex> GetUses(Index<QueryEntityType::kStmt> stmt_no) override;
    std::set<StmtNo> GetUses(std::string_view var_name, StmtType type) override;
    std::set<StmtNo> GetUses(Index<QueryEntityType::kVar> var_index,
                             StmtType type) override;
    std::set<StmtNo> GetUses(StmtType type) override;
    PairVec<StmtNo, VarIndex> GetUsesStmtVar(StmtType type) override;

    // Modifies and Uses for Procedure
    bool ExistModifies(std::string_view proc_name,
                       std::string_view var_name) override;
    bool ExistModifies(std::string_view proc_name) override;
    std::set<VarIndex> GetModifies(Name<ArgPos::kFirst> proc_name) override;
    std::set<ProcIndex> GetModifies(Name<ArgPos::kSecond> var_name) override;
    std::set<ProcIndex> GetModifiesProc() override;
    PairVec<ProcIndex, VarIndex> GetModifiesProcVar() override;
    bool ExistUses(std::string_view proc_name,
                   std::string_view var_name) override;
    bool ExistUses(std::string_view proc_name) override;
    std::set<VarIndex> GetUses(Name<ArgPos::kFirst> proc_name) override;
    std::set<ProcIndex> GetUses(Name<ArgPos::kSecond> var_name) override;
    std::set<ProcIndex> GetUsesProc() override;
    PairVec<ProcIndex, VarIndex> GetUsesProcVar() override;

    std::set<StmtNo> GetPattern(std::vector<QueryToken> tokens) override;
    std::set<StmtNo> GetPatternP(std::vector<QueryToken> tokens) override;
    std::set<StmtNo> GetPattern(std::string_view var_name) override;
    std::set<StmtNo> GetPattern(VarIndex var_index) override;
    std::set<StmtNo> GetPattern(std::string_view var_name,
                                std::vector<QueryToken> second_tokens) override;
    std::set<StmtNo> GetPatternP(
            std::string_view var_name,
            std::vector<QueryToken> second_tokens) override;
    std::set<StmtNo> GetPattern(VarIndex var_index,
                                std::vector<QueryToken> second_tokens) override;
    std::set<StmtNo> GetPatternP(
            VarIndex var_index, std::vector<QueryToken> second_tokens) override;
    PairVec<StmtNo, VarIndex> GetPatternPair(
            std::vector<QueryToken> tokens) override;
    PairVec<StmtNo, VarIndex> GetPatternPairP(
            std::vector<QueryToken> tokens) override;
    PairVec<StmtNo, VarIndex> GetPatternPair() override;

    std::set<StmtNo> GetPattern(StmtType container_type,
                                std::string_view var_name) override;
    std::set<StmtNo> GetPattern(StmtType container_type,
                                VarIndex var_index) override;
    PairVec<StmtNo, VarIndex> GetPatternPairs(StmtType container_type) override;
    std::set<StmtNo> GetPattern(StmtType container_type) override;

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
    std::set<ProcIndex> GetCalls(ArgPos return_pos) override;
    std::set<ProcIndex> GetCalls(Index<ArgPos::kFirst> proc) override;
    std::set<ProcIndex> GetCalls(Name<ArgPos::kFirst> proc_name) override;
    std::set<ProcIndex> GetCallsT(Index<ArgPos::kFirst> first_proc) override;
    std::set<ProcIndex> GetCallsT(Name<ArgPos::kFirst> proc_name) override;
    std::set<ProcIndex> GetCalls(Index<ArgPos::kSecond> proc) override;
    std::set<ProcIndex> GetCalls(Name<ArgPos::kSecond> proc_name) override;
    std::set<ProcIndex> GetCallsT(Index<ArgPos::kSecond> proc) override;
    std::set<ProcIndex> GetCallsT(Name<ArgPos::kSecond> proc_name) override;
    PairVec<ProcIndex> GetCallsPairs() override;
    PairVec<ProcIndex> GetCallsTPairs() override;

    bool ExistNext(Index<ArgPos::kFirst> first_stmt,
                   Index<ArgPos::kSecond> second_stmt) override;
    bool ExistNextT(Index<ArgPos::kFirst> first_stmt,
                    Index<ArgPos::kSecond> second_stmt) override;
    bool ExistNext(Index<ArgPos::kFirst> first_stmt) override;
    bool ExistNext(Index<ArgPos::kSecond> second_stmt) override;
    bool ExistNext() override;
    std::set<StmtNo> GetNext(ArgPos return_pos, StmtType return_type) override;
    std::set<StmtNo> GetNext(Index<ArgPos::kFirst> stmt,
                             StmtType return_type) override;
    std::set<StmtNo> GetNextT(Index<ArgPos::kFirst> stmt,
                              StmtType return_type) override;
    std::set<StmtNo> GetNext(Index<ArgPos::kSecond> stmt,
                             StmtType return_type) override;
    std::set<StmtNo> GetNextT(Index<ArgPos::kSecond> stmt,
                              StmtType return_type) override;
    PairVec<StmtNo> GetNextPairs(StmtType first_type,
                                 StmtType second_type) override;
    PairVec<StmtNo> GetNextTPairs(StmtType first_type,
                                  StmtType second_type) override;
    std::set<StmtNo> GetNextTSelf(StmtType type) override;

    bool ExistAffects(Index<ArgPos::kFirst> first_assign,
                      Index<ArgPos::kSecond> second_assign) override;
    bool ExistAffectsT(Index<ArgPos::kFirst> first_assign,
                       Index<ArgPos::kSecond> second_assign) override;
    bool ExistAffects(Index<ArgPos::kFirst> first_assign) override;
    bool ExistAffects(Index<ArgPos::kSecond> second_assign) override;
    bool ExistAffects() override;
    std::set<StmtNo> GetAffects(ArgPos return_pos) override;
    std::set<StmtNo> GetAffects(Index<ArgPos::kFirst> assign) override;
    std::set<StmtNo> GetAffectsT(Index<ArgPos::kFirst> assign) override;
    std::set<StmtNo> GetAffects(Index<ArgPos::kSecond> assign) override;
    std::set<StmtNo> GetAffectsT(Index<ArgPos::kSecond> assign) override;
    PairVec<StmtNo> GetAffectsPairs() override;
    PairVec<StmtNo> GetAffectsTPairs() override;
    std::set<StmtNo> GetAffectsSelf() override;
    std::set<StmtNo> GetAffectsTSelf() override;

    std::vector<int> GetAllEntityIndices(QueryEntityType et) override;
    std::vector<int> GetAllEntityIndices(StmtType st) override;

    std::vector<std::string_view> GetNames(
            const std::vector<int> &indices,
            QueryEntityType entity_type) override;
    std::vector<std::string_view> GetAttr(const std::vector<int> &indices,
                                          StmtType stmt_type) override;
    void ToName(QueryEntityType et, const std::vector<int> &index_list,
                std::list<std::string> &names) override;
    void ToAttr(StmtType et, const std::vector<StmtNo> &index_list,
                std::list<std::string> &names) override;
    int IdentToIndexValue(std::string_view name, QueryEntityType et) override;

    // mark the end of source processor
    void Compile() override;
    // Clear cache at the start of each query.
    void ClearCache() override;

  private:
    bool compiled = false;

    size_t stmt_count_;     // store number of stmt
    size_t stmtlst_count_;  // store number of stmtlst

    CallsRelationshipStore call_proc_;
    StmtlstParentStore stmtlst_parent_;
    StmtlstStatementsStore stmtlst_stmt_;
    ModifiesRelationshipStore modifies_rel_;
    UsesRelationshipStore uses_rel_;
    NameValueStore name_value_;
    PolishNotationStore polish_notation_;
    TypeStatementsStore type_stmt_;

    std::unique_ptr<ContainerForest> container_forest_;
    std::unique_ptr<ControlFlowGraph> cfg_;
    std::unique_ptr<NextCalculator> next_;
    std::unique_ptr<AffectsCalculator> affects_;
    FollowsRelationshipStore follows_store_;
    ParentRelationshipStore parent_store_;

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
    void ExtractReadModifies(const std::vector<StmtNo> &read_stmts,
                             std::list<std::string> &output) const noexcept;
    void ExtractPrintUses(const std::vector<StmtNo> &print_stmts,
                          std::list<std::string> &output) const noexcept;
    void ExtractCallProcNames(const std::vector<StmtNo> &call_stmts,
                              std::list<std::string> &output) const noexcept;
    [[nodiscard]] std::vector<IdentView> ExtractReadModifies(
            const std::vector<int> &read_stmts) const noexcept;
    [[nodiscard]] std::vector<IdentView> ExtractPrintUses(
            const std::vector<int> &print_stmts) const noexcept;
    [[nodiscard]] std::vector<IdentView> ExtractCallProcNames(
            const std::vector<StmtNo> &call_stmts) const noexcept;
};

}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
