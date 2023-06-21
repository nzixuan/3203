#include "Pattern.h"

#include <memory>

Pattern::Pattern(ClauseArgs ca, Synonym patternSyn)
    : Clause(ca), patternSyn(patternSyn) {
  synonyms.insert(*std::make_shared<Synonym>(patternSyn));
}

Synonym Pattern::getPatternSyn() { return patternSyn; }
