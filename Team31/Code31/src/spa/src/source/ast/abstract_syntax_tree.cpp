#include "abstract_syntax_tree.h"

#include <utility>

#include "source/token.h"

namespace spa {
AbstractSyntaxTree::AbstractSyntaxTree(UniquePtrTokens tokens)
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
            case SourceTokenType::kKeywordProcedure:
                AddProcedure(tokens_->at(i + 1).value);
                i += 3;
                break;
            case SourceTokenType::kKeywordRead:
                basic_entities_->reads.emplace_back(line_number++);
                AddVariable(tokens_->at(i + 1).value);
                i += 3;
                break;
            case SourceTokenType::kKeywordPrint:
                basic_entities_->prints.emplace_back(line_number++);
                AddVariable(tokens_->at(i + 1).value);
                i += 3;
                break;
            case SourceTokenType::kKeywordCall:
                basic_entities_->calls.emplace_back(line_number++);
                i += 3;
                break;
            case SourceTokenType::kKeywordWhile:
                basic_entities_->whiles.emplace_back(line_number++);
                while (tokens_->at(++i).type != SourceTokenType::kBraceL) {
                    switch (tokens_->at(i).type) {
                        case SourceTokenType::kName:
                            AddVariable(tokens_->at(i).value);
                            break;
                        case SourceTokenType::kInteger:
                            AddConstant(tokens_->at(i).value);
                            break;
                        default:
                            break;
                    }
                }
                ++i;
                break;
            case SourceTokenType::kKeywordIf:
                basic_entities_->ifs.emplace_back(line_number++);
                while (tokens_->at(++i).type != SourceTokenType::kBraceL) {
                    switch (tokens_->at(i).type) {
                        case SourceTokenType::kName:
                            AddVariable(tokens_->at(i).value);
                            break;
                        case SourceTokenType::kInteger:
                            AddConstant(tokens_->at(i).value);
                            break;
                        default:
                            break;
                    }
                }
                ++i;
                break;
            case SourceTokenType::kAssignEqual:
                AddVariable(tokens_->at(i - 1).value);
                basic_entities_->assigns.emplace_back(line_number++);
                while (tokens_->at(++i).type != SourceTokenType::kSemicolon) {
                    switch (tokens_->at(i).type) {
                        case SourceTokenType::kName:
                            AddVariable(tokens_->at(i).value);
                            break;
                        case SourceTokenType::kInteger:
                            AddConstant(tokens_->at(i).value);
                            break;
                        default:
                            break;
                    }
                }
                ++i;
                break;
            case SourceTokenType::kKeywordThen:
            case SourceTokenType::kKeywordElse:
            case SourceTokenType::kBracketL:
            case SourceTokenType::kBracketR:
            case SourceTokenType::kBraceL:
            case SourceTokenType::kBraceR:
            case SourceTokenType::kOperatorPlus:
            case SourceTokenType::kOperatorMinus:
            case SourceTokenType::kOperatorTimes:
            case SourceTokenType::kOperatorDivide:
            case SourceTokenType::kOperatorModulo:
            case SourceTokenType::kCondNot:
            case SourceTokenType::kCondAnd:
            case SourceTokenType::kCondOr:
            case SourceTokenType::kRelLt:
            case SourceTokenType::kRelLeq:
            case SourceTokenType::kRelEq:
            case SourceTokenType::kRelNeq:
            case SourceTokenType::kRelGt:
            case SourceTokenType::kRelGeq:
            case SourceTokenType::kSemicolon:
            case SourceTokenType::kName:
            case SourceTokenType::kInteger:
                ++i;
                break;
        }
    }
}
std::shared_ptr<BasicEntities> AbstractSyntaxTree::getInitEntities() const {
    return basic_entities_;
}
void AbstractSyntaxTree::AddProcedure(const std::string &name) {
    auto [itr, b] = procedures_.emplace(name);
    if (b) {
        basic_entities_->procedures.emplace_back(name);
    }
}
void AbstractSyntaxTree::AddVariable(const std::string &name) {
    auto [itr, b] = variables_.emplace(name);
    if (b) {
        basic_entities_->variables.emplace_back(name);
    }
}
void AbstractSyntaxTree::AddConstant(const std::string &value) {
    auto [itr, b] = constants_.emplace(value);
    if (b) {
        basic_entities_->constants.emplace_back(value);
    }
}
}  // namespace spa
