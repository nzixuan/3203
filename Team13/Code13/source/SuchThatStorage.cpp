#include "SuchThatStorage.h"

#include <stdexcept>
#include <utility>

SuchThatStorage::SuchThatStorage() {
  PKBEntityMap lhsToRhsSetMap;
  PKBEntityMap rhsToLhsListMap;
  PKBEntityPairSet entityPairSet;
}

SuchThatStorage::~SuchThatStorage() {}

PKBEntityMap* SuchThatStorage::getLhsMap() { return &(this->lhsToRhsSetMap); }
PKBEntityMap* SuchThatStorage::getRhsMap() { return &(this->rhsToLhsSetMap); }

PKBEntityPairSet* SuchThatStorage::getEntityPairSet() {
  return &(this->entityPairSet);
}

void SuchThatStorage::clear() {
  this->getLhsMap()->clear();
  this->getRhsMap()->clear();
  this->getEntityPairSet()->clear();
}

void SuchThatStorage::storeInMap(PKBEntity key, PKBEntity value,
                                 PKBEntityMap& map) {
  const auto itr = map.find(key);

  if (itr == map.end()) {
    PKBEntitySet newSet;
    newSet.insert(value);
    map.insert({key, newSet});
  } else {
    PKBEntitySet currSet = itr->second;
    currSet.insert(value);
    itr->second = currSet;
  }
}

void SuchThatStorage::setRel(PKBEntity lhs, PKBEntity rhs) {
  this->storeInMap(lhs, rhs, (this->lhsToRhsSetMap));

  this->storeInMap(rhs, lhs, (this->rhsToLhsSetMap));

  this->entityPairSet.insert({lhs, rhs});
}

void SuchThatBasicStorage::set(PKBEntity lhs, PKBEntity rhs) {
  SuchThatStorage::setRel(lhs, rhs);
}

PKBEntitySet SuchThatTStorage::setTransitivesWithLhsEntity(PKBEntity lhs,
                                                           PKBEntity rhs) {
  PKBEntitySet setOfNewRhs;

  auto lhsMapItr = lhsToRhsSetMap.find(rhs);

  if (lhsMapItr != lhsToRhsSetMap.end()) {
    const PKBEntitySet currSet = lhsMapItr->second;
    for (const PKBEntity& entity : currSet) {
      // Add transitive rel to storage
      SuchThatStorage::setRel(lhs, entity);
      setOfNewRhs.insert(entity);
    }
  }

  return setOfNewRhs;
}

PKBEntitySet SuchThatTStorage::setTransitivesWithRhsEntity(PKBEntity lhs,
                                                           PKBEntity rhs) {
  PKBEntitySet setOfNewLhs;

  auto rhsMapItr = rhsToLhsSetMap.find(lhs);

  if (rhsMapItr != rhsToLhsSetMap.end()) {
    const PKBEntitySet currSet = rhsMapItr->second;
    for (const PKBEntity& entity : currSet) {
      // Add transitive rel to storage
      SuchThatStorage::setRel(entity, rhs);
      setOfNewLhs.insert(entity);
    }
  }
  return setOfNewLhs;
}

void SuchThatTStorage::set(PKBEntity lhs, PKBEntity rhs) {
  SuchThatStorage::setRel(lhs, rhs);

  // Check if rhs is on the lhs of any other pair stored previously
  // and creates a transitive with the current lhs and other rhs
  PKBEntitySet setOfNewRhs =
      SuchThatTStorage::setTransitivesWithLhsEntity(lhs, rhs);

  // Check if lhs is on the rhs of any other pair stored previously
  // and creates a transitive with the other lhs and current rhs
  PKBEntitySet setOfNewLhs =
      SuchThatTStorage::setTransitivesWithRhsEntity(lhs, rhs);

  for (const PKBEntity& newLhs : setOfNewLhs) {
    for (const PKBEntity& newRhs : setOfNewRhs) {
      // Add transitive rel to storage
      SuchThatStorage::setRel(newLhs, newRhs);
    }
  }
}

PKBEntitySet SuchThatStorage::getSetFromIdentifier(std::string val,
                                                   PKBEntityMap& map) {
  for (const EntityType& identType : IDENTIFIER_ENTITIES) {
    const auto iter = map.find({val, identType});
    if (iter != map.end()) {
      return iter->second;
    }
  }

  PKBEntitySet emptySet;

  return emptySet;
}

PKBEntitySet SuchThatStorage::getSetFromNumber(std::string val,
                                               PKBEntityMap& map) {
  for (const EntityType& numType : STMT_ENTITIES) {
    const auto iter = map.find({val, numType});
    if (iter != map.end()) {
      return iter->second;
    }
  }

  PKBEntitySet emptySet;
  return emptySet;
}

PKBEntitySet SuchThatStorage::getSetFromQueryConstant(QueryConstant constant,
                                                      PKBEntityMap& map) {
  if (constant.type == QueryConstantType::Identifier) {
    return getSetFromIdentifier(constant.val, map);
  } else if (constant.type == QueryConstantType::Number) {
    return getSetFromNumber(constant.val, map);
  } else {
    throw std::logic_error("Unknown query constant type found " + constant.val);
  }
}

