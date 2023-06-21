#include <list>

#include "SuchThatTQuerier.h"

SuchThatTQuerier::SuchThatTQuerier() { this->queriable = NULL; }

SuchThatTQuerier::SuchThatTQuerier(SuchThatQueriable* queriable) {
  this->queriable = queriable;
}

SuchThatTQuerier::~SuchThatTQuerier() {}

void SuchThatTQuerier::setStorage(SuchThatQueriable* queriable) {
  this->queriable = queriable;
}

/*
 * Public API Getters for SuchThatTQueriers Storages below
 */

std::list<PKBEntityPair> SuchThatTQuerier::getLhsSynRhsSyn(EntityType lhsType,
                                                           EntityType rhsType) {
  PKBEntityPairSet results;

  std::list<PKBEntity> listOfLhsWithType =
      queriable->getLhsSynRhsWildcard(lhsType);

  for (PKBEntity& lhsEntity : listOfLhsWithType) {
    std::list<PKBEntity> listOfRhsWithType = getLhsConstantRhsSyn(
        {lhsEntity.val, QueryConstantType::Number}, rhsType);

    for (PKBEntity& rhsEntity : listOfRhsWithType) {
      results.insert({lhsEntity, rhsEntity});
    }
  }

  return convertPKBEntityPairSetToList(&results);
}

std::list<PKBEntity> SuchThatTQuerier::getLhsRhsSameSynObj(EntityType type) {
  PKBEntitySet results;

  std::list<PKBEntity> listOfLhsWithType =
      queriable->getLhsSynRhsWildcard(type);
  for (PKBEntity& entity : listOfLhsWithType) {
    QueryConstant entityConstant = {entity.val, QueryConstantType::Number};
    if (hasLhsConstantRhsConstant(entityConstant, entityConstant)) {
      results.insert(entity);
    }
  }
  return convertPKBEntitySetToList(&results);
}

std::list<PKBEntity> SuchThatTQuerier::getLhsSynRhsWildcard(
    EntityType lhsType) {
  return queriable->getLhsSynRhsWildcard(lhsType);
}

std::list<PKBEntity> SuchThatTQuerier::getLhsSynRhsConstant(
    EntityType lhsType, QueryConstant rhsConstant) {
  // Checks cache for results
  if (hasEntitiesFromCache(rhsConstant.val, rhsToLhsMapCache)) {
    return getEntitiesFromCache(rhsConstant.val, lhsType, rhsToLhsMapCache);
  }

  // Get all the lhs for rhsConstant regardless of lhs type
  std::list<PKBEntity> listOfLhs =
      queriable->getLhsSynRhsConstant(EntityType::Stmt, rhsConstant);
  PKBEntitySet results;

  PKBEntitySet visitedLhsSet;

  for (PKBEntity& entity : listOfLhs) {
    visitedLhsSet.insert(entity);
  }

  while (!listOfLhs.empty()) {
    PKBEntity currEntity = listOfLhs.front();

    // Cache any result regardless of entity type
    addToCache(rhsConstant.val, currEntity, rhsToLhsMapCache);

    if (lhsType == EntityType::Stmt || currEntity.entityType == lhsType) {
      results.insert(currEntity);
    }

    std::list<PKBEntity> newLhsEntities = queriable->getLhsSynRhsConstant(
        EntityType::Stmt, {currEntity.val, QueryConstantType::Number});
    for (PKBEntity& entity : newLhsEntities) {
      auto isInserted = visitedLhsSet.insert(entity);
      if (isInserted.second) {
        listOfLhs.emplace_back(entity);
      }
    }
    listOfLhs.pop_front();
  }

  return convertPKBEntitySetToList(&results);
}

std::list<PKBEntity> SuchThatTQuerier::getLhsWildcardRhsSyn(
    EntityType rhsType) {
  return queriable->getLhsWildcardRhsSyn(rhsType);
}

bool SuchThatTQuerier::hasLhsWildcardRhsWildcard() {
  return queriable->hasLhsWildcardRhsWildcard();
}

bool SuchThatTQuerier::hasLhsWildcardRhsConstant(QueryConstant rhsConstant) {
  return queriable->hasLhsWildcardRhsConstant(rhsConstant);
}

std::list<PKBEntity> SuchThatTQuerier::getLhsConstantRhsSyn(
    QueryConstant lhsConstant, EntityType rhsType) {
  // Checks cache for results
  if (hasEntitiesFromCache(lhsConstant.val, lhsToRhsMapCache)) {
    return getEntitiesFromCache(lhsConstant.val, rhsType, lhsToRhsMapCache);
  }

  // Get all the rhs for lhsConstant regardless of rhs type
  std::list<PKBEntity> listOfRhs =
      queriable->getLhsConstantRhsSyn(lhsConstant, EntityType::Stmt);
  PKBEntitySet results;

  PKBEntitySet visitedRhsSet;

  for (PKBEntity& entity : listOfRhs) {
    visitedRhsSet.insert(entity);
  }

  while (!listOfRhs.empty()) {
    PKBEntity currEntity = listOfRhs.front();

    // Cache any result regardless of entity type
    addToCache(lhsConstant.val, currEntity, lhsToRhsMapCache);

    if (rhsType == EntityType::Stmt || currEntity.entityType == rhsType) {
      results.insert(currEntity);
    }

    std::list<PKBEntity> newRhsEntities = queriable->getLhsConstantRhsSyn(
        {currEntity.val, QueryConstantType::Number}, EntityType::Stmt);
    for (PKBEntity& entity : newRhsEntities) {
      auto isInserted = visitedRhsSet.insert(entity);
      if (isInserted.second) {
        listOfRhs.emplace_back(entity);
      }
    }
    listOfRhs.pop_front();
  }

  return convertPKBEntitySetToList(&results);
}

bool SuchThatTQuerier::hasLhsConstantRhsWildcard(QueryConstant lhsConstant) {
  return queriable->hasLhsConstantRhsWildcard(lhsConstant);
}

bool SuchThatTQuerier::hasLhsConstantRhsConstant(QueryConstant lhsConstant,
                                                 QueryConstant rhsConstant) {
  // Checks either cache for results
  if (hasEntityFromCache(lhsConstant.val, rhsConstant, lhsToRhsMapCache) ||
      hasEntityFromCache(rhsConstant.val, lhsConstant, rhsToLhsMapCache)) {
    return true;
  }

  if (queriable->hasLhsConstantRhsConstant(lhsConstant, rhsConstant)) {
    return true;
  } else if (!queriable->hasLhsConstantRhsWildcard(lhsConstant) ||
             !queriable->hasLhsWildcardRhsConstant(rhsConstant)) {
    return false;
  }

  std::list<PKBEntity> listOfRhs =
      queriable->getLhsConstantRhsSyn(lhsConstant, EntityType::Stmt);

  PKBEntitySet results;

  PKBEntitySet visitedRhsSet;

  for (PKBEntity& entity : listOfRhs) {
    visitedRhsSet.insert(entity);
  }

  while (!listOfRhs.empty()) {
    PKBEntity currEntity = listOfRhs.front();

    if (currEntity.val == rhsConstant.val) {
      return true;
    }

    std::list<PKBEntity> newRhsEntities = queriable->getLhsConstantRhsSyn(
        {currEntity.val, QueryConstantType::Number}, EntityType::Stmt);
    for (PKBEntity& entity : newRhsEntities) {
      auto isInserted = visitedRhsSet.insert(entity);
      if (isInserted.second) {
        listOfRhs.emplace_back(entity);
      }
    }
    listOfRhs.pop_front();
  }
  return false;
}
