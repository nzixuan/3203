#include "stdafx.h"
#include "CppUnitTest.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(QueryParserParentTest)
	{
	public:
		Tokenizer tokenizer;
		// Template string such that query is valid
		std::string templateStr = "if " + ifSyn.getVal() + "; assign " +
			assignSyn.getVal() + "; Select " + assignSyn.getVal() + " such that ";
		ClauseArgs ca;
		Query expectedQuery;

		TEST_METHOD_INITIALIZE(initializeParentQuery) {
			// Reinitalize the query after every method
			expectedQuery = {};
			
			expectedQuery.addSelectArg(ifSyn);
			ca = ClauseArgs{};
		}
		/* Valid Extractions */
		// W = wildcard '_', S = synonym, I = integer
		// WI = wildcard on lhs, integer on rhs, and so on for the others.
		TEST_METHOD(testExtractParentWW) {
			/* Test if we can successfully extract a Parent relationship
			   lhs = '_', rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Parent*(_,_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::ParentT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}
		TEST_METHOD(testExtractParentWI) {
			/* Test if we can successfully extract a Parent relationship
			   lhs = '_', rhs = integer
			*/
			std::stringstream ss;
			ss << templateStr << "Parent*(_," << number.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(number);
			SuchThat expectedSuchThat{ ca, RelRefType::ParentT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractParentWS) {
			/* Test if we can successfully extract a Parent relationship
			   lhs = '_', rhs = declared synonym (We use the predeclared assign synonym for convenience)
			*/

			std::stringstream ss;
			ss << templateStr << "Parent*(_," << assignSyn.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(assignSyn);
			SuchThat expectedSuchThat{ ca, RelRefType::ParentT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractParentIW) {
			/* Test if we can successfully extract a Parent relationship
			   lhs = integer, rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Parent*(" << number.getVal() << ", _)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(number);
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::ParentT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractParentII) {
			/* Test if we can successfully extract a Parent relationship
			   lhs = integer, rhs = integer
			*/
			std::stringstream ss;
			ss << templateStr << "Parent*(" << number.getVal() << "," << number2.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(number);
			ca.addRelArg(number2);
			SuchThat expectedSuchThat{ ca, RelRefType::ParentT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractParentIS) {
			/* Test if we can successfully extract a Parent relationship
			   lhs = integer, rhs = declared synonym
			*/

			std::stringstream ss;
			ss << templateStr << "Parent*(" << number.getVal() << "," << assignSyn.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(number);
			ca.addRelArg(assignSyn);
			SuchThat expectedSuchThat{ ca, RelRefType::ParentT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractParentSW) {
			/* Test if we can successfully extract a Parent relationship
			   lhs = declared synonym, rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Parent*(" << ifSyn.getVal() << ",_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(ifSyn);
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::ParentT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractParentSI) {
			/* Test if we can successfully extract a Parent relationship
			   lhs = declared synonym, rhs = integer
			*/
			std::stringstream ss;
			ss << templateStr << "Parent*(" << ifSyn.getVal() << ",2)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(ifSyn);
			ca.addRelArg(number);
			SuchThat expectedSuchThat{ ca, RelRefType::ParentT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractParentSS) {
			/* Test if we can successfully extract a Parent relationship
			   lhs = declared synonym, rhs = declared synonym
			*/
			std::stringstream ss;
			ss << templateStr << "Parent*(" << ifSyn.getVal() << "," << assignSyn.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(ifSyn);
			ca.addRelArg(assignSyn);
			SuchThat expectedSuchThat{ ca, RelRefType::ParentT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractParentStmtSubtypes) {
			/* Tests if all stmt subtypes are valid as arguments
			*/
			std::map<std::string, EntityType> testMap;
			std::string val = "syn";
			for (const auto& stmt : stmtSet) {
				std::stringstream ss;
				ss << stmt.val << " " << val << ";" << "Select "
					<< val << " such that Parent*(" << val << ",_)";

				std::list<Token> testTokens = tokenizer.tokenize(ss);
				testMap = std::map<std::string, EntityType>{
					{val, tokenToEntMap.at(stmt)}
				};

				// Reset the query
				expectedQuery = {};

				expectedQuery.addSelectArg(Synonym{ val, tokenToEntMap.at(stmt) });

				ca.addRelArg(Synonym{ val, tokenToEntMap.at(stmt) });
				ca.addRelArg(wildcard);
				SuchThat expectedSuchThat{ ca, RelRefType::ParentT };

				addST(expectedQuery, expectedSuchThat);

				assertQueryEqual(expectedQuery, testTokens);
			}
		}

		/* Invalid extractions */
		TEST_METHOD(testExtractParentInvalidLhs) {
			std::stringstream ss;
			ss << templateStr << "Parent*(\"invalidSyn\", _)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractParentInvalidRhs) {
			std::stringstream ss;
			ss << templateStr << "Parent*(_, \"invalidSyn\")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractParentUndeclaredLhs) {
			std::stringstream ss;
			ss << templateStr << "Parent*(undeclaredSyn, _)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractParentUndeclaredRhs) {
			std::stringstream ss;
			ss << templateStr << "Parent*(_, undeclaredSyn)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		// Parent with invalid synonym types
		TEST_METHOD(testExtractParentWrongSynType) {
			const std::set<Token> invalidSynSet = {
					PQLToken::DesignEntity::Constant,
					PQLToken::DesignEntity::Procedure,
					PQLToken::DesignEntity::Variable,
			};
			std::string val = "syn";
			for (const auto& syn : invalidSynSet) {
				std::stringstream ss;
				ss << syn.val << " " << val << ";" << "Select "
					<< val << " such that Parent*(" << val << ",_)";
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