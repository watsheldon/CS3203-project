#ifndef SRC_SPA_SRC_QPS_ENT_REF_H_
#define SRC_SPA_SRC_QPS_ENT_REF_H_

#include <cassert>
#include <string>

#include "synonym.h"

namespace spa {
struct EntRef {
  private:
    const std::string identifier = "";
    const Synonym *syn = nullptr;

    EntRef(std::string str) : identifier(std::move(str)) {}
    EntRef(const Synonym *syn_ptr) : syn(syn_ptr) {
        assert(syn->type == DeclarationType::VARIABLE);
    }
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_ENT_REF_H_