const PKBEntity* SuchThatStorage::getEntityFromIdentifier(std::string val,
                                                          PKBEntityMap& map) {
  for (const EntityType& identType : IDENTIFIER_ENTITIES) {
    const auto iter = map.find({val, identType});
    if (iter != map.end()) {
      const PKBEntity* ptr = &(iter->first);
      return ptr;
    }
  }

  return NULL;
}

const PKBEntity* SuchThatStorage::getEntityFromNumber(std::string val,
                                                      PKBEntityMap& map) {
  for (const EntityType& numType : STMT_ENTITIES) {
    const auto iter = map.find({val, numType});
    if (iter != map.end()) {
      const PKBEntity* ptr = &(iter->first);
      return ptr;
    }
  }
  return NULL;
}

const PKBEntity* SuchThatStorage::getEntityFromQueryConstant(
    QueryConstant constant, PKBEntityMap& map) {
  if (constant.type == QueryConstantType::Identifier) {
    return getEntityFromIdentifier(constant.val, map);
  } else if (constant.type == QueryConstantType::Number) {
    return getEntityFromNumber(constant.val, map);
  } else {
    throw std::logic_error("Unknown query constant type found " + constant.val);
  }
}

std::list<PKBEntityPair> SuchThatStorage::getLhsStmtRhsStmt() {
  std::list<PKBEntityPair> results;

  for (const auto& pair : this->entityPairSet) {
    if (isStmtEntityType(pair.lhs.entityType) &&
        isStmtEntityType(pair.rhs.entityType)) {
      results.emplace_back(pair);
    }
  }
  return results;
}
std::list<PKBEntityPair> SuchThatStorage::getLhsStmtRhsSyn(EntityType rhsType) {
  std::list<PKBEntityPair> results;
  for (const auto& pair : this->entityPairSet) {
    if (isStmtEntityType(pair.lhs.entityType) &&
        pair.rhs.entityType == rhsType) {
      results.emplace_back(pair);
    }
  }
  return results;
}

std::list<PKBEntityPair> SuchThatStorage::getLhsSynRhsStmt(EntityType lhsType) {
  std::list<PKBEntityPair> results;
  for (const auto& pair : this->entityPairSet) {
    if (pair.lhs.entityType == lhsType &&
        isStmtEntityType(pair.rhs.entityType)) {
      results.emplace_back(pair);
    }
  }
  return results;
}

std::list<PKBEntity> SuchThatStorage::getLhsStmtRhsWildcard() {
  PKBEntitySet resultSet;

  for (const auto& pair : this->entityPairSet) {
    if (isStmtEntityType(pair.lhs.entityType)) {
      resultSet.insert(pair.lhs);
    }
  }
  std::list<PKBEntity> results;

  for (auto it = resultSet.begin(); it != resultSet.end();) {
    results.push_back(std::move(resultSet.extract(it++).value()));
  }

  return results;
}

std::list<PKBEntity> SuchThatStorage::getLhsWildcardRhsStmt() {
  PKBEntitySet resultSet;

  for (const auto& pair : this->entityPairSet) {
    if (isStmtEntityType(pair.rhs.entityType)) {
      resultSet.insert(pair.rhs);
    }
  }
  std::list<PKBEntity> results;

  for (auto it = resultSet.begin(); it != resultSet.end();) {
    results.push_back(std::move(resultSet.extract(it++).value()));
  }

  return results;
}

std::list<PKBEntity> SuchThatStorage::getLhsStmtRhsConstant(
    QueryConstant rhsConstant) {
  std::list<PKBEntity> results;

  const PKBEntitySet resultSet =
      getSetFromQueryConstant(rhsConstant, rhsToLhsSetMap);

  for (PKBEntity const& lhs : resultSet) {
    if (isStmtEntityType(lhs.entityType)) {
      results.emplace_back(lhs);
    }
  }

  return results;
}

std::list<PKBEntity> SuchThatStorage::getLhsConstantRhsStmt(
    QueryConstant lhsConstant) {
  std::list<PKBEntity> results;

  const PKBEntitySet resultSet =
      getSetFromQueryConstant(lhsConstant, lhsToRhsSetMap);

  for (PKBEntity const& rhs : resultSet) {
    if (isStmtEntityType(rhs.entityType)) {
      results.emplace_back(rhs);
    }
  }

  return results;
}

std::list<PKBEntity> SuchThatStorage::getLhsRhsSameStmtObj() {
  std::list<PKBEntity> results;
  for (const auto& pair : this->entityPairSet) {
    if (isStmtEntityType(pair.lhs.entityType) && pair.lhs == pair.rhs) {
      results.emplace_back(pair.lhs);
    }
  }
  return results;
}

/*
 * Public API Getters for SuchThat Storages below
 */
