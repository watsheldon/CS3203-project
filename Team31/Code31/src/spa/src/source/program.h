#ifndef SRC_SPA_SRC_SOURCE_PROGRAM_H_
#define SRC_SPA_SRC_SOURCE_PROGRAM_H_

#include <memory>

namespace spa {

class Program : public AstNode {
	public: 
	bool addProcedure(std::shared_ptr<Procedure> procedure);
	SharedPtrVec<Procedure> getProcedures() const;

	private:
	SharedPtrVec<Procedure> procedures;
};

} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_PROGRAM_H_