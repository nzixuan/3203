#pragma once
#include <unordered_set>

enum class EntityType {
  Procedure,
  Stmt,
  Read,
  Print,
  Assign,
  Call,
  While,
  If,
  Variable,
  Constant
};

struct EntityType_hash_fn {
  std::size_t operator()(const EntityType& type) const {
    return static_cast<std::size_t>(type);
  }
};

enum class RelRefType {
  Modifies,
  Uses,
  Follows,
  FollowsT,
  Parent,
  ParentT,
  Calls,
  CallsT,
  Next,
  NextT,
  Affects,
  AffectsT
};

struct RelRefType_hash_fn {
  std::size_t operator()(const RelRefType& type) const {
    return static_cast<std::size_t>(type);
  }
};

enum class QueryConstantType { Number, Identifier };

static const std::unordered_set<EntityType, EntityType_hash_fn> STMT_ENTITIES =
    {EntityType::Read,  EntityType::Print, EntityType::Assign, EntityType::Call,
     EntityType::While, EntityType::If,    EntityType::Stmt};
static const std::unordered_set<EntityType, EntityType_hash_fn>
    IDENTIFIER_ENTITIES = {EntityType::Procedure, EntityType::Variable,
                           EntityType::Constant};

bool isStmtEntityType(EntityType type);
