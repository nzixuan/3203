#include "PKB.h"

#include <string>
#include <vector>
#include <list>

#include "FollowsStorage.h"
#include "FollowsTStorage.h"
#include "ModifiesStorage.h"
#include "ParentStorage.h"
#include "ParentTStorage.h"
#include "UsesStorage.h"

PKB::PKB() {}

void PKB::addProcedure(std::string procName) {
  procedureSet.insert(procName).second;
}

void PKB::addStatement(int stmtNo, EntityType sType) {
  switch (sType) {
    case EntityType::Read:
      readStmtSet.insert(stmtNo);
      break;
    case EntityType::Print:
      printStmtSet.insert(stmtNo);
      break;
    case EntityType::Assign:
      assignStmtSet.insert(stmtNo);
      break;
    case EntityType::While:
      whileStmtSet.insert(stmtNo);
      break;
    case EntityType::If:
      ifStmtSet.insert(stmtNo);
      break;
    case EntityType::Call:
      callStmtSet.insert(stmtNo);
      break;
    case EntityType::Stmt:
      assignStmtSet.insert(stmtNo);
      break;
    default:
      throw std::logic_error("Type provided is not a statement type!");
  }

  stmtEntityMap.insert({stmtNo, {std::to_string(stmtNo), sType}});
}

void PKB::addVariable(std::string name) { varSet.insert(name); }

void PKB::addConstant(std::string value) { constSet.insert(value); }

void PKB::addProcNameForCall(int stmtNo, std::string procName) {
  callsToProcMap.insert({stmtNo, {procName, EntityType::Procedure}});
}

void PKB::addVarNameForPrint(int stmtNo, std::string varName) {
  printToVarMap.insert({stmtNo, {varName, EntityType::Variable}});
}

void PKB::addVarNameForRead(int stmtNo, std::string varName) {
  readToVarMap.insert({stmtNo, {varName, EntityType::Variable}});
}

int PKB::getTotalStmtCount() { return stmtEntityMap.size(); }

PKBEntity PKB::getStmtEntity(int stmt) {
  const auto iter = stmtEntityMap.find(stmt);
  if (iter != stmtEntityMap.end()) {
    return iter->second;
  } else {
    throw std::logic_error("Stmt entity not found in PKB: Stmt no " +
                           std::to_string(stmt));
  }
}

PKBEntity PKB::getProcEntityFromCall(int callsStmtNo) {
  const auto iter = callsToProcMap.find(callsStmtNo);
  if (iter != callsToProcMap.end()) {
    return iter->second;
  } else {
    throw std::logic_error(
        "Call stmt not found in callToProcMap: Stmt value: " + callsStmtNo);
  }
}

PKBEntity PKB::getVarEntityFromPrint(int printStmtNo) {
  const auto iter = printToVarMap.find(printStmtNo);
  if (iter != printToVarMap.end()) {
    return iter->second;
  } else {
    throw std::logic_error(
        "Print stmt not found in printToVarMap: Stmt value: " + printStmtNo);
  }
}

PKBEntity PKB::getVarEntityFromRead(int readStmtNo) {
  const auto iter = readToVarMap.find(readStmtNo);
  if (iter != readToVarMap.end()) {
    return iter->second;
  } else {
    throw std::logic_error("Read stmt not found in readToVarMap: Stmt value: " +
                           readStmtNo);
  }
}

std::list<PKBEntity> PKB::getProcList() {
  std::list<PKBEntity> results;
  for (const std::string& procedure : procedureSet) {
    PKBEntity procEntity = {procedure, EntityType::Procedure};
    results.emplace_back(procEntity);
  }

  return results;
}

std::list<PKBEntity> PKB::getReadStmts() {
  std::list<PKBEntity> results;
  for (const int& readStmt : readStmtSet) {
    PKBEntity readEntity = {std::to_string(readStmt), EntityType::Read};
    results.emplace_back(readEntity);
  }

  return results;
}

