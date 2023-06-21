#include <stdexcept>
#include <string>
#include <vector>
#include <list>
#include <unordered_set>

#include "AffectsQuerier.h"

AffectsQuerier::AffectsQuerier() {
  this->cfgStorage = NULL;
  this->modifiesStorage = NULL;
  this->usesStorage = NULL;
  this->stmtEntityMap = NULL;
}

AffectsQuerier::AffectsQuerier(CFGStorage* cfgStorage,
                               ModifiesStorage* modifiesStorage,
                               UsesStorage* usesStorage,
                               StmtToEntityMap* stmtEntityMap) {
  this->cfgStorage = cfgStorage;
  this->modifiesStorage = modifiesStorage;
  this->usesStorage = usesStorage;
  this->stmtEntityMap = stmtEntityMap;
}

AffectsQuerier::~AffectsQuerier() {}

void AffectsQuerier::setStorages(CFGStorage* cfgStorage,
                                 ModifiesStorage* modifiesStorage,
                                 UsesStorage* usesStorage,
                                 StmtToEntityMap* stmtEntityMap) {
  this->cfgStorage = cfgStorage;
  this->modifiesStorage = modifiesStorage;
  this->usesStorage = usesStorage;
  this->stmtEntityMap = stmtEntityMap;
}

bool AffectsQuerier::isStmtAssignType(int stmtNo) {
  auto stmtEntityIter = stmtEntityMap->find(stmtNo);
  if (stmtEntityIter == stmtEntityMap->end()) {
    throw std::logic_error("Stmt not found stmtEntityMap: " + stmtNo);
  } else {
    PKBEntity stmtEntity = stmtEntityIter->second;
    return stmtEntity.entityType == EntityType::Assign;
  }
}

bool AffectsQuerier::isStmtReadCallOrAssignType(int stmtNo) {
  auto stmtEntityIter = stmtEntityMap->find(stmtNo);
  if (stmtEntityIter == stmtEntityMap->end()) {
    throw std::logic_error("Stmt not found stmtEntityMap: " + stmtNo);
  } else {
    PKBEntity stmtEntity = stmtEntityIter->second;
    return stmtEntity.entityType == EntityType::Read ||
           stmtEntity.entityType == EntityType::Call ||
           stmtEntity.entityType == EntityType::Assign;
  }
}

int AffectsQuerier::getNumFromQueryConstant(QueryConstant constant) {
  int stmtNo = 0;

  if (constant.type != QueryConstantType::Number) {
    throw std::logic_error("constant is not a number " + constant.val);
  }

  try {
    stmtNo = stoi(constant.val);
  } catch (std::invalid_argument argument) {
    throw std::logic_error("constant value is not a number " + constant.val);
  }

  return stmtNo;
}

std::string AffectsQuerier::getModifiedVarFromAssignStmt(
    QueryConstant constant) {
  std::list<PKBEntity> modifiedVarList =
      modifiesStorage->getLhsConstantRhsSyn(constant, EntityType::Variable);

  if (modifiedVarList.size() != 1) {
    throw std::logic_error(
        "Assign statement retrived from node contains more than one modified "
        "variable when it should contain only one. Stmt no: " +
        constant.val);
  }

  return modifiedVarList.front().val;
}

CFGAbstract* AffectsQuerier::getNodeFromCfg(int stmtNo) {
  CFGAbstract* currNode = cfgStorage->findNode(stmtNo);

  if (currNode == NULL) {
    throw std::logic_error("Stmt not present in CFG. Stmt no: " + stmtNo);
  }
  return currNode;
}

CFGAbstract* AffectsQuerier::getFirstWhileNode(CFGAbstract* node) {
  CFGAbstract* currNode = node;
  while (currNode != NULL) {
    if (cfgStorage->isWhileNode(currNode)) {
      return currNode;
    }
    currNode = currNode->getNext();
  }
  return currNode;
}

