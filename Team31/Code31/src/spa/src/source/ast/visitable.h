#ifndef SRC_SPA_SRC_SOURCE_AST_VISITABLE_H_
#define SRC_SPA_SRC_SOURCE_AST_VISITABLE_H_

#include "visitor.h"
namespace spa {
class Visitable {
  public:
    virtual void Accept(AstVisitor &visitor) const noexcept = 0;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_AST_VISITABLE_H_