std::list<PKBEntity> PKB::getPrintStmts() {
  std::list<PKBEntity> results;
  for (const int& printStmt : printStmtSet) {
    PKBEntity printEntity = {std::to_string(printStmt), EntityType::Print};
    results.emplace_back(printEntity);
  }

  return results;
}

std::list<PKBEntity> PKB::getAssignStmts() {
  std::list<PKBEntity> results;
  for (const int& assignStmt : assignStmtSet) {
    PKBEntity assignEntity = {std::to_string(assignStmt), EntityType::Assign};
    results.emplace_back(assignEntity);
  }

  return results;
}

std::list<PKBEntity> PKB::getIfStmts() {
  std::list<PKBEntity> results;
  for (const int& ifStmt : ifStmtSet) {
    PKBEntity ifEntity = {std::to_string(ifStmt), EntityType::If};
    results.emplace_back(ifEntity);
  }

  return results;
}

std::list<PKBEntity> PKB::getWhileStmts() {
  std::list<PKBEntity> results;
  for (const int& whileStmt : whileStmtSet) {
    PKBEntity whileEntity = {std::to_string(whileStmt), EntityType::While};
    results.emplace_back(whileEntity);
  }

  return results;
}

std::list<PKBEntity> PKB::getCallStmts() {
  std::list<PKBEntity> results;
  for (const int& callStmt : callStmtSet) {
    PKBEntity callEntity = {std::to_string(callStmt), EntityType::Call};
    results.emplace_back(callEntity);
  }

  return results;
}

std::list<PKBEntity> PKB::getVariables() {
  std::list<PKBEntity> results;
  for (const std::string& variable : varSet) {
    PKBEntity varEntity = {variable, EntityType::Variable};
    results.emplace_back(varEntity);
  }

  return results;
}

std::list<PKBEntity> PKB::getConstants() {
  std::list<PKBEntity> results;
  for (const std::string& constant : constSet) {
    PKBEntity constEntity = {constant, EntityType::Constant};
    results.emplace_back(constEntity);
  }

  return results;
}

std::list<PKBEntity> PKB::getAllStmts() {
  std::list<PKBEntity> results;
  for (auto& keyValue : stmtEntityMap) {
    results.emplace_back(keyValue.second);
  }
  return results;
}

SuchThatQueriable* PKB::getRelSuchThatStoragePtr(RelRefType relRefType) {
  switch (relRefType) {
    case RelRefType::Modifies:
      return &modifiesStorage;
    case RelRefType::Uses:
      return &usesStorage;
    case RelRefType::Follows:
      return &followsStorage;
    case RelRefType::FollowsT:
      return &followsTStorage;
    case RelRefType::Parent:
      return &parentStorage;
    case RelRefType::ParentT:
      return &parentTStorage;
    case RelRefType::Calls:
      return &callsStorage;
    case RelRefType::CallsT:
      return &callsTStorage;
    case RelRefType::Next:
      return &nextStorage;
    case RelRefType::NextT:
      nextTQuerier.setStorage(&nextStorage);
      return &nextTQuerier;
    case RelRefType::Affects:
      affectsQuerier.setStorages(&cfgStorage, &modifiesStorage, &usesStorage,
                                 &stmtEntityMap);
      return &affectsQuerier;
    case RelRefType::AffectsT:
      affectsQuerier.setStorages(&cfgStorage, &modifiesStorage, &usesStorage,
                                 &stmtEntityMap);
      affectsTQuerier.setStorage(&affectsQuerier);
      return &affectsTQuerier;
    default:
      throw std::logic_error("Unknown SuchThat RelRefType");
  }
}

