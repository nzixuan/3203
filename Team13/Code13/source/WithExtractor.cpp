#include "WithExtractor.h"

#include <list>

#include "PQLExcept.h"

void WithExtractor::extractWith(
    PQLList* pqlList, std::map<std::string, EntityType>* synDesignEntMap,
    Query* query) {
  std::list<RelRefFormat> possibleFormats =
      relToRefTypeMap.at(PQLToken::Relationship::With);
  std::list<PQLList> relArgs = RelArgsExtractor::extractRefs(pqlList);

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

  if (!validateSemanticWith(clauseArgs)) {
    throwSemanticError();
  }

  WithRefType wrType = getWithRefType(clauseArgs.relArgs[0]);
  With clause{clauseArgs, wrType};
  query->getClauseManager()->addClause(clause);
}
