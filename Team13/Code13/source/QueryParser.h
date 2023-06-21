#pragma once

#include <list>

// Include the other utility functions
#include "PQLList.h"
#include "PQLToken.h"
#include "Query.h"
#include "QueryExtractor.h"
#include "QueryValidator.h"

class QueryParser {
 private:
  PQLList pqlList;
  Query query;
  QueryExtractor queryExtractor;

 public:
  explicit QueryParser(std::list<Token> tokens);

  // Main API function
  Query parseQuery();

  // Getter functions

  PQLList getPQLList();
  Query getQuery();
};
