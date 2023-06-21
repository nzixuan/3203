#include "SourceArg.h"

SourceArg::SourceArg(std::string val, bool isExact)
    : RelArg(val, RelArgType::SourceArg), isExact(isExact) {}

bool SourceArg::getIsExact() { return isExact; }