int AffectsQuerier::getFirstStmtModifyingVarInNode(CFGAbstract* node,
                                                   std::string varName,
                                                   int stmtBeforeStartingStmt) {
  std::vector<int> stmtNos = node->getStmts();

  std::vector<int>::iterator iter;

  // If no starting stmt is provided, start from the beginning of vector, else
  // start from after the staring stmt
  if (stmtBeforeStartingStmt == NO_STARTING_STMT) {
    iter = stmtNos.begin();
  } else {
    iter =
        std::find(stmtNos.begin(), stmtNos.end(), stmtBeforeStartingStmt) + 1;
  }

  for (std::vector<int>::iterator it = iter; it != stmtNos.end(); it++) {
    int currStmtNo = *it;

    QueryConstant stmtConstant = {std::to_string(currStmtNo),
                                  QueryConstantType::Number};

    if (modifiesStorage->hasLhsConstantRhsConstant(
            stmtConstant, {varName, QueryConstantType::Identifier})) {
      if (!isStmtReadCallOrAssignType(currStmtNo)) {
        continue;
      }
      // Stmt is modifiying modified var
      return currStmtNo;
    }
  }
  return NULL;
}

int AffectsQuerier::getFirstAssignStmtUsingVarInNode(
    CFGAbstract* node, std::string varName, int stmtBeforeStartingStmt) {
  std::vector<int> stmtNos = node->getStmts();

  std::vector<int>::iterator iter;

  // If no starting stmt is provided, start from the beginning of vector, else
  // start from after the staring stmt
  if (stmtBeforeStartingStmt == NO_STARTING_STMT) {
    iter = stmtNos.begin();
  } else {
    iter =
        std::find(stmtNos.begin(), stmtNos.end(), stmtBeforeStartingStmt) + 1;
  }

  for (std::vector<int>::iterator it = iter; it != stmtNos.end(); it++) {
    int currStmtNo = *it;

    QueryConstant stmtConstant = {std::to_string(currStmtNo),
                                  QueryConstantType::Number};

    if (isStmtAssignType(currStmtNo) &&
        usesStorage->hasLhsConstantRhsConstant(
            stmtConstant, {varName, QueryConstantType::Identifier})) {
      // Stmt is assign and is using var
      return currStmtNo;
    }
  }
  return NULL;
}

std::list<int> AffectsQuerier::getListOfAssignUsingVarInNode(
    CFGAbstract* node, std::string varName, int stmtBeforeStartingStmt) {
  std::list<int> results;

  std::vector<int> stmtNos = node->getStmts();

  std::vector<int>::iterator iter;

  // If no starting stmt is provided, start from the beginning of vector, else
  // start from after the staring stmt
  if (stmtBeforeStartingStmt == NO_STARTING_STMT) {
    iter = stmtNos.begin();
  } else {
    iter =
        std::find(stmtNos.begin(), stmtNos.end(), stmtBeforeStartingStmt) + 1;
  }

  for (std::vector<int>::iterator it = iter; it != stmtNos.end(); it++) {
    int currStmtNo = *it;

    QueryConstant stmtConstant = {std::to_string(currStmtNo),
                                  QueryConstantType::Number};

    // Stmt is assign and is using var
    if (isStmtAssignType(currStmtNo) &&
        usesStorage->hasLhsConstantRhsConstant(
            stmtConstant, {varName, QueryConstantType::Identifier})) {
      results.emplace_back(currStmtNo);
    }
  }
  return results;
}

void AffectsQuerier::addAllUseBeforeModifiesStmt(
    std::string modifyingStmt, std::list<int> usesStmts, int modifiesStmt,
    std::list<PKBEntity>& results) {
  for (int usesStmt : usesStmts) {
    if (modifiesStmt != NULL && usesStmt > modifiesStmt) {
      // Stop adding once modifies stmt is in between
      break;
    }
    PKBEntity useEntity = {std::to_string(usesStmt), EntityType::Assign};
    results.push_back(useEntity);
    addToCache(modifyingStmt, useEntity, lhsToRhsMapCache);
  }
}

