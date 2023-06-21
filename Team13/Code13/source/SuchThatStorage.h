#pragma once

#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "SuchThatQueriable.h"

class SuchThatStorage : public SuchThatQueriable {
 private:
  PKBEntitySet getSetFromIdentifier(std::string val, PKBEntityMap& map);
  PKBEntitySet getSetFromNumber(std::string val, PKBEntityMap& map);
  PKBEntitySet getSetFromQueryConstant(QueryConstant constant,
                                       PKBEntityMap& map);

  const PKBEntity* getEntityFromIdentifier(std::string val, PKBEntityMap& map);
  const PKBEntity* getEntityFromNumber(std::string val, PKBEntityMap& map);
  const PKBEntity* getEntityFromQueryConstant(QueryConstant constant,
                                              PKBEntityMap& map);

  std::list<PKBEntityPair> getLhsStmtRhsStmt();
  std::list<PKBEntityPair> getLhsStmtRhsSyn(EntityType rhsType);
  std::list<PKBEntityPair> getLhsSynRhsStmt(EntityType lhsType);

  std::list<PKBEntity> getLhsStmtRhsWildcard();
  std::list<PKBEntity> getLhsWildcardRhsStmt();

  std::list<PKBEntity> getLhsStmtRhsConstant(QueryConstant rhsConstant);
  std::list<PKBEntity> getLhsConstantRhsStmt(QueryConstant lhsConstant);

  std::list<PKBEntity> getLhsRhsSameStmtObj();

 protected:
  PKBEntityMap lhsToRhsSetMap;
  PKBEntityMap rhsToLhsSetMap;
  PKBEntityPairSet entityPairSet;
  void storeInMap(PKBEntity key, PKBEntity value, PKBEntityMap& map);
  void setRel(PKBEntity lhs, PKBEntity rhs);

 public:
  SuchThatStorage();
  ~SuchThatStorage();

  PKBEntityMap* getLhsMap();
  PKBEntityMap* getRhsMap();
  PKBEntityPairSet* getEntityPairSet();

  virtual void set(PKBEntity lhs, PKBEntity rhs) = 0;

  void clear();

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

class SuchThatBasicStorage : public SuchThatStorage {
  using SuchThatStorage::SuchThatStorage;

 public:
  void set(PKBEntity lhs, PKBEntity rhs) override;
};

class SuchThatTStorage : public SuchThatStorage {
  using SuchThatStorage::SuchThatStorage;

 protected:
  PKBEntitySet setTransitivesWithLhsEntity(PKBEntity lhs, PKBEntity rhs);
  PKBEntitySet setTransitivesWithRhsEntity(PKBEntity lhs, PKBEntity rhs);

 public:
  void set(PKBEntity lhs, PKBEntity rhs) override;
};
