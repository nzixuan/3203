#include "Tokenizer.h"

#include <istream>
#include <list>
#include <string>

#include "Token.h"

Token Tokenizer::getIdentifier(std::istream& istream) {
  std::string identifier;
  while (std::isalnum(istream.peek())) {
    identifier += istream.get();
  }
  return {TokenType::Identifier, identifier};
}
Token Tokenizer::getNumber(std::istream& istream) {
  std::string number;
  while (std::isdigit(istream.peek())) {
    number += istream.get();
  }
  return {TokenType::Number, number};
}

Token Tokenizer::getDelimiter(std::istream& istream) {
  std::string delimiter;
  delimiter += istream.get();
  return {TokenType::Delimiter, delimiter};
}

Token Tokenizer::getOperator(std::istream& istream) {
  std::string opt;
  opt += istream.get();
  return {TokenType::Operator, opt};
}

Token Tokenizer::getPunct(std::istream& istream) {
  std::string val;

  switch (istream.peek()) {
    case '"':
    case '(':
    case ')':
    case '{':
    case '}':
    case ';':
    case ',':
    case '_':
    case '.':
    case '#':
      val += istream.get();
      return {TokenType::Delimiter, val};
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
      val += istream.get();
      return {TokenType::Operator, val};
    case '>':
    case '<':
      val += istream.get();
      if (istream.peek() == '=') {
        val += istream.get();
      }
      return {TokenType::ComparisonOperator, val};
    case '!':
      val += istream.get();
      if (istream.peek() == '=') {
        val += istream.get();
      } else {
        return {TokenType::BooleanOperator, val};
      }
      return {TokenType::ComparisonOperator, val};
    case '=':
      val += istream.get();
      if (istream.peek() == '=') {
        val += istream.get();
      } else {
        return {TokenType::Operator, val};
      }
      return {TokenType::ComparisonOperator, val};
    case '&':
      val += istream.get();
      if (istream.peek() == '&') {
        val += istream.get();
        return {TokenType::BooleanOperator, val};
      } else {
        throw std::logic_error("Expects && but encounter: &" + istream.peek());
      }
    case '|':
      val += istream.get();
      if (istream.peek() == '|') {
        val += istream.get();
        return {TokenType::BooleanOperator, val};
      } else {
        throw std::logic_error("Expects || but encounter: |" + istream.peek());
      }

    default:
      throw std::logic_error("Unknown punctuation encountered: " +
                             istream.peek());
  }
}

std::list<Token> Tokenizer::tokenize(std::istream& istream) {
  std::list<Token> tokens;

  while (istream.peek() != EOF) {
    if (std::isdigit(istream.peek())) {
      tokens.emplace_back(getNumber(istream));
    } else if (std::isalpha(istream.peek())) {
      tokens.emplace_back(getIdentifier(istream));
    } else if (std::ispunct(istream.peek())) {
      tokens.emplace_back(getPunct(istream));
    } else if (std::isspace(istream.peek())) {
      istream.get();
      continue;
    } else {
      throw std::logic_error("Unknown token encountered: " + istream.peek());
    }
  }
  return tokens;
}
