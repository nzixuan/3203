#pragma once

#include <string>

enum class TokenType {
  Identifier,
  Number,
  Operator,
  BooleanOperator,
  ComparisonOperator,
  Delimiter,
};

struct Token {
  TokenType tokenType;
  std::string val;

  friend bool operator<(const Token& lhs, const Token& rhs) {
    return lhs.val < rhs.val;
  }

  friend bool operator>(const Token& lhs, const Token& rhs) {
    return rhs.val < lhs.val;
  }

  friend bool operator<=(const Token& lhs, const Token& rhs) {
    return !(rhs.val < lhs.val);
  }

  friend bool operator>=(const Token& lhs, const Token& rhs) {
    return !(lhs.val < rhs.val);
  }

  friend bool operator==(const Token& lhs, const Token& rhs) {
    return lhs.tokenType == rhs.tokenType && lhs.val == rhs.val;
  }

  friend bool operator!=(const Token& lhs, const Token& rhs) {
    return !(lhs == rhs);
  }
};
