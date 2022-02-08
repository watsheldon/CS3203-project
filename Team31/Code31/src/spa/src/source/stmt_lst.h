#ifndef SRC_SPA_SRC_SOURCE_STMT_LST_H_
#define SRC_SPA_SRC_SOURCE_STMT_LST_H_

#include "indexed_ast_node.h"
#include "statement.h"

#include <string>
#include <cstring>
#include <memory>

namespace spa {

class StmtLst : public IndexedAstNode<StmtLst> {
  public:
	bool setStmtLst(std::shared_ptr<StmtLst> stmtLst);
	bool addStatement(std::shared_ptr<Statement> statement);
	SharedPtrVec<Statement> getStatements();

  private:
	std::shared_ptr<StmtLst> stmtLst_;
	SharedPtrVec<Statement> statements;
};

} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_STMT_LST_H_