#pragma once

#include <istream>
#include <list>

#include "Token.h"

class Tokenizer {
 private:
  Token getIdentifier(std::istream& istream);
  Token getNumber(std::istream& istream);
  Token getDelimiter(std::istream& istream);
  Token getOperator(std::istream& istream);

  Token getPunct(std::istream& istream);

 public:
  std::list<Token> tokenize(std::istream& istream);
};
