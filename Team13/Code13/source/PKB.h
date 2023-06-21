#pragma once

#include <stdio.h>

#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <list>

#include "AffectsQuerier.h"
#include "AffectsTQuerier.h"
#include "AssignStorage.h"
#include "CFG.h"
#include "CFGStorage.h"
#include "CallsStorage.h"
#include "CallsTStorage.h"
#include "CtrlVarStorage.h"
#include "FollowsStorage.h"
#include "FollowsTStorage.h"
#include "IfStorage.h"
#include "ModifiesStorage.h"
#include "NextStorage.h"
#include "NextTQuerier.h"
#include "PKBEntity.h"
#include "ParentStorage.h"
#include "ParentTStorage.h"
#include "PatternStorage.h"
#include "SuchThatQueriable.h"
#include "SuchThatStorage.h"
#include "UsesStorage.h"
#include "WhileStorage.h"
#include "common.h"

class PKB {
 public:
  PKB();
  /*
      Adder Methods for SPA
  */
  void addProcedure(std::string procName);

  /* Add Statement to respective Statement Lists */
  void addStatement(int stmtNo, EntityType sType);

  /* Add variable to variable list */
  void addVariable(std::string name);

  /* Add constant to constant list */
  void addConstant(std::string value);

  /* Adds the name of procedure that's used in calls stmt*/
  void addProcNameForCall(int stmtNo, std::string procName);

  /* Adds the name of variable that's used in print stmt*/
  void addVarNameForPrint(int stmtNo, std::string varName);

  /* Adds the name of variable that's used in read stmt*/
  void addVarNameForRead(int stmtNo, std::string varName);

  // returns list of 'read' statements
  virtual std::list<PKBEntity> getReadStmts();

  // returns list of 'print' statements
  virtual std::list<PKBEntity> getPrintStmts();

  // returns list of 'assign' statements
  virtual std::list<PKBEntity> getAssignStmts();

  // returns list of 'if' statements
  virtual std::list<PKBEntity> getIfStmts();

  // returns list of 'while' statements
  virtual std::list<PKBEntity> getWhileStmts();

  // returns list of 'call' statements
  virtual std::list<PKBEntity> getCallStmts();

  // returns list of all variables stored
  virtual std::list<PKBEntity> getVariables();

  // returns list of all constants stored
  virtual std::list<PKBEntity> getConstants();

  // returns list of all procedures stored
  virtual std::list<PKBEntity> getProcList();

  // returns list of all stmt stored
  virtual std::list<PKBEntity> getAllStmts();

  int getTotalStmtCount();

  // To get the respective PKBEntity for Stmt Types
  PKBEntity getStmtEntity(int stmt);

  // To get the respective Procedure PKBEntity for Calls Stmt
  virtual PKBEntity getProcEntityFromCall(int callsStmtNo);

  // To get the respective Variable PKBEntity for Print Stmt
  virtual PKBEntity getVarEntityFromPrint(int printStmtNo);

  // To get the respective Variable PKBEntity for Read Stmt
  virtual PKBEntity getVarEntityFromRead(int readStmtNo);

  // Clears cache in the respective queriers
  void clearQuerierCache();

  /* ==================== API for Such That storages ==================== */
  void storeRel(PKBEntity lhs, PKBEntity rhs, RelRefType relRefType);

  virtual std::list<PKBEntityPair> getLhsSynRhsSynForRel(EntityType lhsType,
                                                         EntityType rhsType,
                                                         RelRefType relRefType);
  virtual std::list<PKBEntity> getLhsRhsSameSynObj(EntityType type,
                                                   RelRefType relRefType);
  virtual std::list<PKBEntity> getLhsSynRhsWildcardForRel(
      EntityType lhsType, RelRefType relRefType);
  virtual std::list<PKBEntity> getLhsSynRhsConstantForRel(
      EntityType lhsType, QueryConstant rhsConstant, RelRefType relRefType);

