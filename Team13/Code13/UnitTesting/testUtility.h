#pragma once
#include <list>
#include "QueryParser.h"
#include "PQLToken.h"
#include "Tokenizer.h"
#include "common.h"

const std::set<Token> stmtSet = {
	PQLToken::DesignEntity::Assign,
	PQLToken::DesignEntity::If,
	PQLToken::DesignEntity::Print,
	PQLToken::DesignEntity::Read,
	PQLToken::DesignEntity::Statement,
	PQLToken::DesignEntity::While,
	PQLToken::DesignEntity::Call
};

// Sample synonyms that can be used in tests
static Synonym procSyn{ "p", EntityType::Procedure };
static Synonym stmtSyn{ "s", EntityType::Stmt };
static Synonym readSyn{ "r", EntityType::Read };
static Synonym printSyn{ "pr", EntityType::Print };
static Synonym assignSyn{ "a", EntityType::Assign };
static Synonym callSyn{ "c", EntityType::Call };
static Synonym whileSyn{ "w", EntityType::While };
static Synonym ifSyn{ "ifs", EntityType::If };
static Synonym varSyn{ "v", EntityType::Variable };
static Synonym constSyn{ "const", EntityType::Constant };
static Wildcard wildcard{};
static Number number{ "1" };
static Number number2{ "2" };

// Creates a list of tokens that represents [Relationship](lhs, rhs)
std::list<Token> createRelationshipTokens(Token rel, Token lhs, Token rhs);
// Creates a list representing declarations of one type
std::list<Token> createDeclarations(Token synType, std::list<Token> synList);
std::list<Token> createSelect(Token syn);
void assertQueryEqual(Query expectedQuery, std::list<Token> testTokens);
void addST(Query& query, SuchThat& st);
void addP(Query& query, Pattern& p);
void addW(Query& query, With& w);