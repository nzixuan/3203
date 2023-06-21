#include "ClauseManager.h"

void ClauseManager::addClause(SuchThat suchThat) {
  if (isFinalized) return;
  group.addClause(suchThat);
}

void ClauseManager::addClause(Pattern pattern) {
  if (isFinalized) return;
  group.addClause(pattern);
}

void ClauseManager::addClause(With with) {
  if (isFinalized) return;
  group.addClause(with);
}

// Utility functions
bool ClauseManager::hasNoClauses() { return clauses.empty(); }

Group ClauseManager::nextGroup() {
  Group group = clauses.front();
  clauses.pop_front();
  return group;
}

void ClauseManager::finalize(std::set<Synonym> selectArgs) {
  if (isFinalized) return;
  isFinalized = true;

  if (group.hasNoClauses()) return;

  clauses.push_back(group);
  queryOptimiser.optimise(&clauses, selectArgs);
}
