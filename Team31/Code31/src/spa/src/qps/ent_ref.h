#ifndef SPA_SRC_SPA_SRC_QPS_ENT_REF_H_
#define SPA_SRC_SPA_SRC_QPS_ENT_REF_H_

#include <string>

#include "declaration_cl.h"
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
#endif  // SPA_SRC_SPA_SRC_QPS_ENT_REF_H_
