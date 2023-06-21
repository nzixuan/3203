#include "DeclarationExtractor.h"

#include "PQLExcept.h"

void DeclarationExtractor::extractDeclaration(
    PQLList* pqlList, std::map<std::string, EntityType>* synDesignEntMap) {
  Token entToken = pqlList->require(TokenType::Identifier);

  if (tokenToEntMap.count(entToken) == 0) {
    // Invalid design entity
    throwSyntaxError();
  }

  EntityType entity = tokenToEntMap.at(entToken);

  /* Extraction of synonyms follow pattern of
     Identifier -> Delimiter -> Identifier -> Delimiter
  */
  TokenType nextType = TokenType::Identifier;
  Token token;
  std::string synName;

  // Extract the synonyms
  while (!pqlList->empty()) {
    token = pqlList->require(nextType);
    switch (token.tokenType) {
      case TokenType::Delimiter:
        if (token == PQLToken::Delimiter::Comma) {
          // We still have synonyms, so we continue
          nextType = TokenType::Identifier;
          break;
        } else if (token == PQLToken::Delimiter::Semicolon) {
          // We reached the end of this set of declarations.
          return;
        } else {
          throwSyntaxError();
        }
        break;
      case TokenType::Identifier:
        synName = token.val;
        if (synDesignEntMap->count(synName)) {
          throwSemanticError();
        }
        (*synDesignEntMap)[synName] = entity;
        nextType = TokenType::Delimiter;
        break;
      default:
        throwSyntaxError();
        return;
    }
  }

  // Tokens ended unexpectedly; should not reach here if syntax is correct.
  throwSyntaxError();
}
