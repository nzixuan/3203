#pragma once

#include <string>

#include "Clause.h"
#include "common.h"

class Pattern : public Clause {
 private:
  Synonym patternSyn;

 public:
  Pattern::Pattern(ClauseArgs ca, Synonym patternSyn);
  Synonym getPatternSyn();

  friend bool operator==(const Pattern& lhs, const Pattern& rhs) {
    return ((Clause)lhs).getClauseArgs() == ((Clause)rhs).getClauseArgs() &&
           lhs.patternSyn == rhs.patternSyn;
  }
};
