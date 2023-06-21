#pragma once
#include <list>
#include <utility>

#include "PQLToken.h"

/* Helper class to iterate through list of tokens */
class PQLList {
 private:
  std::list<Token> tokens;

 public:
  explicit PQLList(std::list<Token> tokens);

  // Used to increment the iterator whilst expecting a specific Token/TokenType
  Token require(Token token);
  Token require(TokenType tokenType);

  // Pops the top Token if it matches the given Token and returns true. Does
  // nothing otherwise.
  bool advanceIfMatch(Token token);
  /*
  Pops the frontand back token if they both match the given token and returns
  true. Front and back should not be the same token. */
  bool advanceIfFrontBackMatch(Token token);
  Token peekFront();
  Token peekBack();
  bool empty();
  int size();

  // Pop methods
  Token popFront();
  Token popBack();
  std::pair<Token, Token> popFrontAndBack();
};
