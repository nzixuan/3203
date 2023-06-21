#pragma once
#include <map>
#include <string>

#include "PQLGrammar.h"
#include "PatternExtractor.h"
#include "SuchThatExtractor.h"
#include "WithExtractor.h"
class AndExtractor {
 public:
  static void extractAnd(ClauseType clauseType, PQLList* pqlList,
                         std::map<std::string, EntityType>* synDesignEntMap,
                         Query* query);
};
