#ifndef SRC_SPA_SRC_SOURCE_INDEXED_AST_NODE_H_
#define SRC_SPA_SRC_SOURCE_INDEXED_AST_NODE_H_

#include "ast_node.h"

namespace spa {

template <typename T>

class IndexedAstNode : public AstNode {
    public:
    int getIndex() const;

    protected:
    static int count;
    int index;	
};

} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_INDEXED_AST_NODE_H_