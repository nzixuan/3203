#pragma once
#include <algorithm>
#include <deque>
#include <memory>
#include <set>
#include <unordered_map>
#include <vector>

#include "ClauseNode.h"
#include "Group.h"
#include "SynNode.h"

typedef std::shared_ptr<Group> GroupPtr;

class QueryOptimiser {
 private:
  std::vector<std::vector<bool>> adjList;
  std::vector<bool> visited;
  bool hasCommonSyn(std::set<Synonym> s1, std::set<Synonym> s2);
  void processNode(ClauseNodePtr pClauseNode, Group* pGroup);
  void processNode(SynNodePtr pSynNode, Group* pGroup);

 public:
  void optimise(std::deque<Group>* clauses, std::set<Synonym> syns);
};
