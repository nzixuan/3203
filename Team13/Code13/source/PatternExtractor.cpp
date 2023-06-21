#include "PatternExtractor.h"

#include <list>

#include "PQLExcept.h"

void PatternExtractor::extractPattern(
    PQLList* pqlList, std::map<std::string, EntityType>* synDesignEntMap,
    Query* query) {
  std::string key = pqlList->require(TokenType::Identifier).val;

  // Undeclared syn or not a syn for valid pattern
  if (synDesignEntMap->count(key) == 0 ||
      !patternEntSet.count(synDesignEntMap->at(key))) {
    throwSemanticError();
  }

  EntityType entType = synDesignEntMap->at(key);
  std::list<RelRefFormat> possibleFormats;

  possibleFormats = RelArgsExtractor::getRefTypeFormats(
      ClauseType::Pattern, PQLToken::Relationship::Pattern, entType);
  std::list<PQLList> relArgs =
      RelArgsExtractor::extractRelArgs(pqlList, possibleFormats.front().size());

  bool validFormatFound = false;
  ClauseArgs clauseArgs;

  for (const auto& format : possibleFormats) {
    // Check each possible format for lhs and rhs to see if any format is valid.
    if (validateRefType(relArgs, format, synDesignEntMap)) {
      // Using the format, extract the RelArgs
      clauseArgs =
          RelArgsExtractor::extractClauseArgs(relArgs, format, synDesignEntMap);
      validFormatFound = true;
      break;
    }
  }

  if (!validFormatFound) {
    throwSyntaxError();
  }

  if (!validateSemanticPattern(clauseArgs)) {
    throwSemanticError();
  }

  Pattern clause{clauseArgs, Synonym{key, entType}};
  query->getClauseManager()->addClause(clause);
}
