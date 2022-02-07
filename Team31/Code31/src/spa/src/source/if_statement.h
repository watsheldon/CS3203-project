#ifndef SRC_SPA_SRC_SOURCE_IF_STATEMENT_H_
#define SRC_SPA_SRC_SOURCE_IF_STATEMENT_H_

#include "container_statement.h"
#include "stmt_lst.h"

#include <memory>

namespace spa {

class IfStatement : public ContainerStatement {
  public:
	bool setThenLst(std::shared_ptr<StmtLst> thenLst);
	bool setElseLst(std::shared_ptr<StmtLst> elseLst);
	std::shared_ptr<StmtLst> getThenLst();
	std::shared_ptr<StmtLst> getElseLst();

  private:
	std::shared_ptr<StmtLst> thenLst_, elseLst_;
};

} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_IF_STATEMENT_H_