void AffectsQuerier::addNextNodeToQueue(
    CFGAbstract* currNode, std::queue<CFGAbstract*>& nodeQueue,
    std::unordered_set<int>& visitedWhileStmts) {
  if (cfgStorage->isIfNode(currNode)) {
    // Add nodes in then and else to the queue
    CFGIf* ifNode = reinterpret_cast<CFGIf*>(currNode);
    nodeQueue.push(ifNode->getThen());
    nodeQueue.push(ifNode->getElse());
  } else if (cfgStorage->isWhileNode(currNode)) {
    CFGWhile* whileNode = reinterpret_cast<CFGWhile*>(currNode);

    // Set while node as visited
    int whileStmt = whileNode->getStmts().front();
    visitedWhileStmts.insert(whileStmt);

    // Add nodes in while and after while to the queue
    nodeQueue.push(whileNode->getWhile());
    nodeQueue.push(whileNode->getNext());
  } else {
    nodeQueue.push(currNode->getNext());
  }
}

/*
        API Definitions
*/

std::list<PKBEntityPair> AffectsQuerier::getLhsSynRhsSyn(EntityType lhsType,
                                                         EntityType rhsType) {
  std::list<PKBEntityPair> results;
  // If synonym is not assign stmt, return empty list;
  if ((lhsType != EntityType::Assign && lhsType != EntityType::Stmt) ||
      (rhsType != EntityType::Assign && rhsType != EntityType::Stmt)) {
    return results;
  }

  // Gets list of assign stmts that modifies variable
  std::list<PKBEntity> listOfModifyingAssigns =
      modifiesStorage->getLhsSynRhsWildcard(EntityType::Assign);

  for (PKBEntity& lhsEntity : listOfModifyingAssigns) {
    std::string modifiedVarName = getModifiedVarFromAssignStmt(
        {lhsEntity.val, QueryConstantType::Number});

    // If variabled modified is not used in any assign stmt, continue
    if (usesStorage
            ->getLhsSynRhsConstant(
                EntityType::Assign,
                {modifiedVarName, QueryConstantType::Identifier})
            .empty()) {
      continue;
    }

    // Get list of assign stmts affected by lhsEntity stmt
    std::list<PKBEntity> listOfUseEntitiesForStmt = getLhsConstantRhsSyn(
        {lhsEntity.val, QueryConstantType::Number}, rhsType);
    // Add lhsEntity stmt and each of the affected stmts as a pair to results
    for (PKBEntity& rhsEntity : listOfUseEntitiesForStmt) {
      results.push_back({lhsEntity, rhsEntity});
    }
  }

  return results;
}
std::list<PKBEntity> AffectsQuerier::getLhsRhsSameSynObj(EntityType type) {
  std::list<PKBEntity> results;
  // If synonym is not assign stmt, return empty list;
  if (type != EntityType::Assign && type != EntityType::Stmt) {
    return results;
  }

  // Gets list of assign stmts that modifies variable
  std::list<PKBEntity> listOfModifyingAssigns =
      modifiesStorage->getLhsSynRhsWildcard(EntityType::Assign);

  for (PKBEntity& entity : listOfModifyingAssigns) {
    std::string modifiedVarName =
        getModifiedVarFromAssignStmt({entity.val, QueryConstantType::Number});

    // If variabled modified is not used in any assign stmt, continue
    if (usesStorage
            ->getLhsSynRhsConstant(
                EntityType::Assign,
                {modifiedVarName, QueryConstantType::Identifier})
            .empty()) {
      continue;
    }
    QueryConstant stmtConstant = {entity.val, QueryConstantType::Number};

    if (hasLhsConstantRhsConstant(stmtConstant, stmtConstant)) {
      results.push_back(entity);
    }
  }

  return results;
}
std::list<PKBEntity> AffectsQuerier::getLhsSynRhsWildcard(EntityType lhsType) {
  std::list<PKBEntity> results;
  // If synonym is not assign stmt, return empty list;
  if (lhsType != EntityType::Assign && lhsType != EntityType::Stmt) {
    return results;
  }

  // Gets list of assign stmts that modifies variable
  std::list<PKBEntity> listOfModifyingAssigns =
      modifiesStorage->getLhsSynRhsWildcard(EntityType::Assign);

  for (PKBEntity& entity : listOfModifyingAssigns) {
    std::string modifiedVarName =
        getModifiedVarFromAssignStmt({entity.val, QueryConstantType::Number});

    // If variabled modified is not used in any assign stmt, continue
    if (usesStorage
            ->getLhsSynRhsConstant(
                EntityType::Assign,
                {modifiedVarName, QueryConstantType::Identifier})
            .empty()) {
      continue;
    }
    QueryConstant stmtConstant = {entity.val, QueryConstantType::Number};

    if (hasLhsConstantRhsWildcard(stmtConstant)) {
      results.push_back(entity);
    }
  }

  return results;
}
std::list<PKBEntity> AffectsQuerier::getLhsSynRhsConstant(
    EntityType lhsType, QueryConstant rhsConstant) {
  // Checks cache for results
  if (hasEntitiesFromCache(rhsConstant.val, rhsToLhsMapCache)) {
    return getEntitiesFromCache(rhsConstant.val, lhsType, rhsToLhsMapCache);
  }

  // Use list since each lhs assign stmt is checked and added once
  std::list<PKBEntity> results;
  // If synonym is not assign stmt, return empty list;
  if (lhsType != EntityType::Assign && lhsType != EntityType::Stmt) {
    return results;
  }

  int stmtNo = getNumFromQueryConstant(rhsConstant);
  // If its not assign stmt, return false;
  if (!isStmtAssignType(stmtNo)) {
    return results;
  }

  std::list<PKBEntity> listOfModifyingAssigns =
      modifiesStorage->getLhsSynRhsWildcard(EntityType::Assign);

  for (PKBEntity& entity : listOfModifyingAssigns) {
    std::string modifiedVarName =
        getModifiedVarFromAssignStmt({entity.val, QueryConstantType::Number});

    // If variabled modified is not used in any assign stmt, continue
    if (!usesStorage->hasLhsConstantRhsConstant(
            rhsConstant, {modifiedVarName, QueryConstantType::Identifier})) {
      continue;
    }
    QueryConstant stmtConstant = {entity.val, QueryConstantType::Number};
    if (hasLhsConstantRhsConstant(stmtConstant, rhsConstant)) {
      results.push_back(entity);
      addToCache(rhsConstant.val, entity, rhsToLhsMapCache);
    }
  }

  return results;
}

