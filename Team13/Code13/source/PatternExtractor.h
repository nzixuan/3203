#pragma once
#include <map>
#include <string>

#include "PQLGrammar.h"
#include "PQLList.h"
#include "Pattern.h"
#include "Query.h"
#include "QueryValidator.h"
#include "RelArgsExtractor.h"

class PatternExtractor {
 public:
  static void extractPattern(PQLList* pqlList,
                             std::map<std::string, EntityType>* synDesignEntMap,
                             Query* query);
};
