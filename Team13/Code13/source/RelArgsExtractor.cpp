#include "RelArgsExtractor.h"

#include "PQLExcept.h"
#include "QueryValidator.h"
std::list<PQLList> RelArgsExtractor::extractRelArgs(PQLList* pqlList,
                                                    int numOfArgs) {
  // Extracts relArgs according to the given format
  if (!pqlList->advanceIfMatch(PQLToken::Bracket::LeftBracket)) {
    throwSyntaxError();
  }

  std::list<PQLList> relArgsList;
  for (int i = 0; i < numOfArgs; i++) {
    relArgsList.push_back(extractRelArg(pqlList));

    // We want a comma if there's still args, else we want a right bracket.
    (i == numOfArgs - 1) ? pqlList->require(PQLToken::Bracket::RightBracket)
                         : pqlList->require(PQLToken::Delimiter::Comma);
  }
  return relArgsList;
}

PQLList RelArgsExtractor::extractRelArg(PQLList* pqlList) {
  std::list<Token> relArgList;

  // We need to account for underscores or quotation marks.
  Token endToken = pqlList->popFront();
  relArgList.push_back(endToken);

  // Matching "IDENT" or _"IDENT"_; otherwise only one token is in the relArg
  if (endToken == PQLToken::Delimiter::DoubleQuotes ||
      pqlList->peekFront() == PQLToken::Delimiter::DoubleQuotes) {
    while (!pqlList->advanceIfMatch(endToken)) {
      relArgList.push_back(pqlList->popFront());
    }
    relArgList.push_back(endToken);
  }

  // The std::list is automatically converted to a PQLList once returned
  return PQLList{relArgList};
}

std::list<PQLList> RelArgsExtractor::extractRefs(PQLList* pqlList) {
  // Refs are not enclosed by brackets. Need to parse differently
  std::list<PQLList> relArgsList;
  std::list<Token> refList;
  while (!pqlList->advanceIfMatch(PQLToken::Operator::Equals)) {
    // Extract lhs of with clause
    refList.push_back(pqlList->popFront());
  }
  relArgsList.push_back(PQLList{refList});

  refList.clear();

  // Extract rhs of with clause
  refList.push_back(pqlList->popFront());
  if (refList.front().tokenType == TokenType::Identifier ||
      refList.front().tokenType == TokenType::Delimiter) {
    refList.push_back(pqlList->popFront());
    refList.push_back(pqlList->popFront());
  }

  if (!pqlList->empty() && pqlList->peekFront() == PQLToken::Delimiter::Hashtag) {
    refList.push_back(pqlList->popFront());
  }

  if (!pqlList->empty() && !clauseKeywords.count(pqlList->peekFront())) {
    // The next token, if it exists, should be a clause keyword.
    throwSyntaxError();
  }
  relArgsList.push_back(PQLList{refList});

  return relArgsList;
}

ClauseArgs RelArgsExtractor::extractClauseArgs(
    std::list<PQLList> relArgList, RelRefFormat refTypes,
    std::map<std::string, EntityType>* synDesignEntMap) {
  ClauseArgs clauseArgs;
  auto relArgIt = relArgList.begin();
  auto fmtIt = refTypes.begin();
  for (; relArgIt != relArgList.end(); relArgIt++, fmtIt++) {
    processRelArg(*relArgIt, *fmtIt, &clauseArgs, synDesignEntMap);
  }
  return clauseArgs;
}

