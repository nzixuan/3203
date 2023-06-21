#pragma once

#include "Clause.h"

class With : public Clause {
 private:
  WithRefType wrType;

 public:
  With(ClauseArgs ca, WithRefType wrt);

  WithRefType getWithRefType();

  friend bool operator==(const With& lhs, const With& rhs) {
    return ((Clause)lhs).getClauseArgs() == ((Clause)rhs).getClauseArgs();
  }
};
