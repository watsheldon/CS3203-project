#include "abstract_syntax_tree.h"

#include <utility>
namespace spa {
AbstractSyntaxTree::AbstractSyntaxTree(SharedVecToken tokens)
        : tokens_(std::move(tokens)),
          basic_entities_(std::make_shared<BasicEntities>()) {
    basic_entities_->procedures.emplace_back();
    basic_entities_->variables.emplace_back();
    basic_entities_->constants.emplace_back();
    basic_entities_->reads.emplace_back();
    basic_entities_->prints.emplace_back();
    basic_entities_->calls.emplace_back();
    basic_entities_->whiles.emplace_back();
    basic_entities_->ifs.emplace_back();
    basic_entities_->assigns.emplace_back();
    int line_number = 1;
    int i = 0;
    while (i < tokens_->size()) {
        switch (tokens_->at(i).type) {
            case kKeywordProcedure:AddProcedure(tokens_->at(i + 1).value);
                i += 3;
                break;
            case kKeywordRead:basic_entities_->reads.emplace_back(line_number++);
                AddVariable(tokens_->at(i + 1).value);
                i += 3;
                break;
            case kKeywordPrint:basic_entities_->prints.emplace_back(line_number++);
                AddVariable(tokens_->at(i + 1).value);
                i += 3;
                break;
            case kKeywordCall:basic_entities_->calls.emplace_back(line_number++);
                i += 3;
                break;
            case kKeywordWhile:basic_entities_->whiles.emplace_back(line_number++);
                while (tokens_->at(++i).type != kBraceL) {
                    switch (tokens_->at(i).type) {
                        case kName:AddVariable(tokens_->at(i).value);
                            break;
                        case kInteger:AddConstant(tokens_->at(i).value);
                            break;
                        default:break;
                    }
                }
                ++i;
                break;
            case kKeywordIf:basic_entities_->ifs.emplace_back(line_number++);
                while (tokens_->at(++i).type != kBraceL) {
                    switch (tokens_->at(i).type) {
                        case kName:AddVariable(tokens_->at(i).value);
                            break;
                        case kInteger:AddConstant(tokens_->at(i).value);
                            break;
                        default:break;
                    }
                }
                ++i;
                break;
            case kAssignEqual:AddVariable(tokens_->at(i - 1).value);
                basic_entities_->assigns.emplace_back(line_number++);
                while (tokens_->at(++i).type != kSemicolon) {
                    switch (tokens_->at(i).type) {
                        case kName:AddVariable(tokens_->at(i).value);
                            break;
                        case kInteger:AddConstant(tokens_->at(i).value);
                            break;
                        default:break;
                    }
                }
                ++i;
                break;
            case kKeywordThen:
            case kKeywordElse:
            case kBracketL:
            case kBracketR:
            case kBraceL:
            case kBraceR:
            case kOperatorPlus:
            case kOperatorMinus:
            case kOperatorTimes:
            case kOperatorDivide:
            case kOperatorModulo:
            case kCondNot:
            case kCondAnd:
            case kCondOr:
            case kRelLt:
            case kRelLeq:
            case kRelEq:
            case kRelNeq:
            case kRelGt:
            case kRelGeq:
            case kSemicolon:
            case kName:
            case kInteger:++i;
                break;
        }
    }
}
std::shared_ptr<BasicEntities> AbstractSyntaxTree::getInitEntities() const {
    return basic_entities_;
}
void AbstractSyntaxTree::AddProcedure(const std::string &name) {
    auto[itr, b] = procedures_.emplace(name);
    if (b) {
        basic_entities_->procedures.emplace_back(name);
    }
}
void AbstractSyntaxTree::AddVariable(const std::string &name) {
    auto[itr, b] = variables_.emplace(name);
    if (b) {
        basic_entities_->variables.emplace_back(name);
    }
}
void AbstractSyntaxTree::AddConstant(const std::string &value) {
    auto[itr, b] = constants_.emplace(value);
    if (b) {
        basic_entities_->constants.emplace_back(value);
    }
}
}