void RelArgsExtractor::processRelArg(
    PQLList tokens, RefType refType, ClauseArgs* clauseArgs,
    std::map<std::string, EntityType>* synDesignEntMap) {
  // Tokens should be of valid syntax in this function.
  Token token = tokens.peekFront();
  Token attrName;
  std::string val;
  bool isExact = true;
  std::shared_ptr<Synonym> attrRef;
  switch (refType) {
    case RefType::StmtRef:
      switch (token.tokenType) {
        case TokenType::Identifier:
          clauseArgs->addRelArg(
              extractSyn(new PQLList{tokens}, synDesignEntMap));
          return;
        case TokenType::Number:
          clauseArgs->addRelArg(Number{token.val});
          return;
        case TokenType::Delimiter:
          clauseArgs->addRelArg(Wildcard{});
          return;
        default:
          throwSyntaxError();
      }
    case RefType::EntRef:
      if (tokens.size() == 3) {
        // Tokens are: '"', IDENT, '"'
        tokens.advanceIfFrontBackMatch(PQLToken::Delimiter::DoubleQuotes);
        clauseArgs->addRelArg(SourceArg{tokens.popFront().val});
        return;
      }
      if (tokens.size() > 1) {
        // Remaining cases for entRef should have only 1 token
        throwSyntaxError();
      }
      switch (token.tokenType) {
        case TokenType::Identifier:
          clauseArgs->addRelArg(
              extractSyn(new PQLList{tokens}, synDesignEntMap));
          return;
        case TokenType::Delimiter:
          clauseArgs->addRelArg(Wildcard{});
          return;
        default:
          throwSyntaxError();
      }

    case RefType::ExprSpec:
      if (tokens.size() == 1) {
        clauseArgs->addRelArg(Wildcard{});
        return;
      }
      // Remaining expression-spec is always a sourceArg
      // We strip any quotations or underscores
      if (tokens.advanceIfFrontBackMatch(PQLToken::Delimiter::Underscore)) {
        isExact = false;
      }
      tokens.advanceIfFrontBackMatch(PQLToken::Delimiter::DoubleQuotes);
      // We have to convert to postfix for evaluation
      val = infixToPostfix(tokens);
      clauseArgs->addRelArg(SourceArg{val, isExact});
      return;
    case RefType::WithRef:
      if (tokens.size() == 1) {
        clauseArgs->addRelArg(Number{token.val});
        return;
      }
      switch (token.tokenType) {
        case TokenType::Identifier:
          attrRef = extractSynOrAttrRef(new PQLList{tokens}, synDesignEntMap);
          // We make sure it is an attrRef
          if (attrRef->getRelArgType() == RelArgType::Synonym) {
            throwSyntaxError();
          }
          clauseArgs->addRelArg(attrRef);
          return;
        case TokenType::Delimiter:
          // Wildcard
          tokens.advanceIfFrontBackMatch(PQLToken::Delimiter::DoubleQuotes);
          clauseArgs->addRelArg(SourceArg{tokens.popFront().val});
          return;
        default:
          throwSyntaxError();
      }
    case RefType::Wildcard:
      clauseArgs->addRelArg(Wildcard{});
      return;
    default:
      throwSyntaxError();
  }
}

Synonym RelArgsExtractor::extractSyn(
    PQLList* pqlList, std::map<std::string, EntityType>* synDesignEntMap) {
  Token synToken = pqlList->require(TokenType::Identifier);
  if (synDesignEntMap->count(synToken.val) == 0) {
    throwSemanticError();
  }

  return Synonym{synToken.val, synDesignEntMap->at(synToken.val)};
}

std::shared_ptr<Synonym> RelArgsExtractor::extractSynOrAttrRef(
    PQLList* pqlList, std::map<std::string, EntityType>* synDesignEntMap) {
  // Extracts either a synonym or attrRef. Throws syntax error if anything else.
  Synonym syn = extractSyn(pqlList, synDesignEntMap);

  if (pqlList->empty() ||
      !pqlList->advanceIfMatch(PQLToken::Delimiter::Period)) {
    // Is a synonym
    return std::make_shared<Synonym>(syn);
  }

  // Is AttrRef
  Token attrName = pqlList->require(TokenType::Identifier);
  if (pqlList->advanceIfMatch(PQLToken::Delimiter::Hashtag)) {
    // Account for stmtNo
    attrName.val += "#";
  }
  if (tokenToAttrNameMap.count(attrName) == 0) {
    throwSyntaxError();
  }

  if (!validateSemanticAttrRef(syn.getEntityType(),
                               tokenToAttrNameMap.at(attrName))) {
    throwSemanticError();
  }
  return std::make_shared<AttrRef>(
      AttrRef{syn, tokenToAttrNameMap.at(attrName)});
}

std::list<RelRefFormat> RelArgsExtractor::getRefTypeFormats(
    ClauseType clauseType, Token relRef, EntityType entType) {
  switch (clauseType) {
    case ClauseType::SuchThat:
      return relToRefTypeMap.at(relRef);
    case ClauseType::Pattern:
      switch (entType) {
        case EntityType::Assign:
          return relToRefTypeMap.at(relRef);
        case EntityType::If:
          return {{RefType::EntRef, RefType::Wildcard, RefType::Wildcard}};
        case EntityType::While:
          return {{RefType::EntRef, RefType::Wildcard}};
      }
    case ClauseType::With:
    default:
      throwSyntaxError();
  }
}
