#include "Clause.h"

Clause::Clause(ClauseArgs ca) : clauseArgs(ca) {
  for (const auto& arg : ca.relArgs) {
    if (arg->getRelArgType() == RelArgType::Synonym ||
        arg->getRelArgType() == RelArgType::AttrRef) {
      synonyms.insert(*std::dynamic_pointer_cast<Synonym>(arg));
    }
  }
}

std::set<Synonym> Clause::getSynonyms() { return synonyms; }

ClauseArgs Clause::getClauseArgs() { return clauseArgs; }

bool Clause::isUsingSynonyms() { return !synonyms.empty(); }
