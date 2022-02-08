#ifndef SRC_SPA_SRC_SOURCE_CALL_H_
#define SRC_SPA_SRC_SOURCE_CALL_H_

#include "statement.h"
#include "procedure.h"

#include <memory>

namespace spa {

class Call : public Statement {
  public:
	bool setProcedure(std::shared_ptr<Procedure> procedure);
	std::shared_ptr<Procedure> getProcedure();

  private:
	std::shared_ptr<Procedure> procedure_;
};

} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_CALL_H_
