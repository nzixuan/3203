#include "QueryOptimiser.h"

#include <string>

void QueryOptimiser::optimise(std::deque<Group>* clauses,
                              std::set<Synonym> selectSyns) {
  /*
   * 1. We first create nodes out of the synonyms. (+1 node for no synonyms.)
   *	1.1 We also create a map from synonym value to SynNodePtr
   * 2. We then create ClauseNodes and add them to a vector. Then sort the
   *vector.
   * 3. Iterate through ClauseNodes and create edges with the synonyms used
   *(SynNodes).
   * 4. Iterate through vector of ClauseNodePtr, starting a DFS everytime we
   *encounter an unvisited node. 4.1 DFS function should take in a Group ptr,
   *and the finished group is inserted into clauses once DFS terminates. 4.2
   *Repeat until we reach the end of the vector.
   */

  // Initially, all clauses should be in a single group
  Group initialGroup = clauses->front();
  clauses->pop_front();

  size_t m = initialGroup.getNumOfClauses();
  size_t n = initialGroup.getUsedSynonyms().size();
  adjList = std::vector<std::vector<bool>>(m, std::vector<bool>(n + 1, false));
  visited = std::vector<bool>(m, false);

  // Create the SynNodes and map.
  // We use string here since synonyms are distinct in value.
  std::unordered_map<std::string, SynNodePtr> synMap;
  std::unordered_map<std::string, SynNodePtr>::iterator itr = synMap.begin();
  int k = 0;
  for (Synonym syn : initialGroup.getUsedSynonyms()) {
    // We don't actually need to store the synonym in the SynNode.
    // Optimisation of inserts using hints
    itr = synMap.insert(itr,
                        {syn.getVal(), std::make_shared<SynNode>(SynNode{k})});
    k++;
  }

  // Empty SynNode for clauses without synonyms
  synMap.insert(itr, {"", std::make_shared<SynNode>(SynNode{k})});

  std::vector<ClauseNodePtr> clauseNodes{m, nullptr};
  for (size_t i = 0; i < m; i++) {
    ClauseIdx idx = initialGroup.nextIdx();
    ClauseNodePtr pClauseNode =
        std::make_shared<ClauseNode>(std::make_shared<ClauseIdx>(idx), i);

    // Add the clause to the vector
    clauseNodes[i] = pClauseNode;
  }

  // Sort the clauses based on priority.
  std::sort(clauseNodes.begin(), clauseNodes.end(),
            [](ClauseNodePtr cnp1, ClauseNodePtr cnp2) {
              // Lambda function for comparison
              return cnp1->getPriority() < cnp2->getPriority();
            });

  for (size_t i = 0; i < m; i++) {
    ClauseNodePtr pClauseNode = clauseNodes[i];
    std::shared_ptr<ClauseIdx> idx = pClauseNode->getClauseIdx();

    // Clause does not have any synonyms
    if (!idx->clause->isUsingSynonyms()) {
      SynNodePtr pSynNode = synMap[""];
      pSynNode->addNeighbour(pClauseNode);
      pClauseNode->addNeighbour(pSynNode);
      // Create the edge between the two nodes.
      adjList[pClauseNode->getId()][pSynNode->getId()] = true;
      continue;
    }

    // Add this clause as a neighbour to all the used SynNodes
    for (Synonym syn : idx->clause->getSynonyms()) {
      SynNodePtr pSynNode = synMap[syn.getVal()];
      pSynNode->addNeighbour(pClauseNode);
      pClauseNode->addNeighbour(pSynNode);
      adjList[pClauseNode->getId()][pSynNode->getId()] = true;
    }
  }

  // Temporary group to hold clauses with no syns
  Group noSynGroup;

  // Proceed to do DFS on every unvisited clause node.
  for (const auto& pClauseNode : clauseNodes) {
    if (visited[pClauseNode->getId()]) continue;
    Group newGroup;
    processNode(pClauseNode, &newGroup);

    // Prioritise groups with select syns
    if (newGroup.getUsedSynonyms().size() == 0) {
      // We store this group to be executed first later.
      noSynGroup = newGroup;
    } else if (hasCommonSyn(newGroup.getUsedSynonyms(), selectSyns)) {
      clauses->push_front(newGroup);
    } else {
      clauses->push_back(newGroup);
    }
  }

  if (!noSynGroup.hasNoClauses()) {
    clauses->push_front(noSynGroup);
  }
}

void QueryOptimiser::processNode(ClauseNodePtr pClauseNode, Group* pGroup) {
  /*
   * ClauseNode component of DFS
   * 1. Mark this node as visited. Add this node to the group
   * 2. Iterate through its neighbouring SynNodes.
   */
  // NOTE: Because we do not delete the edge when visiting the node,
  // This ends up being BFS for now.
  visited[pClauseNode->getId()] = true;
  pGroup->addClause(*pClauseNode->getClauseIdx());
  for (auto& pNode : pClauseNode->getNeighbours()) {
    SynNodePtr pSynNode = std::dynamic_pointer_cast<SynNode>(pNode);

    // We already used this syn to reach this node
    if (!adjList[pClauseNode->getId()][pSynNode->getId()]) continue;
    processNode(pSynNode, pGroup);
  }
}

void QueryOptimiser::processNode(SynNodePtr pSynNode, Group* pGroup) {
  /*
   * SynNode component of DFS.
   * 1. Iterate through its neighbouring ClauseNodes. Skip if visited.
   * 2. Once all have been processed, we can set the flag for visited all
   * neighbours.
   */
  if (pSynNode->hasVisitedAll) {
    // We've visited all the neighbouring clauses already.
    return;
  }

  for (auto& pNode : pSynNode->getNeighbours()) {
    if (visited[pNode->getId()]) continue;
    ClauseNodePtr pClauseNode = std::dynamic_pointer_cast<ClauseNode>(pNode);
    // Remove the edge between the clause and synonym
    adjList[pClauseNode->getId()][pSynNode->getId()] = false;
    processNode(pClauseNode, pGroup);
  }

  // We've processed all the neighbours. All clauses have been visited.
  pSynNode->hasVisitedAll = true;
}

bool QueryOptimiser::hasCommonSyn(std::set<Synonym> s1, std::set<Synonym> s2) {
  return std::find_first_of(s1.begin(), s1.end(), s2.begin(), s2.end()) !=
         s1.end();
}
