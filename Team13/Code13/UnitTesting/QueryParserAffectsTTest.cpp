#include "stdafx.h"
#include "CppUnitTest.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(QueryParserAffectsTTest)
	{
	public:
		Tokenizer tokenizer;
		// Template string such that query is valid
		Synonym assign2{ "a2", EntityType::Assign };
		std::string templateStr = "assign " + assignSyn.getVal() + ", " +
			assign2.getVal() + "; Select " + assignSyn.getVal() + " such that ";
		ClauseArgs ca;
		Query expectedQuery;

		TEST_METHOD_INITIALIZE(initializeAffectsTQuery) {
			// Reinitalize the query after every method
			expectedQuery = {};
			expectedQuery.addSelectArg(assignSyn);
			ca = ClauseArgs{};
		}
		/* Valid Extractions */
		// W = wildcard '_', S = synonym, I = integer
		// WI = wildcard on lhs, integer on rhs, and so on for the others.
		TEST_METHOD(testExtractAffectsTWW) {
			/* Test if we can successfully extract a AffectsT relationship
			   lhs = '_', rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Affects*(_,_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::AffectsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}
		TEST_METHOD(testExtractAffectsTWI) {
			/* Test if we can successfully extract a AffectsT relationship
			   lhs = '_', rhs = integer
			*/
			std::stringstream ss;
			ss << templateStr << "Affects*(_," << number.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(number);
			SuchThat expectedSuchThat{ ca, RelRefType::AffectsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractAffectsTWS) {
			/* Test if we can successfully extract a AffectsT relationship
			   lhs = '_', rhs = declared synonym (We use the predeclared assign synonym for convenience)
			*/

			std::stringstream ss;
			ss << templateStr << "Affects*(_," << assignSyn.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(assignSyn);
			SuchThat expectedSuchThat{ ca, RelRefType::AffectsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractAffectsTIW) {
			/* Test if we can successfully extract a AffectsT relationship
			   lhs = integer, rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Affects*(" << number.getVal() << ", _)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(number);
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::AffectsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractAffectsTII) {
			/* Test if we can successfully extract a AffectsT relationship
			   lhs = integer, rhs = integer
			*/
			std::stringstream ss;
			ss << templateStr << "Affects*(" << number.getVal() << "," << number2.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(number);
			ca.addRelArg(number2);
			SuchThat expectedSuchThat{ ca, RelRefType::AffectsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractAffectsTIS) {
			/* Test if we can successfully extract a AffectsT relationship
			   lhs = integer, rhs = declared synonym
			*/

			std::stringstream ss;
			ss << templateStr << "Affects*(" << number.getVal() << ", " << assignSyn.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(number);
			ca.addRelArg(assignSyn);
			SuchThat expectedSuchThat{ ca, RelRefType::AffectsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractAffectsTSW) {
			/* Test if we can successfully extract a AffectsT relationship
			   lhs = declared synonym, rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Affects*(" << assignSyn.getVal() << ",_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(assignSyn);
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::AffectsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractAffectsTSI) {
			/* Test if we can successfully extract a AffectsT relationship
			   lhs = declared synonym, rhs = integer
			*/
			std::stringstream ss;
			ss << templateStr << "Affects*(" << assignSyn.getVal() << "," << number.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(assignSyn);
			ca.addRelArg(number);
			SuchThat expectedSuchThat{ ca, RelRefType::AffectsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractAffectsTSS) {
			/* Test if we can successfully extract a AffectsT relationship
			   lhs = declared synonym, rhs = declared synonym
			*/
			std::stringstream ss;
			ss << templateStr << "Affects*(" << assignSyn.getVal() << "," << assign2.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(assignSyn);
			ca.addRelArg(assign2);
			SuchThat expectedSuchThat{ ca, RelRefType::AffectsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractAffectsTStmtSubtypes) {
			/* Tests if all stmt subtypes are valid as arguments
			*/
			std::map<std::string, EntityType> testMap;
			std::string val = "syn";
			for (const auto& stmt : stmtSet) {

				std::stringstream ss;
				ss << stmt.val << " " << val << ";" << "Select "
					<< val << " such that Affects*(" << val << ",_)";

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
		TEST_METHOD(testExtractAffectsTInvalidLhs) {
			std::stringstream ss;
			ss << templateStr << "Affects*(\"invalidSyn\", _)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractAffectsTInvalidRhs) {
			std::stringstream ss;
			ss << templateStr << "Affects*(_, \"invalidSyn\")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractAffectsTUndeclaredLhs) {
			std::stringstream ss;
			ss << templateStr << "Affects*(undeclaredSyn, _)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractAffectsTUndeclaredRhs) {
			std::stringstream ss;
			ss << templateStr << "Affects*(_, undeclaredSyn)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		// AffectsT with invalid synonym types
		TEST_METHOD(testExtractAffectsTWrongSynType) {
			const std::set<Token> invalidSynSet = {
				PQLToken::DesignEntity::Constant,
				PQLToken::DesignEntity::Procedure,
				PQLToken::DesignEntity::Variable,
			};
			std::string val = "syn";
			for (const auto& syn : invalidSynSet) {
				std::stringstream ss;
				ss << syn.val << " " << val << ";" << "Select "
					<< val << " such that Affects*(" << val << ",_)";
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