std::list<PKBEntity> AffectsQuerier::getLhsWildcardRhsSyn(EntityType rhsType) {
  // Use set to prevent duplicates
  std::unordered_set<PKBEntity, PKBEntity_hash_fn> results;
  // If synonym is not assign stmt, return empty list;
  if (rhsType != EntityType::Assign && rhsType != EntityType::Stmt) {
    return convertPKBEntitySetToList(&results);
  }

  std::list<PKBEntity> listOfModifyingAssigns =
      modifiesStorage->getLhsSynRhsWildcard(EntityType::Assign);

  for (PKBEntity& lhsEntity : listOfModifyingAssigns) {
    std::string modifiedVarName = getModifiedVarFromAssignStmt(
        {lhsEntity.val, QueryConstantType::Number});

    // If variabled modified is not used in any assign stmt, continue
    if (usesStorage
            ->getLhsSynRhsConstant(
                EntityType::Assign,
                {modifiedVarName, QueryConstantType::Identifier})
            .empty()) {
      continue;
    }
    QueryConstant stmtConstant = {lhsEntity.val, QueryConstantType::Number};

    std::list<PKBEntity> listOfUseEntitiesForStmt =
        getLhsConstantRhsSyn(stmtConstant, rhsType);

    // Add affected stmts to results
    for (PKBEntity& rhsEntity : listOfUseEntitiesForStmt) {
      results.insert(rhsEntity);
    }
  }

  return convertPKBEntitySetToList(&results);
}
bool AffectsQuerier::hasLhsWildcardRhsWildcard() {
  // Checks cache for results
  if (!lhsToRhsMapCache.empty() || !rhsToLhsMapCache.empty()) {
    return true;
  }

  std::queue<CFGAbstract*> nodeQueue;
  std::unordered_set<int> visitedWhile;
  std::list<CFGRoot*> roots = cfgStorage->getRoots();

  for (CFGRoot* root : roots) {
    // Add the start of each CFG
    nodeQueue.push(root->getNext());
  }

  CFGAbstract* currNode;

  // Perform BFS
  while (!nodeQueue.empty()) {
    currNode = nodeQueue.front();
    nodeQueue.pop();

    if (currNode == NULL) {
      continue;
    }

    // Skip if node has been visited
    if (cfgStorage->isNodeVisited(currNode, visitedWhile)) {
      continue;
    }

    std::vector<int> nodeStmts = currNode->getStmts();

    for (int nodeStmt : nodeStmts) {
      // Check if nodeStmt is assign type
      if (!isStmtAssignType(nodeStmt)) {
        continue;
      }

      // Check if nodeStmt affects any other stmt
      if (hasLhsConstantRhsWildcard(
              {std::to_string(nodeStmt), QueryConstantType::Number})) {
        return true;
      }
    }

    addNextNodeToQueue(currNode, nodeQueue, visitedWhile);
  }
  return false;
}
bool AffectsQuerier::hasLhsWildcardRhsConstant(QueryConstant rhsConstant) {
  // Checks cache for results
  if (hasEntitiesFromCache(rhsConstant.val, rhsToLhsMapCache)) {
    return true;
  }

  int stmtNo = getNumFromQueryConstant(rhsConstant);
  // If its not assign stmt, return false;
  if (!isStmtAssignType(stmtNo)) {
    return false;
  }
  std::queue<CFGAbstract*> nodeQueue;
  std::unordered_set<int> visitedWhile;
  CFGAbstract* currNode = cfgStorage->getRoot(stmtNo);
  // Add the start of the CFG
  nodeQueue.push(currNode->getNext());

  // Perform BFS
  while (!nodeQueue.empty()) {
    currNode = nodeQueue.front();
    nodeQueue.pop();

    if (currNode == NULL) {
      continue;
    }

    // Skip if node has been visited
    if (cfgStorage->isNodeVisited(currNode, visitedWhile)) {
      continue;
    }

    std::vector<int> nodeStmts = currNode->getStmts();

    for (int nodeStmt : nodeStmts) {
      // Check if nodeStmt is assign type
      if (!isStmtAssignType(nodeStmt)) {
        continue;
      }
      std::string modifiedVarName = getModifiedVarFromAssignStmt(
          {std::to_string(nodeStmt), QueryConstantType::Number});
      // Check if rhsConstant stmt uses variable that is modified in nodeStmt
      if (!usesStorage->hasLhsConstantRhsConstant(
              rhsConstant, {modifiedVarName, QueryConstantType::Identifier})) {
        continue;
      }
      // Check if nodeStmt affects rhsConstant stmt
      if (hasLhsConstantRhsConstant(
              {std::to_string(nodeStmt), QueryConstantType::Number},
              rhsConstant)) {
        return true;
      }
    }

    addNextNodeToQueue(currNode, nodeQueue, visitedWhile);
  }
  return false;
}

