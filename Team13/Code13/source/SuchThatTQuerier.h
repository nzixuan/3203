#pragma once
#include <list>

#include "SuchThatQuerier.h"

class SuchThatTQuerier : public SuchThatQuerier {
 private:
  SuchThatQueriable* queriable;

 public:
  SuchThatTQuerier();
  explicit SuchThatTQuerier(SuchThatQueriable* queriable);
  ~SuchThatTQuerier();

  void setStorage(SuchThatQueriable* nextStorage);

  std::list<PKBEntityPair> getLhsSynRhsSyn(EntityType lhsType,
                                           EntityType rhsType) override;
  std::list<PKBEntity> getLhsRhsSameSynObj(EntityType type) override;
  std::list<PKBEntity> getLhsSynRhsWildcard(EntityType lhsType) override;
  std::list<PKBEntity> getLhsSynRhsConstant(EntityType lhsType,
                                            QueryConstant rhsConstant) override;

  std::list<PKBEntity> getLhsWildcardRhsSyn(EntityType rhsType) override;
  bool hasLhsWildcardRhsWildcard() override;
  bool hasLhsWildcardRhsConstant(QueryConstant rhsConstant) override;

  std::list<PKBEntity> getLhsConstantRhsSyn(QueryConstant lhsConstant,
                                            EntityType rhsType) override;
  bool hasLhsConstantRhsWildcard(QueryConstant lhsConstant) override;
  bool hasLhsConstantRhsConstant(QueryConstant lhsConstant,
                                 QueryConstant rhsConstant) override;
};
