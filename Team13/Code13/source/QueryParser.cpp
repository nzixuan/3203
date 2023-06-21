#include "QueryParser.h"

#include "PQLExcept.h"

QueryParser::QueryParser(std::list<Token> tokens) : pqlList(tokens) {}

Query QueryParser::parseQuery() {
  if (pqlList.empty()) {
    // Empty list to parse
    throwSyntaxError();
  }

  /* Recursively extract all declarations
   *  Does not execute if there are no declarations
   */
  while (!pqlList.advanceIfMatch(PQLToken::Select)) {
    queryExtractor.extractDeclaration(&pqlList);
  }

  // Extract the synonym to select
  queryExtractor.extractSelectArgs(&pqlList, &query);

  ClauseType prevClause = ClauseType::None;
  while (!pqlList.empty()) {
    if (pqlList.advanceIfMatch(PQLToken::Clause::Such)) {
      pqlList.require(PQLToken::Clause::That);
      // Extract one such that clause
      queryExtractor.extractSuchThat(&pqlList, &query);
      prevClause = ClauseType::SuchThat;
    } else if (pqlList.advanceIfMatch(PQLToken::Clause::Pattern)) {
      // Extract pattern
      queryExtractor.extractPattern(&pqlList, &query);
      prevClause = ClauseType::Pattern;
    } else if (pqlList.advanceIfMatch(PQLToken::Clause::With)) {
      queryExtractor.extractWith(&pqlList, &query);
      prevClause = ClauseType::With;
    } else if (pqlList.advanceIfMatch(PQLToken::Clause::And)) {
      queryExtractor.extractAnd(prevClause, &pqlList, &query);
    } else {
      throwSyntaxError();
    }
  }

  query.finalize();
  return query;
}

// Getter functions
PQLList QueryParser::getPQLList() { return pqlList; }

Query QueryParser::getQuery() { return query; }
