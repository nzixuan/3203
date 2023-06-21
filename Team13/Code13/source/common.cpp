#include "common.h"

bool isStmtEntityType(EntityType type) {
  const auto iter = STMT_ENTITIES.find(type);
  return iter != STMT_ENTITIES.end();
}
