#pragma once
#include <string>

#include "RelArg.h"

class SourceArg : public RelArg {
 private:
  bool isExact;

 public:
  explicit SourceArg(std::string val, bool isExact = true);

  bool getIsExact();
};
