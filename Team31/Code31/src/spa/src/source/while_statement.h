#ifndef SRC_SPA_SRC_SOURCE_WHILE_STATEMENT_H_
#define SRC_SPA_SRC_SOURCE_WHILE_STATEMENT_H_

#include "container_statement.h"
#include "stmt_lst.h"

#include <memory>

namespace spa {

class WhileStatement : public ContainerStatement {
  public:
	bool setStmtLst(std::shared_ptr<StmtLst> stmtLst);
	std::shared_ptr<StmtLst> getStmtLst();

  private:
	std::shared_ptr<StmtLst> stmtLst_;
};

} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_WHILE_STATEMENT_H_
