#pragma once
#include <deque>
#include <memory>
#include <set>

#include "Group.h"
#include "Pattern.h"
#include "QueryOptimiser.h"
#include "SuchThat.h"
#include "With.h"

class ClauseManager {
 private:
  // We collate all clauses into a single group.
  Group group;
  std::deque<Group> clauses;

  // Optimiser to optimise clause execution order.
  QueryOptimiser queryOptimiser;

  // Prevents adding of clauses after we finalized the groups
  bool isFinalized = false;

 public:
  void addClause(SuchThat suchThat);
  void addClause(Pattern pattern);
  void addClause(With with);
  Group nextGroup();
  bool hasNoClauses();

  // Adds the group to clauses and prevents further clauses from being added.
  void finalize(std::set<Synonym> selectArgs);

  friend bool operator==(const ClauseManager& cm1, const ClauseManager& cm2) {
    return cm1.clauses == cm2.clauses;
  }
};