void PKB::setToRelStorage(PKBEntity lhs, PKBEntity rhs, RelRefType relRefType) {
  switch (relRefType) {
    case RelRefType::Modifies:
      modifiesStorage.set(lhs, rhs);
      break;
    case RelRefType::Uses:
      usesStorage.set(lhs, rhs);
      break;
    case RelRefType::Follows:
      followsStorage.set(lhs, rhs);
      followsTStorage.set(lhs, rhs);
      break;
    case RelRefType::Parent:
      parentStorage.set(lhs, rhs);
      parentTStorage.set(lhs, rhs);
      break;
    case RelRefType::Calls:
      callsStorage.set(lhs, rhs);
      callsTStorage.set(lhs, rhs);
      break;
    case RelRefType::Next:
      nextStorage.set(lhs, rhs);
      break;
    default:
      throw std::logic_error("Unknown SuchThat Storage RelRefType");
  }
}

void PKB::clearQuerierCache() {
  affectsQuerier.clearCache();
  affectsTQuerier.clearCache();
  nextTQuerier.clearCache();
}

/*
        API for Such That Storages
*/

void PKB::storeRel(PKBEntity lhs, PKBEntity rhs, RelRefType relRefType) {
  setToRelStorage(lhs, rhs, relRefType);
}

std::list<PKBEntityPair> PKB::getLhsSynRhsSynForRel(EntityType lhsType,
                                                    EntityType rhsType,
                                                    RelRefType relRefType) {
  SuchThatQueriable* ptr = getRelSuchThatStoragePtr(relRefType);
  return ptr->getLhsSynRhsSyn(lhsType, rhsType);
}

std::list<PKBEntity> PKB::getLhsRhsSameSynObj(EntityType type,
                                              RelRefType relRefType) {
  SuchThatQueriable* ptr = getRelSuchThatStoragePtr(relRefType);
  return ptr->getLhsRhsSameSynObj(type);
}

std::list<PKBEntity> PKB::getLhsSynRhsWildcardForRel(EntityType lhsType,
                                                     RelRefType relRefType) {
  SuchThatQueriable* ptr = getRelSuchThatStoragePtr(relRefType);
  return ptr->getLhsSynRhsWildcard(lhsType);
}

std::list<PKBEntity> PKB::getLhsSynRhsConstantForRel(EntityType lhsType,
                                                     QueryConstant rhsConstant,
                                                     RelRefType relRefType) {
  SuchThatQueriable* ptr = getRelSuchThatStoragePtr(relRefType);
  return ptr->getLhsSynRhsConstant(lhsType, rhsConstant);
}

std::list<PKBEntity> PKB::getLhsWildcardRhsSynForRel(EntityType rhsType,
                                                     RelRefType relRefType) {
  SuchThatQueriable* ptr = getRelSuchThatStoragePtr(relRefType);
  return ptr->getLhsWildcardRhsSyn(rhsType);
}

bool PKB::hasLhsWildcardRhsWildcardForRel(RelRefType relRefType) {
  SuchThatQueriable* ptr = getRelSuchThatStoragePtr(relRefType);
  return ptr->hasLhsWildcardRhsWildcard();
}

bool PKB::hasLhsWildcardRhsConstantForRel(QueryConstant rhsConstant,
                                          RelRefType relRefType) {
  SuchThatQueriable* ptr = getRelSuchThatStoragePtr(relRefType);
  return ptr->hasLhsWildcardRhsConstant(rhsConstant);
}

std::list<PKBEntity> PKB::getLhsConstantRhsSynForRel(QueryConstant lhsConstant,
                                                     EntityType rhsType,
                                                     RelRefType relRefType) {
  SuchThatQueriable* ptr = getRelSuchThatStoragePtr(relRefType);
  return ptr->getLhsConstantRhsSyn(lhsConstant, rhsType);
}
bool PKB::hasLhsConstantRhsWildcardForRel(QueryConstant lhsConstant,
                                          RelRefType relRefType) {
  SuchThatQueriable* ptr = getRelSuchThatStoragePtr(relRefType);
  return ptr->hasLhsConstantRhsWildcard(lhsConstant);
}
bool PKB::hasLhsConstantRhsConstantForRel(QueryConstant lhsConstant,
                                          QueryConstant rhsConstant,
                                          RelRefType relRefType) {
  SuchThatQueriable* ptr = getRelSuchThatStoragePtr(relRefType);
  return ptr->hasLhsConstantRhsConstant(lhsConstant, rhsConstant);
}

