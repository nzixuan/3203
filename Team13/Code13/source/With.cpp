#include "With.h"

With::With(ClauseArgs ca, WithRefType wrt) : Clause(ca), wrType(wrt) {}

WithRefType With::getWithRefType() { return wrType; }