std::list<PKBEntity> AffectsQuerier::getLhsConstantRhsSyn(
    QueryConstant lhsConstant, EntityType rhsType) {
  // Checks cache for results
  if (hasEntitiesFromCache(lhsConstant.val, lhsToRhsMapCache)) {
    return getEntitiesFromCache(lhsConstant.val, rhsType, lhsToRhsMapCache);
  }

  std::list<PKBEntity> results;
  // If synonym is not assign stmt, return empty list;
  if (rhsType != EntityType::Assign && rhsType != EntityType::Stmt) {
    return results;
  }
  int stmtNo = getNumFromQueryConstant(lhsConstant);
  // If its not assign stmt, return false;
  if (!isStmtAssignType(stmtNo)) {
    return results;
  }
  std::string modifiedVarName = getModifiedVarFromAssignStmt(lhsConstant);
  CFGAbstract* currNode = getNodeFromCfg(stmtNo);
  int stmtBeforeStartingStmt = stmtNo;
  std::queue<CFGAbstract*> nodeQueue;
  std::unordered_set<int> visitedWhile;

  nodeQueue.push(currNode);

  // Perform BFS
  while (!nodeQueue.empty()) {
    currNode = nodeQueue.front();
    nodeQueue.pop();

    if (currNode == NULL) {
      continue;
    }

    // Skip if node has been visited
    if (cfgStorage->isNodeVisited(currNode, visitedWhile)) {
      continue;
    }

    int stmtModifyingVar = getFirstStmtModifyingVarInNode(
        currNode, modifiedVarName, stmtBeforeStartingStmt);
    std::list<int> listOfAssignStmtUsingVar = getListOfAssignUsingVarInNode(
        currNode, modifiedVarName, stmtBeforeStartingStmt);

    if (stmtModifyingVar != NULL) {
      addAllUseBeforeModifiesStmt(lhsConstant.val, listOfAssignStmtUsingVar,
                                  stmtModifyingVar, results);
      continue;
    } else if (!listOfAssignStmtUsingVar.empty()) {
      // No modifying stmt between lhs stmt and rhs stmt in the node
      addAllUseBeforeModifiesStmt(lhsConstant.val, listOfAssignStmtUsingVar,
                                  stmtModifyingVar, results);
    }

    addNextNodeToQueue(currNode, nodeQueue, visitedWhile);
    stmtBeforeStartingStmt = NO_STARTING_STMT;
  }

  // Store results in cache (Works for Affects since all resultant stmts are
  // assign)
  for (PKBEntity& rhsEntity : results) {
    addToCache(lhsConstant.val, rhsEntity, lhsToRhsMapCache);
  }

  return results;
}

