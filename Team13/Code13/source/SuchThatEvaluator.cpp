#include "SuchThatEvaluator.h"

SuchThatEvaluator::SuchThatEvaluator(Query query, PKB* pkb)
    : query(query), pkb(pkb) {}

bool SuchThatEvaluator::evalSuchThatClause(SuchThat s, Table* t) {
  const ClauseArgs ca = s.getClauseArgs();
  const std::shared_ptr<RelArg> lhs = ca.relArgs[0];
  const std::shared_ptr<RelArg> rhs = ca.relArgs[1];
  if (!s.isUsingSynonyms()) {
    // Query does not have synonyms (boolean result)
    return evalBoolSuchThat(s);
  } else if (lhs->getRelArgType() == RelArgType::Synonym &&
             rhs->getRelArgType() == RelArgType::Synonym) {
    // Query has 2 synonyms
    if (*lhs == *rhs) {
      Synonym syn = *std::dynamic_pointer_cast<Synonym>(lhs);
      std::list<PKBEntity> res =
          pkb->getLhsRhsSameSynObj(syn.getEntityType(), s.getRelationship());
      t->update(syn.getVal(), res);
    } else {
      std::list<PKBEntityPair> res = evalTwoSynSuchThat(s);
      t->update(lhs->getVal(), rhs->getVal(), res);
    }
  } else {
    // Query has 1 synonym and 1 constant
    std::string syn = lhs->getRelArgType() == RelArgType::Synonym
                          ? lhs->getVal()
                          : rhs->getVal();
    std::list<PKBEntity> res = evalOneSynSuchThat(s);
    t->update(syn, res);
  }
  return !t->isTableEmpty();
}

bool SuchThatEvaluator::evalBoolSuchThat(SuchThat s) {
  const ClauseArgs ca = s.getClauseArgs();
  const std::shared_ptr<RelArg> lhs = ca.relArgs[0];
  const std::shared_ptr<RelArg> rhs = ca.relArgs[1];
  const RelRefType relRef = s.getRelationship();
  switch (lhs->getRelArgType()) {
    case RelArgType::Wildcard:
      switch (rhs->getRelArgType()) {
        case RelArgType::Wildcard:
          return pkb->hasLhsWildcardRhsWildcardForRel(relRef);
        case RelArgType::Number:
          return pkb->hasLhsWildcardRhsConstantForRel(
              {rhs->getVal(), QueryConstantType::Number}, relRef);
        case RelArgType::SourceArg:
          return pkb->hasLhsWildcardRhsConstantForRel(
              {rhs->getVal(), QueryConstantType::Identifier}, relRef);
      }
    case RelArgType::Number:
      switch (rhs->getRelArgType()) {
        case RelArgType::Wildcard:
          return pkb->hasLhsConstantRhsWildcardForRel(
              {lhs->getVal(), QueryConstantType::Number}, relRef);
        case RelArgType::Number:
          return pkb->hasLhsConstantRhsConstantForRel(
              {lhs->getVal(), QueryConstantType::Number},
              {rhs->getVal(), QueryConstantType::Number}, relRef);
        case RelArgType::SourceArg:
          return pkb->hasLhsConstantRhsConstantForRel(
              {lhs->getVal(), QueryConstantType::Number},
              {rhs->getVal(), QueryConstantType::Identifier}, relRef);
      }
    case RelArgType::SourceArg:
      switch (rhs->getRelArgType()) {
        case RelArgType::Wildcard:
          return pkb->hasLhsConstantRhsWildcardForRel(
              {lhs->getVal(), QueryConstantType::Identifier}, relRef);
        case RelArgType::Number:
          return pkb->hasLhsConstantRhsConstantForRel(
              {lhs->getVal(), QueryConstantType::Identifier},
              {rhs->getVal(), QueryConstantType::Number}, relRef);
        case RelArgType::SourceArg:
          return pkb->hasLhsConstantRhsConstantForRel(
              {lhs->getVal(), QueryConstantType::Identifier},
              {rhs->getVal(), QueryConstantType::Identifier}, relRef);
      }
  }
  return false;
}

std::list<PKBEntity> SuchThatEvaluator::evalOneSynSuchThat(SuchThat s) {
  const ClauseArgs ca = s.getClauseArgs();
  const std::shared_ptr<RelArg> lhs = ca.relArgs[0];
  const std::shared_ptr<RelArg> rhs = ca.relArgs[1];
  const std::string lhsVal = lhs->getVal();
  const std::string rhsVal = rhs->getVal();
  const RelRefType relRef = s.getRelationship();
  if (lhs->getRelArgType() == RelArgType::Synonym) {
    const EntityType lhsEntType =
        std::dynamic_pointer_cast<Synonym>(lhs)->getEntityType();
    // LHS synonym
    switch (rhs->getRelArgType()) {
      case RelArgType::Wildcard:
        return pkb->getLhsSynRhsWildcardForRel(lhsEntType, relRef);
      case RelArgType::Number:
        return pkb->getLhsSynRhsConstantForRel(
            lhsEntType, {rhsVal, QueryConstantType::Number}, relRef);
      case RelArgType::SourceArg:
        return pkb->getLhsSynRhsConstantForRel(
            lhsEntType, {rhsVal, QueryConstantType::Identifier}, relRef);
    }
  } else {
    // RHS synonym
    const EntityType rhsEntType =
        std::dynamic_pointer_cast<Synonym>(rhs)->getEntityType();
    switch (lhs->getRelArgType()) {
      case RelArgType::Wildcard:
        return pkb->getLhsWildcardRhsSynForRel(rhsEntType, relRef);
      case RelArgType::Number:
        return pkb->getLhsConstantRhsSynForRel(
            {lhsVal, QueryConstantType::Number}, rhsEntType, relRef);
      case RelArgType::SourceArg:
        return pkb->getLhsConstantRhsSynForRel(
            {lhsVal, QueryConstantType::Identifier}, rhsEntType, relRef);
    }
  }
  return std::list<PKBEntity>{};
}

std::list<PKBEntityPair> SuchThatEvaluator::evalTwoSynSuchThat(SuchThat s) {
  const ClauseArgs ca = s.getClauseArgs();
  const std::shared_ptr<RelArg> lhs = ca.relArgs[0];
  const std::shared_ptr<RelArg> rhs = ca.relArgs[1];
  const EntityType lhsEntType =
      std::dynamic_pointer_cast<Synonym>(lhs)->getEntityType();
  const EntityType rhsEntType =
      std::dynamic_pointer_cast<Synonym>(rhs)->getEntityType();
  return pkb->getLhsSynRhsSynForRel(lhsEntType, rhsEntType,
                                    s.getRelationship());
}
