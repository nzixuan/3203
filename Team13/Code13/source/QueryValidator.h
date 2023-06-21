#pragma once

#include <list>
#include <map>
#include <string>

#include "Clause.h"
#include "PQLGrammar.h"
#include "PQLList.h"
#include "PQLToken.h"

// Functions to validate different RefTypes

// Calls relevant validator based on passed refType
bool validateRefType(std::list<PQLList> relArgList, RelRefFormat formats,
                     std::map<std::string, EntityType>* synDesignEntMap);
bool validateStmtRef(PQLList tokens,
                     std::map<std::string, EntityType>* synDesignEntMap);
bool validateEntRef(PQLList tokens,
                    std::map<std::string, EntityType>* synDesignEntMap);
bool validateWithRef(PQLList tokens,
                     std::map<std::string, EntityType>* synDesignEntMap);
bool validateExprSpec(PQLList tokens);
bool validateExpr(PQLList tokens);
bool validateSemanticSuchThat(ClauseArgs clauseArgs, RelRefType relRef);
bool validateSemanticPattern(ClauseArgs clauseArgs);
bool validateSemanticWith(ClauseArgs clauseArgs);
bool validateSemanticAttrRef(EntityType entityType, AttrName attrName);
bool validateWildCard(PQLList tokens);
bool validateInteger(Token token);
bool validateIdent(Token token);
