#include "Query.h"

#include <set>

// Getters
std::vector<std::shared_ptr<Synonym>> Query::getSelectArgs() {
  return selectArgs;
}

bool Query::isBooleanSelect() { return isSelectingBoolean; }

ClauseManager* Query::getClauseManager() { return &clauseManager; }

// Setters

void Query::addSelectArg(std::shared_ptr<Synonym> arg) {
  selectArgs.push_back(arg);
}

void Query::addSelectArg(Synonym syn) {
  selectArgs.push_back(std::make_shared<Synonym>(syn));
}

void Query::addSelectArg(AttrRef ar) {
  selectArgs.push_back(std::make_shared<AttrRef>(ar));
}

void Query::finalize() {
  std::set<Synonym> selectSynSet;
  for (const auto& syn : selectArgs) {
    selectSynSet.insert(*syn);
  }
  clauseManager.finalize(selectSynSet);
}

void Query::setBooleanSelectFlag() { isSelectingBoolean = true; }
