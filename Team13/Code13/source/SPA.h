#pragma once

#include <list>
#include <string>

#include "PKB.h"
#include "QueryEvaluator.h"
#include "QueryParser.h"
#include "Tokenizer.h"

class SPA {
 private:
  Tokenizer tokenizer;
  PKB pkb;

 public:
  SPA();
  void parseSIMPLEFile(std::string filename);
  void parsePQL(std::string query, std::list<std::string>& results);
};