bool AffectsQuerier::hasLhsConstantRhsWildcard(QueryConstant lhsConstant) {
  // Checks cache for results
  if (hasEntitiesFromCache(lhsConstant.val, lhsToRhsMapCache)) {
    return true;
  }

  int stmtNo = getNumFromQueryConstant(lhsConstant);

  // If its not assign stmt, return false;
  if (!isStmtAssignType(stmtNo)) {
    return false;
  }
  std::string modifiedVarName = getModifiedVarFromAssignStmt(lhsConstant);
  CFGAbstract* currNode = getNodeFromCfg(stmtNo);
  int stmtBeforeStartingStmt = stmtNo;
  std::queue<CFGAbstract*> nodeQueue;
  std::unordered_set<int> visitedWhile;

  nodeQueue.push(currNode);

  // Perform BFS
  while (!nodeQueue.empty()) {
    currNode = nodeQueue.front();
    nodeQueue.pop();

    if (currNode == NULL) {
      continue;
    }

    // Skip if node has been visited
    if (cfgStorage->isNodeVisited(currNode, visitedWhile)) {
      continue;
    }

    int stmtModifyingVar = getFirstStmtModifyingVarInNode(
        currNode, modifiedVarName, stmtBeforeStartingStmt);
    int assignStmtUsingVar = getFirstAssignStmtUsingVarInNode(
        currNode, modifiedVarName, stmtBeforeStartingStmt);

    if (stmtModifyingVar != NULL && assignStmtUsingVar != NULL) {
      // Assumption: All the stmts in node are always in increasing order
      if (assignStmtUsingVar <= stmtModifyingVar) {
        // Modifying stmt is after lhs stmt and rhs stmt in the node
        return true;
      } else {
        // Modifying stmt is before lhs stmt and rhs stmt in the node
        continue;
      }
    } else if (assignStmtUsingVar != NULL) {
      // No modifying stmt between lhs stmt and rhs stmt in the node
      return true;
    } else if (stmtModifyingVar != NULL) {
      // No using stmt in the node
      continue;
    }

    addNextNodeToQueue(currNode, nodeQueue, visitedWhile);
    stmtBeforeStartingStmt = NO_STARTING_STMT;
  }

  return false;
}

