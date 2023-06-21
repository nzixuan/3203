#include "Synonym.h"

Synonym::Synonym(std::string val, EntityType entType)
    : entityType(entType), RelArg(val, RelArgType::Synonym) {}

EntityType Synonym::getEntityType() { return entityType; }
