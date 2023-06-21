#include "ClauseNode.h"

ClauseNode::ClauseNode(std::shared_ptr<ClauseIdx> clauseIdx, int id)
    : idx(clauseIdx), Node(NodeType::ClauseNode, id) {
  priority = calculatePriority(clauseIdx);
}

int ClauseNode::calculatePriority(std::shared_ptr<ClauseIdx> clauseIdx) {
  int n = idx->clause->getSynonyms().size();
  // No synonyms
  switch (idx->clauseType) {
    case ClauseType::SuchThat:
      return calculatePrioritySuchThat(clauseIdx, n);
    case ClauseType::Pattern:
      return calculatePriorityPattern(clauseIdx, n);
    case ClauseType::With:
      return calculatePriorityWith(clauseIdx, n);
    default:
      return -1;
  }
}

int ClauseNode::calculatePrioritySuchThat(std::shared_ptr<ClauseIdx> clauseIdx,
                                          int numSyns) {
  RelRefType relationship =
      std::dynamic_pointer_cast<SuchThat>(idx->clause)->getRelationship();
  if (relationship == RelRefType::Affects ||
      relationship == RelRefType::AffectsT) {
    switch (clauseIdx->clause->getSynonyms().size()) {
      case 0:
        return Priority::affectsNoSyn;
      case 1:
        return Priority::affectsOneSyn;
      default:
        return Priority::affectsMultiSyn;
    }
  }

  switch (numSyns) {
    case 0:
      if (relationship == RelRefType::NextT) {
        return Priority::noSynNextT;
      }
      return Priority::noSyn;
    case 1:
      if (isTransitive(relationship)) {
        return Priority::oneSynSuchThatT;
      }
      return Priority::oneSynSuchThat;
    default:
      if (isTransitive(relationship)) {
        return Priority::multiSynSuchThatT;
      }
      return Priority::multiSynSuchThat;
  }
}

int ClauseNode::calculatePriorityPattern(std::shared_ptr<ClauseIdx> clauseIdx,
                                         int numSyns) {
  switch (numSyns) {
    case 0:
      return Priority::noSyn;
    case 1:
      return Priority::oneSynPattern;
    default:
      return Priority::multiSynPattern;
  }
}

int ClauseNode::calculatePriorityWith(std::shared_ptr<ClauseIdx> clauseIdx,
                                      int numSyns) {
  switch (numSyns) {
    case 0:
      return Priority::noSyn;
    case 1:
      return Priority::oneSynWith;
    default:
      return Priority::multiSynWith;
  }
}

bool ClauseNode::isTransitive(RelRefType type) {
  // AffectsT is handled separately.
  return type == RelRefType::FollowsT || type == RelRefType::ParentT ||
         type == RelRefType::NextT || type == RelRefType::CallsT;
}

int ClauseNode::getPriority() { return priority; }

std::shared_ptr<ClauseIdx> ClauseNode::getClauseIdx() { return idx; }
