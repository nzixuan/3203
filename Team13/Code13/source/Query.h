#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "ClauseManager.h"
#include "PQLGrammar.h"
#include "Pattern.h"
#include "SuchThat.h"
#include "Synonym.h"

class Query {
  std::vector<std::shared_ptr<Synonym>> selectArgs;
  ClauseManager clauseManager;
  // True if Select BOOLEAN and BOOLEAN is not declared synonym.
  bool isSelectingBoolean = false;

 public:
  Query() = default;

  // Getters
  std::vector<std::shared_ptr<Synonym>> getSelectArgs();
  bool isBooleanSelect();
  ClauseManager* getClauseManager();

  // Setters
  void setBooleanSelectFlag();
  void addSelectArg(std::shared_ptr<Synonym> arg);
  void addSelectArg(Synonym syn);
  void addSelectArg(AttrRef ar);
  void finalize();

  friend bool operator==(const Query& q1, const Query& q2) {
    if (q1.selectArgs.size() != q2.selectArgs.size()) return false;
    for (size_t i = 0; i < q1.selectArgs.size(); i++) {
      if (*q1.selectArgs[i] == *q2.selectArgs[i]) continue;
      return false;
    }
    return q1.isSelectingBoolean == q2.isSelectingBoolean &&
           q1.clauseManager == q2.clauseManager;
  }
};
