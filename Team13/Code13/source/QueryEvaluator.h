#pragma once

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "PKB.h"
#include "PatternEvaluator.h"
#include "Query.h"
#include "SuchThatEvaluator.h"
#include "Table.h"
#include "WithEvaluator.h"

class QueryEvaluator {
 private:
  Query query;
  PKB* pkb;
  std::list<std::string>& results;

  bool evaluateGroup(Group, Table*, SuchThatEvaluator&, PatternEvaluator&,
                     WithEvaluator&) const;
  std::list<std::string> getSelectStringVals();
  void updateTableWithSelectVals(Table*);
  std::list<PKBEntity> getAllOfType(EntityType) const;
  void populateResults(Table*, bool);

 public:
  QueryEvaluator(Query, PKB*, std::list<std::string>&);
  void evaluate();
};
