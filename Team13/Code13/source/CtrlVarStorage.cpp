#include <utility>
#include <string>
#include <list>

#include "CtrlVarStorage.h"

CtrlVarStorage::CtrlVarStorage() {}

void CtrlVarStorage::addControlVariable(int stmtNo, std::string variable) {
	CtrlVariableStmtList currStmtList;

	if (!(ctrlVariableToStmtMap.find(variable) == ctrlVariableToStmtMap.end())) {
		currStmtList = ctrlVariableToStmtMap[variable];
	}

	currStmtList.insert(stmtNo);
	ctrlVariableToStmtMap[variable] = currStmtList;
}

std::list<PKBEntity> CtrlVarStorage::getPatternWildcard() {
	std::list<PKBEntity> result;

	for (std::pair<std::string, CtrlVariableStmtList>
		pair : ctrlVariableToStmtMap) {
		for (int stmtNo : pair.second) {
			PKBEntity curr = { std::to_string(stmtNo), entityType };
			result.emplace_back(curr);
		}
	}
	return result;
}

std::list<PKBEntityPair> CtrlVarStorage::getPatternSyn() {
	std::list<PKBEntityPair> result;

	for (std::pair<std::string, CtrlVariableStmtList>
		pair : ctrlVariableToStmtMap) {
		for (int stmtNo : pair.second) {
			PKBEntity lhs = { std::to_string(stmtNo), entityType };
			PKBEntity rhs = { pair.first, entityType };
			result.emplace_back(PKBEntityPair({ lhs, rhs }));
		}
	}
	return result;
}

std::list<PKBEntity> CtrlVarStorage::getPatternConst(std::string constVal) {
	std::list<PKBEntity> result;

	for (int i : ctrlVariableToStmtMap[constVal]) {
		PKBEntity curr = { std::to_string(i), entityType };
		result.emplace_back(curr);
	}
	return result;
}

void CtrlVarStorage::clear() {
	ctrlVariableToStmtMap.clear();
}
