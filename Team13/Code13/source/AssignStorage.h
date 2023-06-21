#pragma once

#include "PatternStorage.h"

class AssignStorage : public PatternStorage {
	using PatternStorage::PatternStorage;
	EntityType entityType = EntityType::Assign;
};
