#include "SelectClauseExtractor.h"

#include "PQLExcept.h"
#include "RelArgsExtractor.h"

void SelectClauseExtractor::extractSelectArgs(
    PQLList* pqlList, std::map<std::string, EntityType>* synDesignEntMap,
    Query* query) {
  Token synToken = pqlList->peekFront();

  if (synToken.tokenType == TokenType::Identifier) {
    if (synDesignEntMap->count(synToken.val) != 0) {
      query->addSelectArg(
          RelArgsExtractor::extractSynOrAttrRef(pqlList, synDesignEntMap));
      return;
    } else if (synToken == PQLToken::Boolean) {
      // Only valid undeclared synonym is BOOLEAN
      pqlList->popFront();
      query->setBooleanSelectFlag();
      return;
    } else {
      throwSemanticError();
    }
  } else {
    extractTuple(pqlList, synDesignEntMap, query);
  }
}

void SelectClauseExtractor::extractTuple(
    PQLList* pqlList, std::map<std::string, EntityType>* synDesignEntMap,
    Query* query) {
  pqlList->require(PQLToken::Bracket::LeftAngularBracket);

  TokenType reqType = TokenType::Identifier;
  Token token;
  std::string synName;

  while (!pqlList->advanceIfMatch(PQLToken::Bracket::RightAngularBracket)) {
    switch (reqType) {
      case TokenType::Delimiter:
        if (pqlList->advanceIfMatch(PQLToken::Delimiter::Comma)) {
          // We still have synonyms, so we continue
          reqType = TokenType::Identifier;
          break;
        } else {
          throwSyntaxError();
        }
        break;
      case TokenType::Identifier:
        query->addSelectArg(
            RelArgsExtractor::extractSynOrAttrRef(pqlList, synDesignEntMap));
        reqType = TokenType::Delimiter;
        break;
      default:
        throwSyntaxError();
    }
  }
}
