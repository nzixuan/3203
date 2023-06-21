#pragma once

#include <list>
#include <memory>
#include <string>

#include "PKB.h"
#include "Pattern.h"
#include "Table.h"

class PatternEvaluator {
  PKB* pkb;

  void evalOneSynAssignPattern(Pattern, Table*) const;
  void evalTwoSynAssignPattern(Pattern, Table*) const;
  void evalOneSynIfWhilePattern(Pattern, Table*) const;
  void evalTwoSynIfWhilePattern(Pattern, Table*) const;

 public:
  explicit PatternEvaluator(PKB*);
  void evalPatternClause(Pattern, Table*) const;
};
