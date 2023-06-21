#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <list>

#include "common.h"

struct PKBEntity {
  std::string val;
  EntityType entityType;

  friend bool operator==(const PKBEntity& lhs, const PKBEntity& rhs) {
    return lhs.val == rhs.val && lhs.entityType == rhs.entityType;
  }

  friend bool operator!=(const PKBEntity& lhs, const PKBEntity& rhs) {
    return !(lhs == rhs);
  }

  friend bool operator<(const PKBEntity& lhs, const PKBEntity& rhs) {
    return lhs.entityType < rhs.entityType;
  }

  friend bool operator>(const PKBEntity& lhs, const PKBEntity& rhs) {
    return rhs.entityType < lhs.entityType;
  }

  friend bool operator<=(const PKBEntity& lhs, const PKBEntity& rhs) {
    return !(lhs > rhs);
  }

  friend bool operator>=(const PKBEntity& lhs, const PKBEntity& rhs) {
    return !(lhs < rhs);
  }
};

struct PKBEntity_hash_fn {
  std::hash<std::string> stringHasher;

  std::size_t operator()(const PKBEntity& entity) const {
    std::size_t h1 = stringHasher(entity.val);
    std::size_t h2 = static_cast<std::size_t>(entity.entityType);

    return h1 ^ h2;
  }
};

struct PKBEntityPair {
  PKBEntity lhs;
  PKBEntity rhs;

  friend bool operator==(const PKBEntityPair& lhs, const PKBEntityPair& rhs) {
    return lhs.lhs == rhs.lhs && lhs.rhs == rhs.rhs;
  }

  friend bool operator!=(const PKBEntityPair& lhs, const PKBEntityPair& rhs) {
    return !(lhs == rhs);
  }
};

struct PKBEntityPair_hash_fn {
  std::size_t operator()(const PKBEntityPair& pair) const {
    std::size_t h1 = PKBEntity_hash_fn()(pair.lhs);
    std::size_t h2 = PKBEntity_hash_fn()(pair.rhs);

    return h1 ^ h2 & h1;
  }
};

struct QueryConstant {
  std::string val;
  QueryConstantType type;
};

typedef std::unordered_map<int, PKBEntity> StmtToEntityMap;
typedef std::unordered_set<PKBEntity, PKBEntity_hash_fn> PKBEntitySet;
typedef std::unordered_map<PKBEntity, PKBEntitySet, PKBEntity_hash_fn>
    PKBEntityMap;
typedef std::unordered_set<PKBEntityPair, PKBEntityPair_hash_fn>
    PKBEntityPairSet;

std::list<PKBEntity> convertPKBEntitySetToList(PKBEntitySet* set);
std::list<PKBEntityPair> convertPKBEntityPairSetToList(PKBEntityPairSet* set);
