#include "stdafx.h"
#include "CppUnitTest.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(QueryParserNextTest)
	{
	public:
		Tokenizer tokenizer;
		// Template string such that query is valid
		std::string templateStr = "assign " + assignSyn.getVal() + "; read " +
			readSyn.getVal() + "; Select " + assignSyn.getVal() + " such that ";
		ClauseArgs ca;

		Query expectedQuery;

		TEST_METHOD_INITIALIZE(initializeNextQuery) {
			// Reinitalize the query after every method
			expectedQuery = {};
			
			expectedQuery.addSelectArg(assignSyn);
			ca = ClauseArgs{};
		}
		/* Valid Extractions */
		// W = wildcard '_', S = synonym, I = integer
		// WI = wildcard on lhs, integer on rhs, and so on for the others.
		TEST_METHOD(testExtractNextWW) {
			/* Test if we can successfully extract a Next relationship
			   lhs = '_', rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Next(_,_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::Next };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}
		TEST_METHOD(testExtractNextWI) {
			/* Test if we can successfully extract a Next relationship
			   lhs = '_', rhs = integer
			*/
			std::stringstream ss;
			ss << templateStr << "Next(_," << number.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(number);
			SuchThat expectedSuchThat{ ca, RelRefType::Next };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractNextWS) {
			/* Test if we can successfully extract a Next relationship
			   lhs = '_', rhs = declared synonym (We use the predeclared assign synonym for convenience)
			*/

			std::stringstream ss;
			ss << templateStr << "Next(_," << assignSyn.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(assignSyn);
			SuchThat expectedSuchThat{ ca, RelRefType::Next };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractNextIW) {
			/* Test if we can successfully extract a Next relationship
			   lhs = integer, rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Next(" << number.getVal() << ", _)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(number);
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::Next };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractNextII) {
			/* Test if we can successfully extract a Next relationship
			   lhs = integer, rhs = integer
			*/
			std::stringstream ss;
			ss << templateStr << "Next(" << number.getVal() << "," << number2.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(number);
			ca.addRelArg(number2);
			SuchThat expectedSuchThat{ ca, RelRefType::Next };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractNextIS) {
			/* Test if we can successfully extract a Next relationship
			   lhs = integer, rhs = declared synonym
			*/

			std::stringstream ss;
			ss << templateStr << "Next(" << number.getVal() << "," << assignSyn.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(number);
			ca.addRelArg(assignSyn);
			SuchThat expectedSuchThat{ ca, RelRefType::Next };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractNextSW) {
			/* Test if we can successfully extract a Next relationship
			   lhs = declared synonym, rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Next(" << assignSyn.getVal() << ",_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(assignSyn);
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::Next };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractNextSI) {
			/* Test if we can successfully extract a Next relationship
			   lhs = declared synonym, rhs = integer
			*/
			std::stringstream ss;
			ss << templateStr << "Next(" << assignSyn.getVal() << "," << number.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(assignSyn);
			ca.addRelArg(number);
			SuchThat expectedSuchThat{ ca, RelRefType::Next };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractNextSS) {
			/* Test if we can successfully extract a Next relationship
			   lhs = declared synonym, rhs = declared synonym
			*/
			std::stringstream ss;
			ss << templateStr << "Next(" << assignSyn.getVal() << "," << readSyn.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(assignSyn);
			ca.addRelArg(readSyn);
			SuchThat expectedSuchThat{ ca, RelRefType::Next };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractNextStmtSubtypes) {
			/* Tests if all stmt subtypes are valid as arguments
			*/
			std::map<std::string, EntityType> testMap;
			std::string val = "syn";
			for (const auto& stmt : stmtSet) {

				std::stringstream ss;
				ss << stmt.val << " " << val << ";" << "Select "
					<< val << " such that Next(" << val << ",_)";

				std::list<Token> testTokens = tokenizer.tokenize(ss);
				testMap = std::map<std::string, EntityType>{
					{val, tokenToEntMap.at(stmt)}
				};

				// Reset the query
				expectedQuery = {};

				expectedQuery.addSelectArg(Synonym{ val, tokenToEntMap.at(stmt) });
				ca = ClauseArgs{};

				ca.addRelArg(Synonym{ val, tokenToEntMap.at(stmt) });
				ca.addRelArg(wildcard);
				SuchThat expectedSuchThat{ ca, RelRefType::Next };

				addST(expectedQuery, expectedSuchThat);

				assertQueryEqual(expectedQuery, testTokens);
			}
		}

		/* Invalid extractions */
		TEST_METHOD(testExtractNextInvalidLhs) {
			std::stringstream ss;
			ss << templateStr << "Next(\"invalidSyn\", _)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractNextInvalidRhs) {
			std::stringstream ss;
			ss << templateStr << "Next(_, \"invalidSyn\")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractNextUndeclaredLhs) {
			std::stringstream ss;
			ss << templateStr << "Next(undeclaredSyn, _)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractNextUndeclaredRhs) {
			std::stringstream ss;
			ss << templateStr << "Next(_, undeclaredSyn)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		// Next with invalid synonym types
		TEST_METHOD(testExtractNextWrongSynType) {
			const std::set<Token> invalidSynSet = {
					PQLToken::DesignEntity::Constant,
					PQLToken::DesignEntity::Procedure,
					PQLToken::DesignEntity::Variable,
			};
			std::string val = "syn";
			for (const auto& syn : invalidSynSet) {
				std::stringstream ss;
				ss << syn.val << " " << val << ";" << "Select "
					<< val << " such that Next(" << val << ",_)";
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