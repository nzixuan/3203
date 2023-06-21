#pragma once
#include <memory>

#include "Group.h"
#include "Node.h"

// Possible priority levels of a clause. This allows us to slot in a new
// priority wherever.
namespace Priority {

// Unless specified, such that refers to only non transitive clauses
constexpr int noSyn = 0;
constexpr int noSynNextT = noSyn + 1;
constexpr int oneSynWith = noSynNextT + 1;
constexpr int oneSynSuchThat = oneSynWith + 1;
constexpr int oneSynPattern = oneSynWith + 1;
constexpr int oneSynSuchThatT = oneSynSuchThat + 1;
constexpr int multiSynSuchThat = oneSynSuchThatT + 1;
constexpr int multiSynWith = multiSynSuchThat + 1;
constexpr int multiSynSuchThatT = multiSynWith + 1;
constexpr int multiSynPattern = multiSynWith + 1;
constexpr int affectsNoSyn = multiSynPattern + 1;
constexpr int affectsOneSyn = affectsNoSyn + 1;
constexpr int affectsMultiSyn = affectsOneSyn + 1;
}  // namespace Priority

class ClauseNode;
typedef std::shared_ptr<ClauseNode> ClauseNodePtr;

class ClauseNode : public Node {
 private:
  int priority;
  std::shared_ptr<ClauseIdx> idx;
  int calculatePriority(std::shared_ptr<ClauseIdx> clauseIdx);
  int calculatePrioritySuchThat(std::shared_ptr<ClauseIdx> clauseIdx,
                                int numSyns);
  bool isTransitive(RelRefType type);
  int calculatePriorityPattern(std::shared_ptr<ClauseIdx> clauseIdx,
                               int numSyns);
  int calculatePriorityWith(std::shared_ptr<ClauseIdx> clauseIdx, int numSyns);

 public:
  ClauseNode(std::shared_ptr<ClauseIdx> clauseIdx, int id);
  int getPriority();
  std::shared_ptr<ClauseIdx> getClauseIdx();
};