/*
        API for CFG Storages
*/

void PKB::storeCFG(std::list<CFGRoot*> roots) {
  this->cfgStorage.setStmtEntityMap(&stmtEntityMap);
  this->cfgStorage.storeCfg(roots);
}

/*
        API for Pattern Storages
*/
void PKB::storeFullExpAssignMap(int stmtNo, std::string lhs, std::string rhs) {
  PatternStorage* ptr = &assignStorage;
  return ptr->storeFullExpAssignMap(stmtNo, lhs, rhs);
}

void PKB::storeSubExpsAssignMap(int stmtNo, std::string lhs, std::string rhs) {
  PatternStorage* ptr = &assignStorage;
  return ptr->storeSubExpsAssignMap(stmtNo, lhs, rhs);
}

std::list<PKBEntityPair> PKB::getPatternAssignLhsSynRhsWildcard() {
  PatternStorage* ptr = &assignStorage;
  return ptr->getPatternAssignLhsSynRhsWildcard();
}

std::list<PKBEntityPair> PKB::getPatternAssignLhsSynRhsExp(std::string exp,
                                                           bool isExactMatch) {
  PatternStorage* ptr = &assignStorage;
  return ptr->getPatternAssignLhsSynRhsExp(exp, isExactMatch);
}

std::list<PKBEntity> PKB::getPatternAssignLhsWildcardRhsWildcard() {
  PatternStorage* ptr = &assignStorage;
  return ptr->getPatternAssignLhsWildcardRhsWildcard();
}

std::list<PKBEntity> PKB::getPatternAssignLhsWildcardRhsExp(std::string exp,
                                                            bool isExactMatch) {
  PatternStorage* ptr = &assignStorage;
  return ptr->getPatternAssignLhsWildcardRhsExp(exp, isExactMatch);
}

std::list<PKBEntity> PKB::getPatternAssignLhsConstRhsWildcard(
    std::string lhsConst) {
  PatternStorage* ptr = &assignStorage;
  return ptr->getPatternAssignLhsConstRhsWildcard(lhsConst);
}

std::list<PKBEntity> PKB::getPatternAssignLhsConstRhsExp(std::string lhsConst,
                                                         std::string rhsExp,
                                                         bool isExactMatch) {
  PatternStorage* ptr = &assignStorage;
  return ptr->getPatternAssignLhsConstRhsExp(lhsConst, rhsExp, isExactMatch);
}

/*
        API for Pattern If and While Storages
*/

CtrlVarStorage* PKB::getCtrlVarStoragePtr(EntityType type) {
  if (type == EntityType::If) {
    return &ifStorage;
  } else if (type == EntityType::While) {
    return &whileStorage;
  }
}

void PKB::addControlVariable(EntityType type, int stmtNo,
                             std::string variable) {
  CtrlVarStorage* ptr = getCtrlVarStoragePtr(type);
  return ptr->addControlVariable(stmtNo, variable);
}

std::list<PKBEntity> PKB::getPatternWildcard(EntityType type) {
  CtrlVarStorage* ptr = getCtrlVarStoragePtr(type);
  return ptr->getPatternWildcard();
}

std::list<PKBEntityPair> PKB::getPatternSyn(EntityType type) {
  CtrlVarStorage* ptr = getCtrlVarStoragePtr(type);
  return ptr->getPatternSyn();
}
std::list<PKBEntity> PKB::getPatternConst(std::string constVal,
                                          EntityType type) {
  CtrlVarStorage* ptr = getCtrlVarStoragePtr(type);
  return ptr->getPatternConst(constVal);
}
