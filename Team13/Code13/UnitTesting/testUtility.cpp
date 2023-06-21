#include "stdafx.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

std::list<Token> createRelationshipTokens(Token rel, Token lhs, Token rhs) {
	return {
			rel,
			PQLToken::Bracket::LeftBracket,
			lhs,
			PQLToken::Delimiter::Comma,
			rhs,
			PQLToken::Bracket::RightBracket
	};
}

std::list<Token> createDeclarations(Token synType, std::list<Token> synList) {
	std::list<Token> res;
	res.push_back(synType);
	for (const auto& syn : synList) {
		res.push_back(syn);
		res.push_back(PQLToken::Delimiter::Comma);
	}

	// Replace the last comma with a semicolon
	res.pop_back();
	res.push_back(PQLToken::Delimiter::Semicolon);
	return res;
}

std::list<Token> createSelect(Token syn) {
	return {
			PQLToken::Select,
			syn
	};
}

void addST(Query& query, SuchThat& st) {
	query.getClauseManager()->addClause(st);
}

void addP(Query& query, Pattern& p) {
	query.getClauseManager()->addClause(p);
}

void addW(Query& query, With& w) {
	query.getClauseManager()->addClause(w);
}

void assertQueryEqual(Query expectedQuery, std::list<Token> testTokens) {
	expectedQuery.finalize();
	QueryParser parser(testTokens);
	Query actualQuery = parser.parseQuery();

	Assert::IsTrue(expectedQuery == actualQuery);
};