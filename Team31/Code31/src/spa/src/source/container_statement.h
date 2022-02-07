#ifndef SRC_SPA_SRC_SOURCE_CONTAINER_STATEMENT_H_
#define SRC_SPA_SRC_SOURCE_CONTAINER_STATEMENT_H_

#include "statement.h"
#include "condition.h"

#include <memory>

namespace spa {

class ContainerStatement : public Statement {
  public:
    virtual ~ContainerStatement() = 0;
	bool setCondition(std::shared_ptr<Condition> condition);
	std::shared_ptr<Condition> getCondition();

  protected:
	std::shared_ptr<Condition> condition_;
};

} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_CONTAINER_STATEMENT_H_
