#pragma once
#include <list>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "PQLGrammar.h"
// RelArg includes
#include "AttrRef.h"
#include "Number.h"
#include "RelArg.h"
#include "SourceArg.h"
#include "Synonym.h"
#include "Wildcard.h"

struct ClauseArgs {
  // We can have a variable number of arguments for each clause
  std::vector<std::shared_ptr<RelArg>> relArgs;

  void addRelArg(AttrRef ar) {
    relArgs.push_back(std::make_shared<AttrRef>(ar));
  }

  void addRelArg(Synonym s) { relArgs.push_back(std::make_shared<Synonym>(s)); }

  void addRelArg(Wildcard w) {
    relArgs.push_back(std::make_shared<Wildcard>(w));
  }

  void addRelArg(SourceArg sa) {
    relArgs.push_back(std::make_shared<SourceArg>(sa));
  }

  void addRelArg(Number n) { relArgs.push_back(std::make_shared<Number>(n)); }

  void addRelArg(std::shared_ptr<RelArg> ra) { relArgs.push_back(ra); }

  friend bool operator==(const ClauseArgs& ca1, const ClauseArgs& ca2) {
    if (ca1.relArgs.size() != ca2.relArgs.size()) return false;
    for (size_t i = 0; i < ca1.relArgs.size(); i++) {
      if (*ca1.relArgs[i] == *ca2.relArgs[i]) continue;
      return false;
    }
    return true;
  }
};

class Clause {
 private:
  ClauseArgs clauseArgs;

 protected:
  std::set<Synonym> synonyms;

 public:
  explicit Clause(ClauseArgs ca);

  ClauseArgs getClauseArgs();
  std::set<Synonym> getSynonyms();
  bool isUsingSynonyms();

  virtual ~Clause() {}

  friend bool operator==(const Clause& c1, const Clause& c2) {
    return c1.clauseArgs == c2.clauseArgs && c1.synonyms == c2.synonyms;
  }
};