  virtual std::list<PKBEntity> getLhsWildcardRhsSynForRel(
      EntityType rhsType, RelRefType relRefType);
  virtual bool hasLhsWildcardRhsWildcardForRel(RelRefType relRefType);
  virtual bool hasLhsWildcardRhsConstantForRel(QueryConstant rhsConstant,
                                               RelRefType relRefType);

  virtual std::list<PKBEntity> getLhsConstantRhsSynForRel(
      QueryConstant lhsConstant, EntityType rhsType, RelRefType relRefType);
  virtual bool hasLhsConstantRhsWildcardForRel(QueryConstant lhsConstant,
                                               RelRefType relRefType);
  virtual bool hasLhsConstantRhsConstantForRel(QueryConstant lhsConstant,
                                               QueryConstant rhsConstant,
                                               RelRefType relRefType);

  /* ========================= API for CFG storages ========================= */
  void storeCFG(std::list<CFGRoot*> roots);

  /* ==================== API for Pattern Assign storages ====================
   */
  void storeFullExpAssignMap(int stmtNo, std::string lhs, std::string rhs);
  void storeSubExpsAssignMap(int stmtNo, std::string lhs, std::string rhs);

  virtual std::list<PKBEntityPair> getPatternAssignLhsSynRhsWildcard();
  virtual std::list<PKBEntityPair> getPatternAssignLhsSynRhsExp(
      std::string exp, bool isExactMatch);
  virtual std::list<PKBEntity> getPatternAssignLhsWildcardRhsWildcard();
  virtual std::list<PKBEntity> getPatternAssignLhsWildcardRhsExp(
      std::string exp, bool isExactMatch);
  virtual std::list<PKBEntity> getPatternAssignLhsConstRhsWildcard(
      std::string lhsConst);
  virtual std::list<PKBEntity> getPatternAssignLhsConstRhsExp(
      std::string lhsConst, std::string rhsExp, bool isExactMatch);

  /* ================= API for Pattern If and While Storages =================
   */
  void addControlVariable(EntityType type, int stmtNo, std::string variable);

  virtual std::list<PKBEntity> getPatternWildcard(EntityType type);
  virtual std::list<PKBEntityPair> getPatternSyn(EntityType type);
  virtual std::list<PKBEntity> getPatternConst(std::string constVal,
                                               EntityType type);

 private:
  std::unordered_set<std::string> procedureSet;
  std::unordered_set<std::string> varSet;
  std::unordered_set<std::string> constSet;
  std::unordered_set<int> readStmtSet;
  std::unordered_set<int> printStmtSet;
  std::unordered_set<int> assignStmtSet;
  std::unordered_set<int> ifStmtSet;
  std::unordered_set<int> whileStmtSet;
  std::unordered_set<int> callStmtSet;

  StmtToEntityMap stmtEntityMap;

  StmtToEntityMap callsToProcMap;
  StmtToEntityMap printToVarMap;
  StmtToEntityMap readToVarMap;

  ModifiesStorage modifiesStorage;
  UsesStorage usesStorage;
  FollowsStorage followsStorage;
  FollowsTStorage followsTStorage;
  ParentStorage parentStorage;
  ParentTStorage parentTStorage;
  CallsStorage callsStorage;
  CallsTStorage callsTStorage;
  NextStorage nextStorage;
  AssignStorage assignStorage;
  IfStorage ifStorage;
  WhileStorage whileStorage;

  CFGStorage cfgStorage;

  NextTQuerier nextTQuerier;
  AffectsQuerier affectsQuerier;
  AffectsTQuerier affectsTQuerier;

  SuchThatQueriable* getRelSuchThatStoragePtr(RelRefType relRefType);

  void setToRelStorage(PKBEntity lhs, PKBEntity rhs, RelRefType relRefType);

  CtrlVarStorage* getCtrlVarStoragePtr(EntityType type);
};
