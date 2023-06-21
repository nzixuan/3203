#pragma once
#include <map>
#include <string>

#include "PQLGrammar.h"
#include "PQLList.h"
#include "PQLToken.h"
#include "Query.h"
#include "QueryValidator.h"
#include "RelArgsExtractor.h"
#include "With.h"

class WithExtractor {
 public:
  static void extractWith(PQLList* pqlList,
                          std::map<std::string, EntityType>* synDesignEntMap,
                          Query* query);
};
