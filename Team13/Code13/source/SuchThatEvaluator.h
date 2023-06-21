#pragma once

#include <list>
#include <memory>
#include <string>

#include "PKB.h"
#include "Query.h"
#include "SuchThat.h"
#include "Table.h"

class SuchThatEvaluator {
  Query query;
  PKB* pkb;

  bool evalBoolSuchThat(SuchThat);
  std::list<PKBEntity> evalOneSynSuchThat(SuchThat);
  std::list<PKBEntityPair> evalTwoSynSuchThat(SuchThat);

 public:
  SuchThatEvaluator(Query, PKB*);
  bool evalSuchThatClause(SuchThat, Table*);
};
