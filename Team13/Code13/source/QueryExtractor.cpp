#include "QueryExtractor.h"

#include "AndExtractor.h"
#include "DeclarationExtractor.h"
#include "PatternExtractor.h"
#include "RelArgsExtractor.h"
#include "SelectClauseExtractor.h"
#include "SuchThatExtractor.h"
#include "WithExtractor.h"

void QueryExtractor::extractDeclaration(PQLList* pqlList) {
  DeclarationExtractor::extractDeclaration(pqlList, &synDesignEntMap);
}

void QueryExtractor::extractSelectArgs(PQLList* pqlList, Query* query) {
  SelectClauseExtractor::extractSelectArgs(pqlList, &synDesignEntMap, query);
}

void QueryExtractor::extractSuchThat(PQLList* pqlList, Query* query) {
  SuchThatExtractor::extractSuchThat(pqlList, &synDesignEntMap, query);
}

void QueryExtractor::extractPattern(PQLList* pqlList, Query* query) {
  PatternExtractor::extractPattern(pqlList, &synDesignEntMap, query);
}

void QueryExtractor::extractWith(PQLList* pqlList, Query* query) {
  WithExtractor::extractWith(pqlList, &synDesignEntMap, query);
}

void QueryExtractor::extractAnd(ClauseType clauseType, PQLList* pqlList,
                                Query* query) {
  AndExtractor::extractAnd(clauseType, pqlList, &synDesignEntMap, query);
}

std::map<std::string, EntityType> QueryExtractor::getSynMap() {
  return synDesignEntMap;
}
