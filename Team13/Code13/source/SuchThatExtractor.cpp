#include "SuchThatExtractor.h"

#include <list>

#include "PQLExcept.h"

void SuchThatExtractor::extractSuchThat(
    PQLList* pqlList, std::map<std::string, EntityType>* synDesignEntMap,
    Query* query) {
  Token relRef = pqlList->require(TokenType::Identifier);

  // Transitive version of relRef detected
  if (pqlList->advanceIfMatch(PQLToken::Operator::Star)) {
    relRef.val = relRef.val + "*";
  }

  if (relRefSet.count(relRef) == 0) {
    throwSyntaxError();
  }

  std::list<RelRefFormat> possibleFormats =
      RelArgsExtractor::getRefTypeFormats(ClauseType::SuchThat, relRef);
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

  RelRefType relRefEnum = tokenToEnumRRMap.at(relRef);
  if (!validateSemanticSuchThat(clauseArgs, relRefEnum)) {
    throwSemanticError();
  }

  SuchThat clause{{clauseArgs}, relRefEnum};
  query->getClauseManager()->addClause(clause);
}
