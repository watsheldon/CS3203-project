#ifndef SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
#define SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <list>
#include <memory>
#include <numeric>
#include <set>
#include <string>
#include <vector>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/polish_notation.h"
#include "common/polish_notation_node.h"
#include "knowledge_base.h"
#include "pkb/store/call_procedure_store.h"
#include "pkb/store/constant_value_store.h"
#include "pkb/store/modifies_relationship_store.h"
#include "pkb/store/polish_notation_store.h"
#include "pkb/store/procedure_name_store.h"
#include "pkb/store/stmtlst_statements_store.h"
#include "pkb/store/type_statements_store.h"
#include "pkb/store/uses_relationship_store.h"
#include "pkb/store/variable_name_store.h"
#include "secondary_structure/container_forest.h"
#include "store/stmtlst_parent_store.h"

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
    std::pair<std::vector<int>, std::vector<int>> GetFollowsPairs(
            bool transitive, StmtType first_type,
            StmtType second_type) override;

    std::set<int> GetParent(ArgPos return_pos, StmtType return_type) override;
    std::set<int> GetParent(bool transitive, Index<ArgPos::kFirst> stmt_no,
                            StmtType return_type) override;

    std::set<int> GetParent(bool transitive, Index<ArgPos::kSecond> stmt_no,
                            StmtType return_type) override;
    std::pair<std::vector<int>, std::vector<int>> GetParentPairs(
            bool transitive, StmtType parent_type,
            StmtType child_type) override;

    bool ExistModifies(int stmt_no, int var_index) override;
    bool ExistUses(int stmt_no, int var_index) override;

    std::vector<int> GetModifies(Index<QueryEntityType::kStmt> stmt_no,
                                 const std::vector<int> &filtered_var) override;
    std::vector<int> GetModifies(
            Index<QueryEntityType::kVar> var_index,
            const std::vector<int> &filtered_stmt) override;
    std::vector<int> GetUses(Index<QueryEntityType::kStmt> stmt_no,
                             const std::vector<int> &filtered_var) override;
    std::vector<int> GetUses(Index<QueryEntityType::kVar> var_index,
                             const std::vector<int> &filtered_stmt) override;

    std::vector<int> GetAllEntityIndices(QueryEntityType et) override;
    std::vector<int> GetAllEntityIndices(StmtType st) override;

    void IndexToName(QueryEntityType et, const std::vector<int> &index_list,
                     std::list<std::string> &names) override;

    // mark the end of source processor
    void Compile() override;

  private:
    bool compiled = false;
    const int STMT_TYPE_COUNT = 6;

    size_t stmt_count_;     // store number of stmt
    size_t stmtlst_count_;  // store number of stmtlst

    StmtlstParentStore stmtlst_parent_;
    ProcedureNameStore proc_name_;
    VariableNameStore var_name_;
    ConstantValueStore const_value_;
    StmtlstStatementsStore stmtlst_stmt_;
    TypeStatementsStore type_stmt_;
    ModifiesRelationshipStore modifies_rel_;
    UsesRelationshipStore uses_rel_;
    CallProcedureStore call_proc_;
    PolishNotationStore polish_notation_;

    std::unique_ptr<ContainerForest> container_forest_;

    bool ContainsUnseenVarConst(std::vector<QueryToken> tokens);
    PolishNotation ConvertFromQueryTokens(std::vector<QueryToken> tokens);

    std::set<int> GetAllParents(StmtType return_type);
    std::set<int> GetAllChildren(StmtType return_type);
    void GetNonTransitiveParentFirst(StmtType parent_type, int parent,
                                     std::vector<int> &results) const;
    void GetTransitiveParentFirst(std::vector<int> parent_follower, int parent,
                                  std::vector<int> &results) const;
    void GetTransitiveParentPairs(
            std::pair<std::vector<int>, std::vector<int>> &results);
    void GetNonTransitiveParentPairs(
            std::pair<std::vector<int>, std::vector<int>> &results);
};

}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_PROGRAM_KNOWLEDGE_BASE_H_
