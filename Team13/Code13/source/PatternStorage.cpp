#include "PatternStorage.h"

PatternStorage::PatternStorage() {}

void PatternStorage::storeFullExpAssignMap(int stmtNo,
	std::string lhs, std::string rhs) {
	PKBEntityPair pair = { { lhs, entityType }, { rhs, entityType } };
	mapStmtNumToPairs.insert({ stmtNo, pair });
}

void PatternStorage::storeSubExpsAssignMap(int stmtNo,
	std::string lhs, std::string rhs) {
	PKBEntityPairSet subExpRhs;
	PKBEntityPair pair = { { lhs, entityType }, { rhs, entityType } };
	mapStmtNumToSubExpPairs.insert({ stmtNo, subExpRhs });
	if (mapStmtNumToSubExpPairs.find(stmtNo) == mapStmtNumToSubExpPairs.end()) {
		PKBEntityPairSet rhsSet = { pair };
		mapStmtNumToSubExpPairs.insert({ stmtNo, rhsSet });
	} else {
		mapStmtNumToSubExpPairs[stmtNo].insert(pair);
	}
}


std::list<PKBEntityPair> PatternStorage::getPatternAssignLhsSynRhsWildcard() {
	std::list<PKBEntityPair> result;

	for (const auto& pair : this->mapStmtNumToPairs) {
		PKBEntity aSyn = { std::to_string(pair.first), entityType };
		result.emplace_back(PKBEntityPair({ aSyn, pair.second.lhs }));
	}
	return result;
}


std::list<PKBEntityPair> PatternStorage::getPatternAssignLhsSynRhsExp(
	std::string exp, bool isExactMatch) {
	if (isExactMatch) {
		return getLhsSynRhsExpExactMatch(exp);
	} else {
		return getLhsSynRhsExpNotExactMatch(exp);
	}
}

std::list<PKBEntityPair> PatternStorage::getLhsSynRhsExpExactMatch(
	std::string exp) {
	std::list<PKBEntityPair> result;

	for (const auto& pair : this->mapStmtNumToPairs) {
		if (pair.second.rhs.val == exp) {
			PKBEntity aSyn = { std::to_string(pair.first), entityType };
			result.emplace_back(PKBEntityPair({ aSyn, pair.second.lhs }));
		}
	}
	return result;
}

std::list<PKBEntityPair> PatternStorage::getLhsSynRhsExpNotExactMatch(
	std::string exp) {
	std::list<PKBEntityPair> result;

	for (const auto& pair : this->mapStmtNumToSubExpPairs) {
		PKBEntity toFind = { exp, entityType };
		for (const auto& subExp : pair.second) {
			if (toFind == subExp.rhs) {
				PKBEntity aSyn = { std::to_string(pair.first), entityType };
				result.emplace_back(PKBEntityPair({ aSyn, subExp.lhs }));
			}
		}
	}
	return result;
}


std::list<PKBEntity> PatternStorage::getPatternAssignLhsWildcardRhsWildcard() {
	std::list<PKBEntity> result;

	for (const auto& pair : this->mapStmtNumToPairs) {
		PKBEntity aSyn = { std::to_string(pair.first), entityType };
		result.emplace_back(aSyn);
	}
	return result;
}


std::list<PKBEntity> PatternStorage::getPatternAssignLhsWildcardRhsExp(
	std::string exp, bool isExactMatch) {
	std::list<PKBEntity> result;

	if (isExactMatch) {
		return getLhsWildcardRhsExpExactMatch(exp);
	} else {
		return getLhsWildcardRhsExpNotExactMatch(exp);
	}
}

std::list<PKBEntity> PatternStorage::getLhsWildcardRhsExpExactMatch(
	std::string exp) {
	std::list<PKBEntity> result;

	for (const auto& pair : this->mapStmtNumToPairs) {
		if (pair.second.rhs.val == exp) {
			PKBEntity aSyn = { std::to_string(pair.first), entityType };
			result.emplace_back(aSyn);
		}
	}
	return result;
}

std::list<PKBEntity> PatternStorage::getLhsWildcardRhsExpNotExactMatch(
	std::string exp) {
	std::list<PKBEntity> result;

	for (const auto& pair : this->mapStmtNumToSubExpPairs) {
		PKBEntity toFind = { exp, entityType };
		for (const auto& subExp : pair.second) {
			if (toFind == subExp.rhs) {
				PKBEntity aSyn = { std::to_string(pair.first), entityType };
				result.emplace_back(aSyn);
			}
		}
	}
	return result;
}


std::list<PKBEntity> PatternStorage::getPatternAssignLhsConstRhsWildcard(
	std::string lhsConst) {
	std::list<PKBEntity> result;

	for (const auto& pair : this->mapStmtNumToPairs) {
		if (pair.second.lhs.val == lhsConst) {
			PKBEntity aSyn = { std::to_string(pair.first), entityType };
			result.emplace_back(aSyn);
		}
	}
	return result;
}


std::list<PKBEntity> PatternStorage::getPatternAssignLhsConstRhsExp(
	std::string lhsConst, std::string rhsExp, bool isExactMatch) {
	if (isExactMatch) {
		return getLhsConstRhsExpExactMatch(lhsConst, rhsExp);
	} else {
		return getLhsConstRhsExpNotExactMatch(lhsConst, rhsExp);
	}
}

std::list<PKBEntity> PatternStorage::getLhsConstRhsExpExactMatch(
	std::string lhsConst, std::string rhsExp) {
	std::list<PKBEntity> result;

	for (const auto& pair : this->mapStmtNumToPairs) {
		PKBEntity lhs = pair.second.lhs;
		PKBEntity rhs = pair.second.rhs;
		if (lhs.val == lhsConst &&
			rhs.val == rhsExp) {
			PKBEntity aSyn = { std::to_string(pair.first), entityType };
			result.emplace_back(aSyn);
		}
	}
	return result;
}

std::list<PKBEntity> PatternStorage::getLhsConstRhsExpNotExactMatch(
	std::string lhsConst, std::string rhsExp) {
	std::list<PKBEntity> result;

	for (const auto& pair : this->mapStmtNumToSubExpPairs) {
		PKBEntity toFindRhs = { rhsExp, entityType };
		for (const auto& subExp : pair.second) {
			if (subExp.lhs.val == lhsConst &&
				toFindRhs == subExp.rhs) {
				PKBEntity aSyn = { std::to_string(pair.first), entityType };
				result.emplace_back(aSyn);
			}
		}
	}
	return result;
}


void PatternStorage::clear() {
	entityToStmtNoMap.clear();
	mapStmtNumToPairs.clear();
	mapStmtNumToSubExpPairs.clear();
}
