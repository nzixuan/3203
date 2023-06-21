#pragma once
#include <deque>
#include <map>
#include <memory>
#include <set>

#include "Clause.h"
#include "Pattern.h"
#include "SuchThat.h"
#include "With.h"

struct ClauseIdx {
  ClauseType clauseType;
  std::shared_ptr<Clause> clause;

  explicit ClauseIdx(SuchThat st)
      : clauseType(ClauseType::SuchThat),
        clause(std::make_shared<SuchThat>(st)) {}

  explicit ClauseIdx(Pattern p)
      : clauseType(ClauseType::Pattern), clause(std::make_shared<Pattern>(p)) {}

  explicit ClauseIdx(With w)
      : clauseType(ClauseType::With), clause(std::make_shared<With>(w)) {}

  friend bool operator==(const ClauseIdx& ci1, const ClauseIdx& ci2) {
    return ci1.clauseType == ci2.clauseType && *ci1.clause == *ci2.clause;
  }
};

class Group {
 private:
  std::deque<ClauseIdx> clauseIndices;
  // Set of used synonyms
  std::set<Synonym> synonyms;
  // Map of how many times the synonym was used.
  std::map<Synonym, int> synFreq;

  void addSynonyms(ClauseIdx idx);
  void removeSynonyms(ClauseIdx idx);

 public:
  void addClause(SuchThat suchThat);
  void addClause(Pattern pattern);
  void addClause(With with);
  void addClause(ClauseIdx idx);  // Allows for direct adding of ClauseIdx

  bool hasNoClauses();
  ClauseIdx nextIdx();

  std::set<Synonym> getUsedSynonyms();
  int getNumOfClauses();
  bool containsSyn(const std::shared_ptr<Synonym>& s);

  friend bool operator==(const Group& g1, const Group& g2) {
    return g1.clauseIndices == g2.clauseIndices && g1.synonyms == g2.synonyms &&
           g1.synFreq == g2.synFreq;
  }
};
