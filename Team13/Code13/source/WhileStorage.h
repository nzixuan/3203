#pragma once

#include "CtrlVarStorage.h"

class WhileStorage : public CtrlVarStorage {
	using CtrlVarStorage::CtrlVarStorage;
	EntityType entityType = EntityType::While;
};
