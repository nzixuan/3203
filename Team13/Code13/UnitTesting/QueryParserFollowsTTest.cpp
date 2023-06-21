#include "stdafx.h"
#include "CppUnitTest.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(QueryParserFollowsTTest)
	{
	public:
		// Template string such that query is valid
		std::string templateStr = "assign " + assignSyn.getVal() +"; read " + 
			readSyn.getVal() +"; Select " + assignSyn.getVal() + " such that ";
		ClauseArgs ca;
		Query expectedQuery;
		Tokenizer tokenizer;

		TEST_METHOD_INITIALIZE(initializeFollowsTQuery) {
			// Reinitalize the query after every method
			expectedQuery = {};
			
			expectedQuery.addSelectArg(assignSyn);
			ca = ClauseArgs{};
		}

		/* Valid Extractions */
		// W = wildcard '_', S = synonym, I = integer
		// WI = wildcard on lhs, integer on rhs, and so on for the others.
		TEST_METHOD(testExtractFollowsTWW) {
			/* Test if we can successfully extract a Follows* relationship
			   lhs = '_', rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Follows*(_,_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::FollowsT };
			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractFollowsTWI) {
			/* Test if we can successfully extract a Follows* relationship
			   lhs = '_', rhs = integer
			*/
			std::stringstream ss;
			ss << templateStr << "Follows*(_," << number.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(number);
			SuchThat expectedSuchThat{ ca, RelRefType::FollowsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractFollowsTWS) {
			/* Test if we can successfully extract a Follows* relationship
			   lhs = '_', rhs = declared synonym
			*/
			std::stringstream ss;
			ss << templateStr << "Follows*(_," << assignSyn.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(assignSyn);
			SuchThat expectedSuchThat{ ca, RelRefType::FollowsT };
			
			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractFollowsTIW) {
			/* Test if we can successfully extract a Follows* relationship
			   lhs = integer, rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Follows*(" << number.getVal() << ",_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(number);
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::FollowsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractFollowsTII) {
			/* Test if we can successfully extract a Follows* relationship
			   lhs = integer, rhs = integer
			*/
			std::stringstream ss;
			ss << templateStr << "Follows*(" << number.getVal() << "," 
				<< number2.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(number);
			ca.addRelArg(number2);
			SuchThat expectedSuchThat{ ca, RelRefType::FollowsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractFollowsTIS) {
			/* Test if we can successfully extract a Follows* relationship
			   lhs = integer, rhs = declared synonym
			*/
			std::stringstream ss;
			ss << templateStr << "Follows*(" << number.getVal() << ", " << assignSyn.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(number);
			ca.addRelArg(assignSyn);
			SuchThat expectedSuchThat{ ca, RelRefType::FollowsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractFollowsTSW) {
			/* Test if we can successfully extract a Follows* relationship
			   lhs = declared synonym, rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Follows*(" << assignSyn.getVal() << ",_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(assignSyn);
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::FollowsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractFollowsTSI) {
			/* Test if we can successfully extract a Follows* relationship
			   lhs = declared synonym, rhs = integer
			*/
			std::stringstream ss;
			ss << templateStr << "Follows*(" << assignSyn.getVal() << "," << number.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(assignSyn);
			ca.addRelArg(number);
			SuchThat expectedSuchThat{ ca, RelRefType::FollowsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractFollowsTSS) {
			/* Test if we can successfully extract a Follows* relationship
			   lhs = declared synonym, rhs = declared synonym
			*/
			std::stringstream ss;
			ss << templateStr << "Follows*(" << assignSyn.getVal() 
				<< "," << readSyn.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(assignSyn);
			ca.addRelArg(readSyn);
			SuchThat expectedSuchThat{ ca, RelRefType::FollowsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractFollowsTStmtSubtypes) {
			/* Tests if all stmt subtypes are valid as arguments
			*/
			std::map<std::string, EntityType> testMap;
			std::string val = "syn";
			for (const auto& stmt : stmtSet) {

				std::stringstream ss;
				ss << stmt.val << " " << val << ";" << "Select "
					<< val << " such that Follows*(" << val << ",_)";

				std::list<Token> testTokens = tokenizer.tokenize(ss);
				testMap = std::map<std::string, EntityType>{ 
					{val, tokenToEntMap.at(stmt)}
				};

				// Reset the query
				expectedQuery = {};
				expectedQuery.addSelectArg(Synonym{ val, tokenToEntMap.at(stmt) });
				ClauseArgs ca{};

				ca.addRelArg(Synonym{val, tokenToEntMap.at(stmt) });
				ca.addRelArg(wildcard);
				SuchThat expectedSuchThat{ ca, RelRefType::FollowsT };

				addST(expectedQuery, expectedSuchThat);

				assertQueryEqual(expectedQuery, testTokens);
			}
		}

		/* Invalid extractions */
		TEST_METHOD(testExtractFollowsTInvalidLhs) {
			std::stringstream ss;
			ss << templateStr << "Follows*(\"invalidSyn\", _)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractFollowsTInvalidRhs) {
			std::stringstream ss;
			ss << templateStr << "Follows*(_, \"invalidSyn\")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		// Follows using an undeclared synonym
		TEST_METHOD(testExtractFollowsTUndeclaredLhs) {
			std::stringstream ss;
			ss << templateStr << "Follows*(undeclaredSyn, _)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractFollowsUndeclaredRhs) {
			std::stringstream ss;
			ss << templateStr << "Follows*(_, undeclaredSyn)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		// Follows with invalid synonym types
		TEST_METHOD(testExtractFollowsTWrongSynType) {
			const std::set<Token> invalidSynSet = {
					PQLToken::DesignEntity::Constant,
					PQLToken::DesignEntity::Procedure,
					PQLToken::DesignEntity::Variable,
			};
			std::string val = "syn";
			for (const auto& syn : invalidSynSet) {
				std::stringstream ss;
				ss << syn.val << " " << val << ";" << "Select "
					<< val << " such that Follows*(" << val << ",_)";
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