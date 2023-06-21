#pragma once

#include "Clause.h"

class SuchThat : public Clause {
 private:
  RelRefType relationship;

 public:
  SuchThat(ClauseArgs ca, RelRefType relationship);
  RelRefType getRelationship();

  friend bool operator==(const SuchThat& lhs, const SuchThat& rhs) {
    return lhs.relationship == rhs.relationship &&
           ((Clause)lhs).getClauseArgs() == ((Clause)rhs).getClauseArgs();
  }
};
