#pragma once
#include <list>
#include <string>
#include <unordered_set>

#include "CFG.h"
#include "PKBEntity.h"

class CFGStorage {
 private:
  std::list<CFGRoot*> cfgRoots;
  StmtToEntityMap* stmtEntityMap;

 public:
  CFGStorage();
  ~CFGStorage();
  void setStmtEntityMap(StmtToEntityMap* stmtEntityMap);

  void storeCfg(std::list<CFGRoot*> cfgRoots);

  bool isIfNode(CFGAbstract* node);
  bool isWhileNode(CFGAbstract* node);
  bool isNodeVisited(CFGAbstract* node,
                     std::unordered_set<int> visitedWhileStmts);

  CFGAbstract* getRoot(int stmtNo);
  CFGAbstract* findNode(int stmtNo);
  std::list<CFGRoot*> getRoots();
};
