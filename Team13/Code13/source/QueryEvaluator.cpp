#include "QueryEvaluator.h"

QueryEvaluator::QueryEvaluator(Query query, PKB* pkb,
                               std::list<std::string>& results)
    : query(std::move(query)), pkb(pkb), results(results) {}

void QueryEvaluator::evaluate() {
  auto suchThatEvaluator = SuchThatEvaluator(query, pkb);
  auto patternEvaluator = PatternEvaluator(pkb);
  auto withEvaluator = WithEvaluator(pkb);
  ClauseManager clauseManager = *query.getClauseManager();
  const auto selectTable = new Table();
  bool isQueryValid = true;

  // Iterate through groups
  while (!clauseManager.hasNoClauses()) {
    Group group = clauseManager.nextGroup();
    const auto groupTable = new Table();
    isQueryValid =
        isQueryValid && evaluateGroup(group, groupTable, suchThatEvaluator,
                                      patternEvaluator, withEvaluator);
    if (!isQueryValid) {
      // Clause has no results, can return immediately.
      if (query.isBooleanSelect()) {
        results.emplace_back("FALSE");
      }
      return;
    }
    std::set<std::string> selectSynsInGroupTable;
    for (auto const& syn : query.getSelectArgs()) {
      if (group.containsSyn(syn)) {
        selectSynsInGroupTable.insert(syn->getVal());
      }
    }
    if (!selectSynsInGroupTable.empty()) {
      selectTable->combineTable(*groupTable);
    }
    delete groupTable;
  }

  populateResults(selectTable, isQueryValid);
}

bool QueryEvaluator::evaluateGroup(Group group, Table* groupTable,
                                   SuchThatEvaluator& suchThatEvaluator,
                                   PatternEvaluator& patternEvaluator,
                                   WithEvaluator& withEvaluator) const {
  bool isGroupValid = true;
  // Iterate through the indices within the groups
  while (!group.hasNoClauses()) {
    ClauseIdx idx = group.nextIdx();
    bool isClauseValid;
    switch (idx.clauseType) {
      case ClauseType::SuchThat:
        // Parse such that clause
        // Retrieve the Such That object stored in idx
        isClauseValid = suchThatEvaluator.evalSuchThatClause(
            *std::dynamic_pointer_cast<SuchThat>(idx.clause), groupTable);
        if (!isClauseValid) {
          return false;
        }
        isGroupValid = isGroupValid && isClauseValid;
        break;
      case ClauseType::Pattern:
        // Parse pattern clause
        patternEvaluator.evalPatternClause(
            *std::dynamic_pointer_cast<Pattern>(idx.clause), groupTable);
        isGroupValid = isGroupValid && !groupTable->isTableEmpty();
        break;
      case ClauseType::With:
        // Parse with clause
        isClauseValid = withEvaluator.evalWithClause(
            *std::dynamic_pointer_cast<With>(idx.clause), groupTable);
        if (!isClauseValid) {
          return false;
        }
        isGroupValid = isGroupValid && isClauseValid;
        break;
    }
  }
  return isGroupValid;
}

void QueryEvaluator::populateResults(Table* t, bool isQueryValid) {
  if (query.isBooleanSelect()) {
    // Boolean return
    results.emplace_back(isQueryValid ? "TRUE" : "FALSE");
    return;
  }

  // Single/tuple return
  const std::list<std::string> selectSyns = getSelectStringVals();
  if ((!t->contains(selectSyns) && isQueryValid) ||
      query.getClauseManager()->hasNoClauses()) {
    // Select syn is not in table but table has valid results OR no clauses
    // Add all values of select syn to table
    updateTableWithSelectVals(t);
  }

  std::vector<std::shared_ptr<Synonym>> selectArgs = query.getSelectArgs();
  std::map<int, std::function<PKBEntity(int)>> attrFnMap;
  for (size_t i = 0; i < selectArgs.size(); i++) {
    std::shared_ptr<Synonym> syn = selectArgs[i];
    if (syn->getRelArgType() == RelArgType::AttrRef) {
      std::shared_ptr<AttrRef> attrRef =
          std::dynamic_pointer_cast<AttrRef>(syn);
      if (attrRef->getEntityType() == EntityType::Call &&
          attrRef->getAttrName() == AttrName::ProcName) {
        // call.procName
        attrFnMap[i] = [this](const int i) {
          return pkb->getProcEntityFromCall(i);
        };
      } else if (attrRef->getEntityType() == EntityType::Read &&
                 attrRef->getAttrName() == AttrName::VarName) {
        // read.varName
        attrFnMap[i] = [this](const int i) {
          return pkb->getVarEntityFromRead(i);
        };
      } else if (attrRef->getEntityType() == EntityType::Print &&
                 attrRef->getAttrName() == AttrName::VarName) {
        // print.varName
        attrFnMap[i] = [this](const int i) {
          return pkb->getVarEntityFromPrint(i);
        };
      }
    }
  }

  std::set<std::string> resultSet;
  for (auto const& row : t->getValues(selectSyns)) {
    std::string str;
    for (size_t i = 0; i < row.size(); i++) {
      std::string s = row[i];
      if (attrFnMap.find(i) != attrFnMap.end()) {
        s = attrFnMap[i](std::stoi(s)).val;
      }
      str += s + " ";
    }
    str.pop_back();
    resultSet.insert(str);
  }
  std::copy(resultSet.begin(), resultSet.end(), std::back_inserter(results));
}

std::list<std::string> QueryEvaluator::getSelectStringVals() {
  std::list<std::string> result;
  for (auto const& arg : query.getSelectArgs()) {
    result.emplace_back(arg->getVal());
  }
  return result;
}

void QueryEvaluator::updateTableWithSelectVals(Table* t) {
  for (auto const& arg : query.getSelectArgs()) {
    const EntityType type = arg->getEntityType();
    std::list<PKBEntity> res = getAllOfType(type);
    t->update(arg->getVal(), res);
  }
}

std::list<PKBEntity> QueryEvaluator::getAllOfType(EntityType type) const {
  switch (type) {
    case EntityType::Assign:
      return pkb->getAssignStmts();
    case EntityType::Constant:
      return pkb->getConstants();
    case EntityType::Call:
      return pkb->getCallStmts();
    case EntityType::If:
      return pkb->getIfStmts();
    case EntityType::Print:
      return pkb->getPrintStmts();
    case EntityType::Procedure:
      return pkb->getProcList();
    case EntityType::Read:
      return pkb->getReadStmts();
    case EntityType::Stmt:
      return pkb->getAllStmts();
    case EntityType::Variable:
      return pkb->getVariables();
    case EntityType::While:
      return pkb->getWhileStmts();
  }
}
