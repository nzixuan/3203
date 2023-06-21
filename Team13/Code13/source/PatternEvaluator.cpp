#include "PatternEvaluator.h"

PatternEvaluator::PatternEvaluator(PKB* pkb) : pkb(pkb) {}

void PatternEvaluator::evalPatternClause(Pattern p, Table* t) const {
  ClauseArgs ca = p.getClauseArgs();
  std::shared_ptr<RelArg> lhs = ca.relArgs[0];
  if (p.getPatternSyn().getEntityType() == EntityType::Assign) {
    if (lhs->getRelArgType() == RelArgType::Synonym) {
      evalTwoSynAssignPattern(p, t);
    } else {
      evalOneSynAssignPattern(p, t);
    }
  } else {
    if (lhs->getRelArgType() == RelArgType::Synonym) {
      evalTwoSynIfWhilePattern(p, t);
    } else {
      evalOneSynIfWhilePattern(p, t);
    }
  }
}

void PatternEvaluator::evalOneSynAssignPattern(Pattern p, Table* t) const {
  const ClauseArgs ca = p.getClauseArgs();
  const std::shared_ptr<RelArg> lhs = ca.relArgs[0];
  const std::shared_ptr<RelArg> rhs = ca.relArgs[1];
  const std::string patternSyn = p.getPatternSyn().getVal();
  std::list<PKBEntity> res;
  bool isExactMatch;
  if (lhs->getRelArgType() == RelArgType::Wildcard) {
    if (rhs->getRelArgType() == RelArgType::Wildcard) {
      // Case: pattern a (_, _)
      res = pkb->getPatternAssignLhsWildcardRhsWildcard();
    } else if (rhs->getRelArgType() == RelArgType::SourceArg) {
      isExactMatch = std::dynamic_pointer_cast<SourceArg>(rhs)->getIsExact();
      // Case: pattern a (_, _"x + y"_)
      res = pkb->getPatternAssignLhsWildcardRhsExp(rhs->getVal(), isExactMatch);
    }
  } else if (lhs->getRelArgType() == RelArgType::SourceArg) {
    if (rhs->getRelArgType() == RelArgType::Wildcard) {
      // Case: pattern a ("x", _)
      res = pkb->getPatternAssignLhsConstRhsWildcard(lhs->getVal());
    } else if (rhs->getRelArgType() == RelArgType::SourceArg) {
      isExactMatch = std::dynamic_pointer_cast<SourceArg>(rhs)->getIsExact();
      // Case: pattern a ("x", _"x + y"_)
      res = pkb->getPatternAssignLhsConstRhsExp(lhs->getVal(), rhs->getVal(),
                                                isExactMatch);
    }
  }
  t->update(patternSyn, res);
}

void PatternEvaluator::evalTwoSynAssignPattern(Pattern p, Table* t) const {
  const std::string patternSyn = p.getPatternSyn().getVal();
  const ClauseArgs ca = p.getClauseArgs();
  const std::shared_ptr<RelArg> lhs = ca.relArgs[0];
  const std::shared_ptr<RelArg> rhs = ca.relArgs[1];
  const std::string varSyn = lhs->getVal();
  std::list<PKBEntityPair> res;
  if (rhs->getRelArgType() == RelArgType::Wildcard) {
    // Case: pattern a (v, _)
    res = pkb->getPatternAssignLhsSynRhsWildcard();
  } else if (rhs->getRelArgType() == RelArgType::SourceArg) {
    // Case: pattern a (v, _"x + y"_)
    const bool isExactMatch =
        std::dynamic_pointer_cast<SourceArg>(rhs)->getIsExact();
    res = pkb->getPatternAssignLhsSynRhsExp(rhs->getVal(), isExactMatch);
  }
  t->update(patternSyn, varSyn, res);
}

void PatternEvaluator::evalOneSynIfWhilePattern(Pattern p, Table* t) const {
  const std::string patternSyn = p.getPatternSyn().getVal();
  const EntityType entityType = p.getPatternSyn().getEntityType();
  ClauseArgs ca = p.getClauseArgs();
  std::shared_ptr<RelArg> lhs = ca.relArgs[0];
  std::list<PKBEntity> res;
  if (lhs->getRelArgType() == RelArgType::Wildcard) {
    res = pkb->getPatternWildcard(entityType);
  } else if (lhs->getRelArgType() == RelArgType::SourceArg) {
    res = pkb->getPatternConst(lhs->getVal(), entityType);
  }
  t->update(patternSyn, res);
}

void PatternEvaluator::evalTwoSynIfWhilePattern(Pattern p, Table* t) const {
  const std::string patternSyn = p.getPatternSyn().getVal();
  const EntityType entityType = p.getPatternSyn().getEntityType();
  ClauseArgs ca = p.getClauseArgs();
  std::shared_ptr<RelArg> lhs = ca.relArgs[0];
  const std::string varSyn = lhs->getVal();
  std::list<PKBEntityPair> res = pkb->getPatternSyn(entityType);
  t->update(patternSyn, varSyn, res);
}
