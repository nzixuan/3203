#pragma once
#include <map>
#include <string>

#include "PQLList.h"
#include "PQLToken.h"
#include "Query.h"

class SelectClauseExtractor {
 private:
  static void extractTuple(PQLList* pqlList,
                           std::map<std::string, EntityType>* synDesignEntMap,
                           Query* query);

 public:
  static void extractSelectArgs(
      PQLList* pqlList, std::map<std::string, EntityType>* synDesignEntMap,
      Query* query);
};
