#pragma once
#include <list>
#include <string>
#include <unordered_map>

#include "SuchThatQueriable.h"

class SuchThatQuerier : public SuchThatQueriable {
 protected:
  std::unordered_map<std::string, PKBEntitySet> lhsToRhsMapCache;
  std::unordered_map<std::string, PKBEntitySet> rhsToLhsMapCache;
  void addToCache(std::string keyVal, PKBEntity entityVal,
                  std::unordered_map<std::string, PKBEntitySet>& cache);

  bool hasEntitiesFromCache(
      std::string keyVal, std::unordered_map<std::string, PKBEntitySet>& cache);
  bool hasEntityFromCache(std::string keyVal, QueryConstant constant,
                          std::unordered_map<std::string, PKBEntitySet>& cache);
  std::list<PKBEntity> getEntitiesFromCache(
      std::string keyVal, EntityType type,
      std::unordered_map<std::string, PKBEntitySet>& cache);

 public:
  void clearCache();
};