std::list<PKBEntityPair> SuchThatStorage::getLhsSynRhsSyn(EntityType lhsType,
                                                          EntityType rhsType) {
  // Checks if any of the types provided are EntityType::Stmt
  if (lhsType == EntityType::Stmt && rhsType == EntityType::Stmt) {
    return getLhsStmtRhsStmt();
  } else if (lhsType == EntityType::Stmt) {
    return getLhsStmtRhsSyn(rhsType);
  } else if (rhsType == EntityType::Stmt) {
    return getLhsSynRhsStmt(lhsType);
  }

  std::list<PKBEntityPair> results;

  for (const auto& pair : this->entityPairSet) {
    if (pair.lhs.entityType == lhsType && pair.rhs.entityType == rhsType) {
      results.emplace_back(pair);
    }
  }

  return results;
}

std::list<PKBEntity> SuchThatStorage::getLhsRhsSameSynObj(EntityType type) {
  if (type == EntityType::Stmt) {
    return getLhsRhsSameStmtObj();
  }

  std::list<PKBEntity> results;

  for (const auto& pair : this->entityPairSet) {
    if (pair.lhs.entityType == type && pair.lhs == pair.rhs) {
      results.emplace_back(pair.lhs);
    }
  }

  return results;
}

std::list<PKBEntity> SuchThatStorage::getLhsSynRhsWildcard(EntityType lhsType) {
  // Checks if the type provided is EntityType::Stmt
  if (lhsType == EntityType::Stmt) {
    return getLhsStmtRhsWildcard();
  }

  PKBEntitySet resultSet;

  for (const auto& pair : this->entityPairSet) {
    if (pair.lhs.entityType == lhsType) {
      resultSet.insert(pair.lhs);
    }
  }
  std::list<PKBEntity> results;

  for (auto it = resultSet.begin(); it != resultSet.end();) {
    results.push_back(std::move(resultSet.extract(it++).value()));
  }

  return results;
}

std::list<PKBEntity> SuchThatStorage::getLhsSynRhsConstant(
    EntityType lhsType, QueryConstant rhsConstant) {
  // Checks if the type provided is EntityType::Stmt
  if (lhsType == EntityType::Stmt) {
    return getLhsStmtRhsConstant(rhsConstant);
  }

  std::list<PKBEntity> results;

  const PKBEntitySet resultSet =
      getSetFromQueryConstant(rhsConstant, rhsToLhsSetMap);

  for (PKBEntity const& lhs : resultSet) {
    if (lhs.entityType == lhsType) {
      results.emplace_back(lhs);
    }
  }

  return results;
}

std::list<PKBEntity> SuchThatStorage::getLhsWildcardRhsSyn(EntityType rhsType) {
  // Checks if the type provided is EntityType::Stmt
  if (rhsType == EntityType::Stmt) {
    return getLhsWildcardRhsStmt();
  }

  PKBEntitySet resultSet;

  for (const auto& pair : this->entityPairSet) {
    if (pair.rhs.entityType == rhsType) {
      resultSet.insert(pair.rhs);
    }
  }
  std::list<PKBEntity> results;

  for (auto it = resultSet.begin(); it != resultSet.end();) {
    results.push_back(std::move(resultSet.extract(it++).value()));
  }

  return results;
}

bool SuchThatStorage::hasLhsWildcardRhsWildcard() {
  return entityPairSet.size() > 0;
}
bool SuchThatStorage::hasLhsWildcardRhsConstant(QueryConstant rhsConstant) {
  const PKBEntitySet lhsSet =
      getSetFromQueryConstant(rhsConstant, rhsToLhsSetMap);
  return (lhsSet.size() > 0);
}

std::list<PKBEntity> SuchThatStorage::getLhsConstantRhsSyn(
    QueryConstant lhsConstant, EntityType rhsType) {
  // Checks if the type provided is EntityType::Stmt
  if (rhsType == EntityType::Stmt) {
    return getLhsConstantRhsStmt(lhsConstant);
  }

  std::list<PKBEntity> results;

  const PKBEntitySet resultSet =
      getSetFromQueryConstant(lhsConstant, lhsToRhsSetMap);

  for (PKBEntity const& rhs : resultSet) {
    if (rhs.entityType == rhsType) {
      results.emplace_back(rhs);
    }
  }

  return results;
}
bool SuchThatStorage::hasLhsConstantRhsWildcard(QueryConstant lhsConstant) {
  const PKBEntitySet rhsSet =
      getSetFromQueryConstant(lhsConstant, lhsToRhsSetMap);
  return (rhsSet.size() > 0);
}

bool SuchThatStorage::hasLhsConstantRhsConstant(QueryConstant lhsConstant,
                                                QueryConstant rhsConstant) {
  const PKBEntitySet rhsSet =
      getSetFromQueryConstant(lhsConstant, lhsToRhsSetMap);

  const PKBEntity* lhs =
      getEntityFromQueryConstant(rhsConstant, rhsToLhsSetMap);
  if (rhsSet.size() == 0 || !lhs) {
    return false;
  }

  const auto rhsMapIter = rhsSet.find(*lhs);
  return rhsMapIter != rhsSet.end();
}
