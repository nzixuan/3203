#include <list>
#include <string>
#include <unordered_map>

#include "SuchThatQuerier.h";

/*
 * Cache functions
 */

void SuchThatQuerier::addToCache(
    std::string keyVal, PKBEntity entityVal,
    std::unordered_map<std::string, PKBEntitySet>& cache) {
  const auto itr = cache.find(keyVal);

  if (itr == cache.end()) {
    PKBEntitySet newSet;
    newSet.insert(entityVal);
    cache.insert({keyVal, newSet});
  } else {
    PKBEntitySet currSet = itr->second;
    currSet.insert(entityVal);
    itr->second = currSet;
  }
}

bool SuchThatQuerier::hasEntitiesFromCache(
    std::string keyVal, std::unordered_map<std::string, PKBEntitySet>& cache) {
  const auto itr = cache.find(keyVal);
  return itr != cache.end();
}

std::list<PKBEntity> SuchThatQuerier::getEntitiesFromCache(
    std::string keyVal, EntityType type,
    std::unordered_map<std::string, PKBEntitySet>& cache) {
  std::list<PKBEntity> results;
  const auto itr = cache.find(keyVal);

  if (itr == cache.end()) {
    return results;
  } else {
    PKBEntitySet currSet = itr->second;
    for (const PKBEntity& entity : currSet) {
      if ((type == EntityType::Stmt && isStmtEntityType(entity.entityType)) ||
          entity.entityType == type) {
        results.emplace_back(entity);
      }
    }
  }

  return results;
}

bool SuchThatQuerier::hasEntityFromCache(
    std::string keyVal, QueryConstant constant,
    std::unordered_map<std::string, PKBEntitySet>& cache) {
  const auto itr = cache.find(keyVal);

  if (itr != cache.end()) {
    PKBEntitySet currSet = itr->second;
    for (const PKBEntity& entity : currSet) {
      if (entity.val == constant.val) {
        return true;
      }
    }
  }
  return false;
}

void SuchThatQuerier::clearCache() {
  this->lhsToRhsMapCache.clear();
  this->rhsToLhsMapCache.clear();
}
