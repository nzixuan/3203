#include "SuchThat.h"

SuchThat::SuchThat(ClauseArgs ca, RelRefType rrt)
    : Clause(ca), relationship(rrt) {}

RelRefType SuchThat::getRelationship() { return relationship; }
