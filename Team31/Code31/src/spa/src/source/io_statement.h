#ifndef SRC_SPA_SRC_SOURCE_IO_STATEMENT_H_
#define SRC_SPA_SRC_SOURCE_IO_STATEMENT_H_

#include "statement.h"
#include "variable.h"

#include <memory>

namespace spa {

class IoStatement : public Statement {
  public: 
    virtual ~IoStatement() = 0;
	bool setVariable(std::shared_ptr<Variable> variable);
	int getVariableIndex() const;

  private:
	std::shared_ptr<Variable> variable_;
};

} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_IO_STATEMENT_H_