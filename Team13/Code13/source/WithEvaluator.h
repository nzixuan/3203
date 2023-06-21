#pragma once

#include <list>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "PKB.h"
#include "Table.h"
#include "With.h"

class WithEvaluator {
  PKB* pkb;

  static bool evalBoolWith(With);
  bool evalOneSynWith(With, Table*) const;
  void evalTwoSynWith(With, Table*) const;

  static bool isSpecial(const std::shared_ptr<AttrRef>&);
  std::vector<std::pair<std::string, PKBEntity>> convertAttrRef(
      const std::shared_ptr<AttrRef>&, Table*) const;
  std::list<PKBEntity> getAllOfType(EntityType) const;

 public:
  explicit WithEvaluator(PKB*);
  bool evalWithClause(With, Table*);
};
