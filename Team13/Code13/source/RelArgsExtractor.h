#pragma once
#include <list>
#include <map>
#include <memory>
#include <string>

#include "Clause.h"
#include "PQLGrammar.h"
#include "PQLList.h"
#include "PQLUtil.h"

class RelArgsExtractor {
 private:
  static PQLList extractRelArg(PQLList* pqlList);
  static void processRelArg(PQLList tokens, RefType refType,
                            ClauseArgs* clauseArgs,
                            std::map<std::string, EntityType>* synDesignEntMap);

 public:
  static std::list<PQLList> extractRelArgs(PQLList* pqlList, int numOfArgs);
  static ClauseArgs extractClauseArgs(
      std::list<PQLList> relArgList, RelRefFormat refTypes,
      std::map<std::string, EntityType>* synDesignEntMap);
  static std::shared_ptr<Synonym> extractSynOrAttrRef(
      PQLList* pqlList, std::map<std::string, EntityType>* synDesignEntMap);
  static std::list<PQLList> extractRefs(PQLList* pqlList);
  static Synonym extractSyn(PQLList* pqlList,
                            std::map<std::string, EntityType>* synDesignEntMap);
  static std::list<RelRefFormat> getRefTypeFormats(
      ClauseType clauseType, Token relRef,
      EntityType entType = EntityType::Assign);
};
