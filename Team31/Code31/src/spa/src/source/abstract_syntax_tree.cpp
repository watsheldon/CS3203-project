#include "abstract_syntax_tree.h"
namespace spa {
AbstractSyntaxTree::AbstractSyntaxTree(SharedVecToken tokens)
        : tokens_(tokens) {

}
std::shared_ptr<BasicEntities> AbstractSyntaxTree::getInitEntities() const {
    return {};
}
}
