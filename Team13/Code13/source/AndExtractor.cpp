#include "AndExtractor.h"

#include "PQLExcept.h"

void AndExtractor::extractAnd(
    ClauseType clauseType, PQLList* pqlList,
    std::map<std::string, EntityType>* synDesignEntMap, Query* query) {
  switch (clauseType) {
    case ClauseType::SuchThat:
      SuchThatExtractor::extractSuchThat(pqlList, synDesignEntMap, query);
      break;
    case ClauseType::Pattern:
      PatternExtractor::extractPattern(pqlList, synDesignEntMap, query);
      break;
    case ClauseType::With:
      WithExtractor::extractWith(pqlList, synDesignEntMap, query);
      break;
    default:
      throwSyntaxError();
  }
}
