#include "RelArg.h"

RelArg::RelArg(std::string s, RelArgType t) : val(s), type(t) {}

// Getters
std::string RelArg::getVal() { return val; }

RelArgType RelArg::getRelArgType() { return type; }
