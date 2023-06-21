#pragma once
#include <queue>
#include <string>
#include <list>
#include <unordered_set>

#include "CFGStorage.h"
#include "ModifiesStorage.h"
#include "PKBEntity.h"
#include "SuchThatQuerier.h"
#include "UsesStorage.h"

const int NO_STARTING_STMT = -1;

class AffectsQuerier : public SuchThatQuerier {
 private:
  CFGStorage* cfgStorage;
  ModifiesStorage* modifiesStorage;
  UsesStorage* usesStorage;
  StmtToEntityMap* stmtEntityMap;

  bool isStmtAssignType(int stmtNo);
  bool isStmtReadCallOrAssignType(int stmtNo);
  int getNumFromQueryConstant(QueryConstant constant);
  std::string getModifiedVarFromAssignStmt(QueryConstant lhsConstant);
  CFGAbstract* getNodeFromCfg(int stmtNo);
  CFGAbstract* getFirstWhileNode(CFGAbstract* node);

  int getFirstStmtModifyingVarInNode(CFGAbstract* node, std::string varName,
                                     int stmtBeforeStartingStmt);
  int getFirstAssignStmtUsingVarInNode(CFGAbstract* node, std::string varName,
                                       int stmtBeforeStartingStmt);
  std::list<int> getListOfAssignUsingVarInNode(CFGAbstract* node,
                                               std::string varName,
                                               int stmtBeforeStartingStmt);
  void addAllUseBeforeModifiesStmt(std::string modifyingStmt,
                                   std::list<int> usesStmts, int modifiesStmt,
                                   std::list<PKBEntity>& results);

  void addNextNodeToQueue(CFGAbstract* currNode,
                          std::queue<CFGAbstract*>& queue,
                          std::unordered_set<int>& visitedWhileStmts);

 public:
  AffectsQuerier();
  AffectsQuerier(CFGStorage* cfgStorage, ModifiesStorage* modifiesStorage,
                 UsesStorage* usesStorage, StmtToEntityMap* stmtEntityMap);
  ~AffectsQuerier();

  void setStorages(CFGStorage* cfgStorage, ModifiesStorage* modifiesStorage,
                   UsesStorage* usesStorage, StmtToEntityMap* stmtEntityMap);

  std::list<PKBEntityPair> getLhsSynRhsSyn(EntityType lhsType,
                                           EntityType rhsType) override;
  std::list<PKBEntity> getLhsRhsSameSynObj(EntityType type) override;
  std::list<PKBEntity> getLhsSynRhsWildcard(EntityType lhsType) override;
  std::list<PKBEntity> getLhsSynRhsConstant(EntityType lhsType,
                                            QueryConstant rhsConstant) override;

  std::list<PKBEntity> getLhsWildcardRhsSyn(EntityType rhsType) override;
  bool hasLhsWildcardRhsWildcard() override;
  bool hasLhsWildcardRhsConstant(QueryConstant rhsConstant) override;

  std::list<PKBEntity> getLhsConstantRhsSyn(QueryConstant lhsConstant,
                                            EntityType rhsType) override;
  bool hasLhsConstantRhsWildcard(QueryConstant lhsConstant) override;
  bool hasLhsConstantRhsConstant(QueryConstant lhsConstant,
                                 QueryConstant rhsConstant) override;
};
