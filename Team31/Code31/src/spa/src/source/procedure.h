#ifndef SRC_SPA_SRC_SOURCE_PROCEDURE_H_
#define SRC_SPA_SRC_SOURCE_PROCEDURE_H_

#include "indexed_ast_node.h"
#include "stmt_lst.h"

#include <memory>

namespace spa {

class Procedure : public IndexedAstNode<Procedure> {
	public:
	Procedure(std::shared_ptr<StmtLst> stmtLst);
	bool setStmtLst(std::shared_ptr<StmtLst> stmtLst);
	std::shared_ptr<StmtLst> getStmtLst();

	private:
	std::shared_ptr<StmtLst> stmtLst;
};

} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_PROCEDURE_H_