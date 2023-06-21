#pragma once
#include <map>
#include <string>

#include "PQLGrammar.h"
#include "PQLList.h"
#include "PQLToken.h"

class DeclarationExtractor {
 public:
  static void extractDeclaration(
      PQLList* pqlList, std::map<std::string, EntityType>* synDesignEntMap);
};
