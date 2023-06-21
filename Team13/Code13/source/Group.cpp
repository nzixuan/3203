#include "Group.h"

void Group::addClause(SuchThat suchThat) {
  ClauseIdx idx{suchThat};
  clauseIndices.push_back(idx);
  addSynonyms(idx);
}

void Group::addClause(Pattern pattern) {
  ClauseIdx idx{pattern};
  clauseIndices.push_back(idx);
  addSynonyms(idx);
}

void Group::addClause(With with) {
  ClauseIdx idx{with};
  clauseIndices.push_back(idx);
  addSynonyms(idx);
}

void Group::addClause(ClauseIdx idx) {
  clauseIndices.push_back(idx);
  addSynonyms(idx);
}

bool Group::hasNoClauses() { return clauseIndices.empty(); }

void Group::addSynonyms(ClauseIdx idx) {
  for (const auto& syn : idx.clause->getSynonyms()) {
    synonyms.insert(syn);

    if (synFreq.count(syn)) {
      synFreq[syn]++;
    } else {
      synFreq[syn] = 1;
    }
  }
}

void Group::removeSynonyms(ClauseIdx idx) {
  for (const auto& syn : idx.clause->getSynonyms()) {
    synFreq[syn]--;
    if (synFreq.count(syn)) {
      // There's still other clauses using this synonym.
      continue;
    }
    // No more clauses use this synonym.
    synonyms.erase(syn);
    synFreq.erase(syn);
  }
}

ClauseIdx Group::nextIdx() {
  ClauseIdx idx = clauseIndices.front();
  clauseIndices.pop_front();
  removeSynonyms(idx);
  return idx;
}

std::set<Synonym> Group::getUsedSynonyms() { return synonyms; }

int Group::getNumOfClauses() { return clauseIndices.size(); }

bool Group::containsSyn(const std::shared_ptr<Synonym>& syn) {
  return synonyms.find(*syn) != synonyms.end();
}
