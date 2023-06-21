#include "PQLExcept.h"

// Helper functions to throw errors
void throwSyntaxError() { throw std::logic_error("SyntaxError"); }

void throwSemanticError() { throw std::logic_error("SemanticError"); }
