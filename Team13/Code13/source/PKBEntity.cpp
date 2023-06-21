#include "PKBEntity.h"

std::list<PKBEntity> convertPKBEntitySetToList(PKBEntitySet* set) {
  std::list<PKBEntity> resultList;

  for (auto it = set->begin(); it != set->end();) {
    resultList.push_back(std::move(set->extract(it++).value()));
  }

  return resultList;
}

std::list<PKBEntityPair> convertPKBEntityPairSetToList(PKBEntityPairSet* set) {
  std::list<PKBEntityPair> resultList;

  for (auto it = set->begin(); it != set->end();) {
    resultList.push_back(std::move(set->extract(it++).value()));
  }

  return resultList;
}
