#ifndef SRC_SPA_SRC_QPS_CONDITIONS_USESMODIFIESBASE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_USESMODIFIESBASE_H_

#include "qps/conditions/condition_clause.h"

namespace spa {
class UsesModifiesBase : public ConditionClause {
  public:
    enum Type {
        kIntIdent,
        kIntSyn,
        kIntWild,
        kSynIdent,
        kSynSyn,
        kSynWild,
        kIdentSyn,
        kIdentIdent,
        kIdentWild
    };

    UsesModifiesBase(int first, std::string second);
    UsesModifiesBase(int first, Synonym *second);
    explicit UsesModifiesBase(int first);
    UsesModifiesBase(Synonym *first, std::string second);
    UsesModifiesBase(Synonym *first, Synonym *second);
    explicit UsesModifiesBase(Synonym *first);
    UsesModifiesBase(std::string first, Synonym *second);
    UsesModifiesBase(std::string first, std::string second);
    explicit UsesModifiesBase(std::string first);
    ResultTable Execute(KnowledgeBase *knowledge_base) const override = 0;
    ~UsesModifiesBase() override = 0;

  protected:
    Type type_;
    int first_int_;
    Synonym *first_syn_;
    std::string first_ident_;
    int second_int_;
    Synonym *second_syn_;
    std::string second_ident_;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_USESMODIFIESBASE_H_
