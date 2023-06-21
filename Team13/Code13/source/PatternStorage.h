#pragma once

#include <utility>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "PKBEntity.h"


typedef std::unordered_set<int> StmtNoSet;
typedef std::unordered_set<PKBEntityPair, PKBEntityPair_hash_fn>
	PKBEntityPairSet;
typedef std::pair<std::string, std::string> FullExpPair;
typedef std::pair<std::string, std::unordered_set<std::string>> SubExpPair;
typedef std::unordered_map<EntityType, StmtNoSet, EntityType_hash_fn>
	PatternEntityStmtMap;
typedef std::unordered_map<int, PKBEntityPair> MapStmtNumToPairs;
typedef std::unordered_map<int, PKBEntityPairSet> MapStmtNumToSubExpPairs;



class PatternStorage {
 private:
	// Entity to Stmt No.
	PatternEntityStmtMap entityToStmtNoMap;

	// Assign Map Full Exp
	MapStmtNumToPairs mapStmtNumToPairs;

	// Assign Map Sub Exp
	MapStmtNumToSubExpPairs mapStmtNumToSubExpPairs;

	EntityType entityType = EntityType::Stmt;

	std::list<PKBEntityPair> getLhsSynRhsExpExactMatch(std::string exp);

	std::list<PKBEntityPair> getLhsSynRhsExpNotExactMatch(std::string exp);

	std::list<PKBEntity> getLhsWildcardRhsExpExactMatch(std::string exp);

	std::list<PKBEntity> getLhsWildcardRhsExpNotExactMatch(std::string exp);

	std::list<PKBEntity> getLhsConstRhsExpExactMatch(std::string lhsConst,
		std::string rhsExp);

	std::list<PKBEntity> getLhsConstRhsExpNotExactMatch(std::string lhsConst,
		std::string rhsExp);

 public:
	PatternStorage();

	void storeFullExpAssignMap(int stmtNo, std::string lhs, std::string rhs);

	void storeSubExpsAssignMap(int stmtNo, std::string lhs, std::string rhs);

	void clear();

	// LHS variable synonym, RHS wildcard
	// Returns list<PKBEntityPair>: lhs possible values for assign synonym,
	//                              rhs possible values for variable synonym
	std::list<PKBEntityPair> getPatternAssignLhsSynRhsWildcard();

	// LHS variable synonym, RHS expression
	// Returns list<PKBEntityPair>: lhs possible values for assign synonym,
	//                              rhs possible values for variable synonym
	std::list<PKBEntityPair> getPatternAssignLhsSynRhsExp(std::string exp,
		bool isExactMatch);

	// LHS wildcard, RHS wildcard
	// Returns list<PKBEntity>: possible values for assign synonym
	std::list<PKBEntity> getPatternAssignLhsWildcardRhsWildcard();

	// LHS wildcard, RHS expression
	// Returns list<PKBEntity>: possible values for assign synonym
	std::list<PKBEntity> getPatternAssignLhsWildcardRhsExp(std::string exp,
		bool isExactMatch);

	// LHS const, RHS wildcard
	// Returns list<PKBEntity>: possible values for assign synonym
	std::list<PKBEntity> getPatternAssignLhsConstRhsWildcard(std::string lhsConst);

	// LHS const, RHS expression
	// Returns list<PKBEntity>: possible values for rhsExp
	std::list<PKBEntity> getPatternAssignLhsConstRhsExp(std::string lhsConst,
		std::string rhsExp, bool isExactMatch);
};
