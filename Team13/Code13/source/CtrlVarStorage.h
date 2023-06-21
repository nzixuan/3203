#pragma once

#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "PKBEntity.h"

typedef std::unordered_set<int> CtrlVariableStmtList;
typedef std::unordered_map<std::string, CtrlVariableStmtList>
	CtrlVariableToStmtMap;

class CtrlVarStorage {
 private:
	// Control Variable to Statement Map to store If and While variables
	CtrlVariableToStmtMap ctrlVariableToStmtMap;
	EntityType entityType = EntityType::Stmt;

 public:
	CtrlVarStorage();

	void addControlVariable(int stmtNo, std::string variable);

	// 1. if(_, _, _) w(_, _, _)
	// EntityType is "if" or "while"
	// Returns list<PKBEntity> : possible values for synonym
	std::list<PKBEntity> getPatternWildcard();

	// 2. if(s, _) w(s, _)
	// EntityType is "if" or "while"
	// Returns list<PKBEntityPair> : lhs possible values for synonym,
	// rhs possible values for variable synonym
	std::list<PKBEntityPair> getPatternSyn();

	// 3. if ('s', _) w('s', _)
	// EntityType is "if" or "while"
	// Returns list<PKBEntity> : possible values for synonym
	std::list<PKBEntity> getPatternConst(std::string constVal);

	void clear();
};
