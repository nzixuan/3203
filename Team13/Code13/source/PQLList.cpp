#include "PQLList.h"

#include "PQLExcept.h"

PQLList::PQLList(std::list<Token> tokens) : tokens(tokens) {}

Token PQLList::require(Token token) {
  if (empty() || peekFront() != token) {
    throwSyntaxError();
  }
  return popFront();
}

Token PQLList::require(TokenType tokenType) {
  if (empty() || peekFront().tokenType != tokenType) {
    throwSyntaxError();
  }
  return popFront();
}

bool PQLList::advanceIfMatch(Token token) {
  if (size() == 0) return false;
  if (peekFront() == token) {
    popFront();
    return true;
  }
  return false;
}

bool PQLList::advanceIfFrontBackMatch(Token token) {
  // Front and back token should not be the same token
  if (tokens.size() < 2) {
    return false;
  }

  if (peekFront() == token && peekBack() == token) {
    popFrontAndBack();
    return true;
  }
  return false;
}

/*
        Methods to help retrieve information about the list.
*/
Token PQLList::peekFront() {
  if (empty()) {
    throwSyntaxError();
  }
  return tokens.front();
}

Token PQLList::peekBack() {
  if (empty()) {
    throwSyntaxError();
  }
  return tokens.back();
}

int PQLList::size() { return tokens.size(); }

bool PQLList::empty() { return tokens.empty(); }

Token PQLList::popFront() {
  if (empty()) {
    throwSyntaxError();
  }
  Token token = tokens.front();
  tokens.pop_front();
  return token;
}

Token PQLList::popBack() {
  if (empty()) {
    throwSyntaxError();
  }
  Token token = tokens.back();
  tokens.pop_back();
  return token;
}

std::pair<Token, Token> PQLList::popFrontAndBack() {
  if (tokens.size() < 2) {
    throwSyntaxError();
  }

  Token front = tokens.front();
  tokens.pop_front();
  Token back = tokens.back();
  tokens.pop_back();

  return {front, back};
}
