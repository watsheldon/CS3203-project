#include "calls_base.h"

#include <string>
#include <utility>

#include "qps/synonym.h"
namespace spa {
CallsBase::CallsBase(Synonym *first, Synonym *second) noexcept
        : type_(kSynSyn), first_syn_(first), second_syn_(second) {}
CallsBase::CallsBase(ArgPos pos, Synonym *syn) noexcept {
    if (pos == ArgPos::kFirst) {
        type_ = kSynWild;
        first_syn_ = syn;
    } else {
        type_ = Type::kWildSyn;
        second_syn_ = syn;
    }
}
CallsBase::CallsBase(Synonym *first, std::string second) noexcept
        : type_(kSynIdent),
          first_syn_(first),
          second_ident_(std::move(second)) {}
CallsBase::CallsBase(std::string first, Synonym *second) noexcept
        : type_(kIdentSyn),
          first_ident_(std::move(first)),
          second_syn_(second) {}
CallsBase::CallsBase(ArgPos pos, std::string ident) noexcept {
    if (pos == ArgPos::kFirst) {
        type_ = kIdentWild;
        first_ident_ = std::move(ident);
    } else {
        type_ = Type::kWildIdent;
        second_ident_ = std::move(ident);
    }
}
CallsBase::CallsBase(std::string first, std::string second) noexcept
        : type_(kIdentIdent),
          first_ident_(std::move(first)),
          second_ident_(std::move(second)) {}
CallsBase::CallsBase() : type_(kWildWild) {}
}  // namespace spa
