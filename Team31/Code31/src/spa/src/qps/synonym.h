#ifndef SRC_SPA_SRC_QPS_SYNONYM_H_
#define SRC_SPA_SRC_QPS_SYNONYM_H_

#include <cassert>
#include <string_view>

namespace spa {
struct Synonym {
    const enum Type {
        kStmtAny,
        kStmtRead,
        kStmtPrint,
        kStmtCall,
        kStmtWhile,
        kStmtIf,
        kStmtAssign,
        kVar,
        kConst,
        kProc,
        kNone
    } type;
    explicit Synonym(Type t) : type(t) { assert(t != kNone); }
    void IncRef() { ++ref_count_; }
    void IncResolved() { ++resolved_count_; }
    [[nodiscard]] int GetRef() const { return ref_count_; }
    [[nodiscard]] int GetResolved() const { return resolved_count_; }

  private:
    int ref_count_ = 0;
    int resolved_count_ = 0;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_SYNONYM_H_
