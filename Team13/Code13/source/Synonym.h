#pragma once
#include <string>

#include "RelArg.h"

class Synonym : public RelArg {
 private:
  EntityType entityType;

 public:
  Synonym(std::string val, EntityType entType);

  EntityType getEntityType();

  friend bool operator==(const Synonym& lhs, const Synonym& rhs) {
    return lhs.entityType == rhs.entityType && (RelArg)lhs == (RelArg)rhs;
  }
};
