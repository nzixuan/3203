#pragma once

#include "CtrlVarStorage.h"

class IfStorage : public CtrlVarStorage {
	using CtrlVarStorage::CtrlVarStorage;
	EntityType entityType = EntityType::If;
};
