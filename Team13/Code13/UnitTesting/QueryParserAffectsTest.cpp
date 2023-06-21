#include "stdafx.h"
#include "CppUnitTest.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(QueryParserAffectsTest)
	{
	public:
		Tokenizer tokenizer;
		// Template string such that query is valid
		Synonym assign2{ "a2", EntityType::Assign };
		std::string templateStr = "assign " + assignSyn.getVal() + ", " +
			assign2.getVal() + "; Select " + assignSyn.getVal() + " such that ";
		ClauseArgs ca;
		Query expectedQuery;

		TEST_METHOD_INITIALIZE(initializeAffectsQuery) {
			// Reinitalize the query after every method
			expectedQuery = {};
			expectedQuery.addSelectArg(assignSyn);
			ca = ClauseArgs{};
		}
		/* Valid Extractions */
		// W = wildcard '_', S = synonym, I = integer
		// WI = wildcard on lhs, integer on rhs, and so on for the others.
		TEST_METHOD(testExtractAffectsWW) {
			/* Test if we can successfully extract a Affects relationship
			   lhs = '_', rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Affects(_,_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::Affects };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}
		TEST_METHOD(testExtractAffectsWI) {
			/* Test if we can successfully extract a Affects relationship
			   lhs = '_', rhs = integer
			*/
			std::stringstream ss;
			ss << templateStr << "Affects(_," << number.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(number);
			SuchThat expectedSuchThat{ ca, RelRefType::Affects };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractAffectsWS) {
			/* Test if we can successfully extract a Affects relationship
			   lhs = '_', rhs = declared synonym (We use the predeclared assign synonym for convenience)
			*/

			std::stringstream ss;
			ss << templateStr << "Affects(_," << assignSyn.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(assignSyn);
			SuchThat expectedSuchThat{ ca, RelRefType::Affects };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractAffectsIW) {
			/* Test if we can successfully extract a Affects relationship
			   lhs = integer, rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Affects(" << number.getVal() << ", _)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(number);
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::Affects };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractAffectsII) {
			/* Test if we can successfully extract a Affects relationship
			   lhs = integer, rhs = integer
			*/
			std::stringstream ss;
			ss << templateStr << "Affects(" << number.getVal() << "," << number2.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(number);
			ca.addRelArg(number2);
			SuchThat expectedSuchThat{ ca, RelRefType::Affects };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractAffectsIS) {
			/* Test if we can successfully extract a Affects relationship
			   lhs = integer, rhs = declared synonym
			*/

			std::stringstream ss;
			ss << templateStr << "Affects(" << number.getVal() << ", " << assignSyn.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(number);
			ca.addRelArg(assignSyn);
			SuchThat expectedSuchThat{ ca, RelRefType::Affects };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractAffectsSW) {
			/* Test if we can successfully extract a Affects relationship
			   lhs = declared synonym, rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Affects(" << assignSyn.getVal() << ",_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(assignSyn);
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::Affects };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractAffectsSI) {
			/* Test if we can successfully extract a Affects relationship
			   lhs = declared synonym, rhs = integer
			*/
			std::stringstream ss;
			ss << templateStr << "Affects(" << assignSyn.getVal() << "," << number.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(assignSyn);
			ca.addRelArg(number);
			SuchThat expectedSuchThat{ ca, RelRefType::Affects };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractAffectsSS) {
			/* Test if we can successfully extract a Affects relationship
			   lhs = declared synonym, rhs = declared synonym
			*/
			std::stringstream ss;
			ss << templateStr << "Affects(" << assignSyn.getVal() << "," << assign2.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(assignSyn);
			ca.addRelArg(assign2);
			SuchThat expectedSuchThat{ ca, RelRefType::Affects };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractAffectsStmtSubtypes) {
			/* Tests if all stmt subtypes are valid as arguments
			*/
			std::map<std::string, EntityType> testMap;
			std::string val = "syn";
			for (const auto& stmt : stmtSet) {

				std::stringstream ss;
				ss << stmt.val << " " << val << ";" << "Select "
					<< val << " such that Affects(" << val << ",_)";

				std::list<Token> testTokens = tokenizer.tokenize(ss);
				testMap = std::map<std::string, EntityType>{
					{val, tokenToEntMap.at(stmt)}
				};

				// Reset the query
				expectedQuery = {};
				expectedQuery.addSelectArg(Synonym{ val, tokenToEntMap.at(stmt) });
				ClauseArgs ca{};

				ca.addRelArg(Synonym{ val, tokenToEntMap.at(stmt) });
				ca.addRelArg(wildcard);
				SuchThat expectedSuchThat{ ca, RelRefType::Affects };

				addST(expectedQuery, expectedSuchThat);

				assertQueryEqual(expectedQuery, testTokens);
			}
		}

		/* Invalid extractions */
		TEST_METHOD(testExtractAffectsInvalidLhs) {
			std::stringstream ss;
			ss << templateStr << "Affects(\"invalidSyn\", _)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractAffectsInvalidRhs) {
			std::stringstream ss;
			ss << templateStr << "Affects(_, \"invalidSyn\")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractAffectsUndeclaredLhs) {
			std::stringstream ss;
			ss << templateStr << "Affects(undeclaredSyn, _)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractAffectsUndeclaredRhs) {
			std::stringstream ss;
			ss << templateStr << "Affects(_, undeclaredSyn)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		// Affects with invalid synonym types
		TEST_METHOD(testExtractAffectsWrongSynType) {
			const std::set<Token> invalidSynSet = {
				PQLToken::DesignEntity::Constant,
				PQLToken::DesignEntity::Procedure,
				PQLToken::DesignEntity::Variable
			};
			std::string val = "syn";
			for (const auto& syn : invalidSynSet) {
				std::stringstream ss;
				ss << syn.val << " " << val << ";" << "Select "
					<< val << " such that Affects(" << val << ",_)";
				std::list<Token> testTokens = tokenizer.tokenize(ss);

				auto func = [=] {
					QueryParser parser(testTokens);
					parser.parseQuery();
				};
				Assert::ExpectException<std::logic_error>(func);
			};
		}
	};
}