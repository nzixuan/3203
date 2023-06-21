#pragma once
#include <string>

#include "RelArg.h"

class Number : public RelArg {
 public:
  explicit Number(std::string val);
};
