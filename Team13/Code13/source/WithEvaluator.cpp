#include "WithEvaluator.h"

WithEvaluator::WithEvaluator(PKB* pkb) : pkb(pkb) {}

bool WithEvaluator::evalWithClause(With w, Table* t) {
  const auto [relArgs] = w.getClauseArgs();
  const std::shared_ptr<RelArg> lhs = relArgs[0];
  const std::shared_ptr<RelArg> rhs = relArgs[1];
  if (lhs->getRelArgType() == RelArgType::AttrRef &&
      rhs->getRelArgType() == RelArgType::AttrRef) {
    // 2 synonyms (e.g. v1.varName = v2.varName)
    evalTwoSynWith(w, t);
  } else if ((lhs->getRelArgType() == RelArgType::SourceArg &&
              rhs->getRelArgType() == RelArgType::SourceArg) ||
             (lhs->getRelArgType() == RelArgType::Number &&
              rhs->getRelArgType() == RelArgType::Number)) {
    // No synonyms (e.g. 12 = 12)
    return evalBoolWith(w);
  } else {
    return evalOneSynWith(w, t);
  }
  return !t->isTableEmpty();
}

/* ============================= Boolean with ============================= */

bool WithEvaluator::evalBoolWith(With w) {
  const auto [relArgs] = w.getClauseArgs();
  const std::shared_ptr<RelArg> lhs = relArgs[0];
  const std::shared_ptr<RelArg> rhs = relArgs[1];
  return lhs->getVal() == rhs->getVal();
}

/* ============================= One syn with ============================= */

bool WithEvaluator::evalOneSynWith(With w, Table* t) const {
  const auto [relArgs] = w.getClauseArgs();
  bool lhsSyn = relArgs[0]->getRelArgType() == RelArgType::AttrRef;
  const std::shared_ptr<AttrRef> attrRef =
      std::dynamic_pointer_cast<AttrRef>(lhsSyn ? relArgs[0] : relArgs[1]);
  const std::shared_ptr<RelArg> sourceArg = lhsSyn ? relArgs[1] : relArgs[0];
  std::list<PKBEntity> results;
  if (!isSpecial(attrRef)) {
    std::list<PKBEntity> pkbEnts = t->contains(attrRef->getVal())
                                       ? t->getPkbEntities(attrRef->getVal())
                                       : getAllOfType(attrRef->getEntityType());
    if (!std::any_of(pkbEnts.begin(), pkbEnts.end(),
                     [sourceArg](PKBEntity& pkbEnt) {
                       return pkbEnt.val == sourceArg->getVal();
                     })) {
      return false;
    }
    results.push_back({sourceArg->getVal(), EntityType::Assign});
  } else {
    std::list<PKBEntity> values;
    for (auto const& [val, ent] : convertAttrRef(attrRef, t)) {
      if (val == sourceArg->getVal()) {
        results.push_back(ent);
      }
    }
  }
  t->update(attrRef->getVal(), results);
  return !t->isTableEmpty();
}

/* ============================= Two syn with ============================= */

void WithEvaluator::evalTwoSynWith(With w, Table* t) const {
  const auto [relArgs] = w.getClauseArgs();
  const std::shared_ptr<AttrRef> lhs =
      std::dynamic_pointer_cast<AttrRef>(relArgs[0]);
  const std::shared_ptr<AttrRef> rhs =
      std::dynamic_pointer_cast<AttrRef>(relArgs[1]);
  std::list<PKBEntityPair> results;
  if (isSpecial(lhs) || isSpecial(rhs)) {
    std::vector<std::pair<std::string, PKBEntity>> lhsValEntPairs =
        convertAttrRef(lhs, t);
    std::vector<std::pair<std::string, PKBEntity>> rhsValEntPairs =
        convertAttrRef(rhs, t);
    for (const auto& [lhsVal, lhsEnt] : lhsValEntPairs) {
      for (const auto& [rhsVal, rhsEnt] : rhsValEntPairs) {
        if (lhsVal == rhsVal) {
          results.push_back({lhsEnt, rhsEnt});
        }
      }
    }
  } else if (t->contains(lhs->getVal()) && t->contains(rhs->getVal())) {
    // Both syn in table
    t->removeIfNotEqual(lhs->getVal(), rhs->getVal());
    return;
  } else {
    std::list<PKBEntity> lhsVals = t->contains(lhs->getVal())
                                       ? t->getPkbEntities(lhs->getVal())
                                       : getAllOfType(lhs->getEntityType());
    std::list<PKBEntity> rhsVals = t->contains(rhs->getVal())
                                       ? t->getPkbEntities(rhs->getVal())
                                       : getAllOfType(rhs->getEntityType());
    for (const auto& i : lhsVals) {
      for (const auto& j : rhsVals) {
        if (i.val == j.val) {
          results.push_back({i, j});
        }
      }
    }
  }
  t->update(lhs->getVal(), rhs->getVal(), results);
}

/* ======================= Helper functions ======================= */

bool WithEvaluator::isSpecial(const std::shared_ptr<AttrRef>& attrRef) {
  return attrRef->getEntityType() == EntityType::Call &&
             attrRef->getAttrName() == AttrName::ProcName ||
         attrRef->getEntityType() == EntityType::Read &&
             attrRef->getAttrName() == AttrName::VarName ||
         attrRef->getEntityType() == EntityType::Print &&
             attrRef->getAttrName() == AttrName::VarName;
}

std::vector<std::pair<std::string, PKBEntity>> WithEvaluator::convertAttrRef(
    const std::shared_ptr<AttrRef>& attrRef, Table* t) const {
  const std::list<PKBEntity> values =
      t->contains(attrRef->getVal()) ? t->getPkbEntities(attrRef->getVal())
                                     : getAllOfType(attrRef->getEntityType());
  std::vector<std::pair<std::string, PKBEntity>> results;
  if (attrRef->getEntityType() == EntityType::Call &&
      attrRef->getAttrName() == AttrName::ProcName) {
    // call.procName
    for (auto const& i : values) {
      const int stmtNo = std::stoi(i.val);
      results.emplace_back(pkb->getProcEntityFromCall(stmtNo).val, i);
    }
  } else if (attrRef->getEntityType() == EntityType::Read &&
             attrRef->getAttrName() == AttrName::VarName) {
    // read.varName
    for (auto const& i : values) {
      const int stmtNo = std::stoi(i.val);
      results.emplace_back(pkb->getVarEntityFromRead(stmtNo).val, i);
    }
  } else if (attrRef->getEntityType() == EntityType::Print &&
             attrRef->getAttrName() == AttrName::VarName) {
    // print.varName
    for (auto const& i : values) {
      const int stmtNo = std::stoi(i.val);
      results.emplace_back(pkb->getVarEntityFromPrint(stmtNo).val, i);
    }
  } else {
    for (auto const& i : values) {
      results.emplace_back(i.val, i);
    }
  }
  return results;
}

std::list<PKBEntity> WithEvaluator::getAllOfType(EntityType type) const {
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