bool AffectsQuerier::hasLhsConstantRhsConstant(QueryConstant lhsConstant,
                                               QueryConstant rhsConstant) {
  // Checks either cache for results
  if (hasEntityFromCache(lhsConstant.val, rhsConstant, lhsToRhsMapCache) ||
      hasEntityFromCache(rhsConstant.val, lhsConstant, rhsToLhsMapCache)) {
    return true;
  }

  int lhsStmtNo = getNumFromQueryConstant(lhsConstant);
  int rhsStmtNo = getNumFromQueryConstant(rhsConstant);
  // If either is not assign stmt, return false;
  if (!isStmtAssignType(lhsStmtNo) || !isStmtAssignType(rhsStmtNo)) {
    return false;
  }
  std::string modifiedVarName = getModifiedVarFromAssignStmt(lhsConstant);
  // Check if rhs stmt uses constant, if not return false
  if (!usesStorage->hasLhsConstantRhsConstant(
          rhsConstant, {modifiedVarName, QueryConstantType::Identifier})) {
    return false;
  }
  CFGAbstract* currNode = getNodeFromCfg(lhsStmtNo);
  int stmtBeforeStartingStmt = lhsStmtNo;
  std::queue<CFGAbstract*> nodeQueue;
  std::unordered_set<int> visitedWhile;

  nodeQueue.push(currNode);

  // Perform BFS to find rhs stmt and check if there is any modifying stmt in
  // front of stmt
  while (!nodeQueue.empty()) {
    currNode = nodeQueue.front();
    nodeQueue.pop();

    if (currNode == NULL) {
      continue;
    }

    // Skip if node has been visited
    if (cfgStorage->isNodeVisited(currNode, visitedWhile)) {
      continue;
    }

    int stmtModifyingVar = getFirstStmtModifyingVarInNode(
        currNode, modifiedVarName, stmtBeforeStartingStmt);
    int assignStmtUsingVar = getFirstAssignStmtUsingVarInNode(
        currNode, modifiedVarName, stmtBeforeStartingStmt);

    if (stmtModifyingVar != NULL && assignStmtUsingVar != NULL) {
      // Assumption: All the stmts in node are always in increasing order
      if (assignStmtUsingVar <= stmtModifyingVar &&
          assignStmtUsingVar == rhsStmtNo) {
        // Modifying stmt is after lhs stmt and rhs stmt in the node
        return true;
      } else {
        // Modifying stmt is before lhs stmt and rhs stmt in the node
        continue;
      }
    } else if (assignStmtUsingVar != NULL && assignStmtUsingVar == rhsStmtNo) {
      // No modifying stmt between lhs stmt and rhs stmt in the node
      return true;
    } else if (stmtModifyingVar != NULL) {
      // No using stmt in the node
      continue;
    }

    addNextNodeToQueue(currNode, nodeQueue, visitedWhile);
    stmtBeforeStartingStmt = NO_STARTING_STMT;
  }

  return false;
}
