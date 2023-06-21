#include "CFGStorage.h"

#include <queue>
#include <stdexcept>
#include <unordered_set>
#include <vector>

CFGStorage::CFGStorage() { this->stmtEntityMap = NULL; }
CFGStorage::~CFGStorage() { this->cfgRoots.clear(); }

void CFGStorage::setStmtEntityMap(StmtToEntityMap* stmtEntityMap) {
  this->stmtEntityMap = stmtEntityMap;
}

void CFGStorage::storeCfg(std::list<CFGRoot*> roots) { this->cfgRoots = roots; }

bool CFGStorage::isIfNode(CFGAbstract* node) {
  std::vector<int> stmtNos = node->getStmts();

  // If node should only contain 1 stmt
  if (stmtNos.size() != 1) {
    return false;
  }

  auto finder = stmtEntityMap->find(stmtNos.front());
  if (finder == stmtEntityMap->end()) {
    throw std::logic_error("Stmt not found stmtEntityMap: " + stmtNos.front());
  } else {
    return finder->second.entityType == EntityType::If;
  }
}

bool CFGStorage::isWhileNode(CFGAbstract* node) {
  std::vector<int> stmtNos = node->getStmts();

  // While node should only contain 1 stmt
  if (stmtNos.size() != 1) {
    return false;
  }

  auto finder = stmtEntityMap->find(stmtNos.front());
  if (finder == stmtEntityMap->end()) {
    throw std::logic_error("Stmt not found stmtEntityMap: " + stmtNos.front());
  } else {
    return finder->second.entityType == EntityType::While;
  }
}

bool CFGStorage::isNodeVisited(CFGAbstract* node,
                               std::unordered_set<int> visitedWhileStmts) {
  std::vector<int> stmtNos = node->getStmts();

  return visitedWhileStmts.find(stmtNos.front()) != visitedWhileStmts.end();
}

CFGAbstract* CFGStorage::getRoot(int stmtNo) {
  for (CFGRoot* root : getRoots()) {
    if (stmtNo <= (root->getLast())) {
      return root;
    }
  }
  return NULL;
}

CFGAbstract* CFGStorage::findNode(int stmtNo) {
  CFGAbstract* rootToSearch = getRoot(stmtNo);

  if (rootToSearch == NULL) {
    return NULL;
  }

  CFGAbstract* currNode = rootToSearch->getNext();
  std::queue<CFGAbstract*> queue;
  queue.push(currNode);
  std::unordered_set<int> visitedWhile;

  while (!queue.empty()) {
    currNode = queue.front();
    queue.pop();

    // Reached the end of CFG
    if (currNode == NULL) {
      continue;
    }

    // Skip if node has been visited
    if (isNodeVisited(currNode, visitedWhile)) {
      continue;
    }

    std::vector<int> stmts = currNode->getStmts();
    std::vector<int>::iterator it =
        std::find(stmts.begin(), stmts.end(), stmtNo);

    if (it != stmts.end()) {
      return currNode;

    } else {
      if (isIfNode(currNode)) {
        // Add nodes in then and else to the queue
        CFGIf* ifNode = reinterpret_cast<CFGIf*>(currNode);
        queue.push(ifNode->getThen());
        queue.push(ifNode->getElse());
      } else if (isWhileNode(currNode)) {
        CFGWhile* whileNode = reinterpret_cast<CFGWhile*>(currNode);

        // Set while node as visited
        int whileStmt = whileNode->getStmts().front();
        visitedWhile.insert(whileStmt);

        // Add nodes in while and after while to the queue
        queue.push(whileNode->getWhile());
        queue.push(whileNode->getNext());
      } else {
        queue.push(currNode->getNext());
      }
    }
  }

  return NULL;
}

std::list<CFGRoot*> CFGStorage::getRoots() { return this->cfgRoots; }
