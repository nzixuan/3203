#pragma once
#include <map>
#include <string>

#include "PQLGrammar.h"
#include "PQLList.h"
#include "Query.h"

// This is the main extractor class
class QueryExtractor {
 private:
  std::map<std::string, EntityType> synDesignEntMap;

 public:
  void extractDeclaration(PQLList* pqlList);
  void extractSelectArgs(PQLList* pqlList, Query* query);
  void extractSuchThat(PQLList* pqlList, Query* query);
  void extractPattern(PQLList* pqlList, Query* query);
  void extractWith(PQLList* pqlList, Query* query);
  void extractAnd(ClauseType clauseType, PQLList* pqlList, Query* query);

  // Getter
  std::map<std::string, EntityType> getSynMap();
};
