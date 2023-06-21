#pragma once
#include <map>
#include <string>

#include "PQLGrammar.h"
#include "PQLList.h"
#include "PQLToken.h"
#include "Query.h"
#include "QueryValidator.h"
#include "RelArgsExtractor.h"
#include "SuchThat.h"

class SuchThatExtractor {
 public:
  static void extractSuchThat(
      PQLList* pqlList, std::map<std::string, EntityType>* synDesignEntMap,
      Query* query);
};
