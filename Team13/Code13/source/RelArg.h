#pragma once
#include <string>

#include "PQLGrammar.h"

class RelArg {
 private:
  std::string val;

 protected:
  RelArgType type;

 public:
  RelArg(std::string s, RelArgType t);

  // Getters
  std::string getVal();
  RelArgType getRelArgType();

  virtual ~RelArg() {}

  friend bool operator==(const RelArg& lhs, const RelArg& rhs) {
    return lhs.val == rhs.val && lhs.type == rhs.type;
  }

  friend bool operator<(const RelArg& lhs, const RelArg& rhs) {
    return lhs.val < rhs.val;
  }
};
