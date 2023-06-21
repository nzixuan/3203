#pragma once

#include <list>

#include "PKBEntity.h"

class SuchThatQueriable {
 public:
  virtual std::list<PKBEntityPair> getLhsSynRhsSyn(EntityType lhsType,
                                                   EntityType rhsType) = 0;
  virtual std::list<PKBEntity> getLhsRhsSameSynObj(EntityType type) = 0;
  virtual std::list<PKBEntity> getLhsSynRhsWildcard(EntityType lhsType) = 0;
  virtual std::list<PKBEntity> getLhsSynRhsConstant(
      EntityType lhsType, QueryConstant rhsConstant) = 0;

  virtual std::list<PKBEntity> getLhsWildcardRhsSyn(EntityType rhsType) = 0;
  virtual bool hasLhsWildcardRhsWildcard() = 0;
  virtual bool hasLhsWildcardRhsConstant(QueryConstant rhsConstant) = 0;

  virtual std::list<PKBEntity> getLhsConstantRhsSyn(QueryConstant lhsConstant,
                                                    EntityType rhsType) = 0;
  virtual bool hasLhsConstantRhsWildcard(QueryConstant lhsConstant) = 0;
  virtual bool hasLhsConstantRhsConstant(QueryConstant lhsConstant,
                                         QueryConstant rhsConstant) = 0;
};
