#include "stdafx.h"
#include "CppUnitTest.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(QueryParserUsesTest)
	{
	public:
		Tokenizer tokenizer;
		// Template string such that query is valid
		std::string templateStr = "variable " + varSyn.getVal() + "; print " +
			printSyn.getVal() + "; Select " + varSyn.getVal() + " such that ";
		ClauseArgs ca;
		std::map<std::string, EntityType> expectedMap = {
			{varSyn.getVal() , EntityType::Variable},
			{printSyn.getVal(), EntityType::Print}
		};
		Query expectedQuery;

		TEST_METHOD_INITIALIZE(initializeUsesQuery) {
			// Reinitalize the query after every method
			expectedQuery = {};
			
			expectedQuery.addSelectArg(varSyn);
			ca = ClauseArgs{};
		}

		/* Valid Extractions (UsesS) */
		// W = wildcard '_', S = synonym, I = integer, Src = "IDENT"
		// WI = wildcard on lhs, integer on rhs, and so on for the others.
		TEST_METHOD(testExtractUsesIW) {
			/* Test if we can successfully extract a UsesS relationship
			   lhs = integer, rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Uses(" << number.getVal() << ", _)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(number);
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::Uses };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractUsesIS) {
			/* Test if we can successfully extract a UsesS relationship
			   lhs = integer, rhs = declared synonym
			*/
			std::stringstream ss;
			ss << templateStr << "Uses(" << number.getVal() << "," << varSyn.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(number);
			ca.addRelArg(varSyn);
			SuchThat expectedSuchThat{ ca, RelRefType::Uses };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractUsesISrc) {
			/* Test if we can successfully extract a UsesS relationship
			   lhs = integer, rhs = source argument
			*/
			std::stringstream ss;
			ss << templateStr << "Uses(" << number.getVal() << ", " << "\"x\"" << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(number);
			ca.addRelArg(SourceArg{ "x" });
			SuchThat expectedSuchThat{ ca, RelRefType::Uses };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractUsesSSrc) {
			/* Test if we can successfully extract a UsesS relationship
			   lhs = declared synonym, rhs = source argument
			*/
			std::stringstream ss;
			ss << templateStr << "Uses(" << printSyn.getVal() << ",\"x\"" << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(printSyn);
			ca.addRelArg(SourceArg{ "x" });
			SuchThat expectedSuchThat{ ca, RelRefType::Uses };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractUsesSW) {
			/* Test if we can successfully extract a UsesS relationship
			   lhs = declared synonym, rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Uses(" << printSyn.getVal() << ",_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(printSyn);
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::Uses };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractUsesSS) {
			/* Test if we can successfully extract a UsesS relationship
			   lhs = declared synonym, rhs = declared synonym
			*/
			std::stringstream ss;
			ss << templateStr << "Uses(" << printSyn.getVal() << "," << varSyn.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(printSyn);
			ca.addRelArg(varSyn);
			SuchThat expectedSuchThat{ ca, RelRefType::Uses };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		/* Valid Extractions (UsesP) */
		// Only need to test if UsesP can take SourceArg in LHS
		TEST_METHOD(testExtractUsesSrcW) {
			/* Test if we can successfully extract a UsesP relationship
			   lhs = SourceArg, rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Uses(\"x\",_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(SourceArg{ "x" });
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::Uses };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractUsesSrcS) {
			/* Test if we can successfully extract a UsesP relationship
			   lhs = SourceArg, rhs = Synonym
			*/
			std::stringstream ss;
			ss << templateStr << "Uses(\"x\"," << varSyn.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(SourceArg{ "x" });
			ca.addRelArg(varSyn);
			SuchThat expectedSuchThat{ ca, RelRefType::Uses };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractUsesSrcSrc) {
			/* Test if we can successfully extract a UsesP relationship
			   lhs = SourceArg, rhs = SourceArg
			*/
			std::stringstream ss;
			ss << templateStr << "Uses(\"x\",\"y\")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(SourceArg{ "x" });
			ca.addRelArg(SourceArg{ "y" });
			SuchThat expectedSuchThat{ ca, RelRefType::Uses };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractUsesLhsSubtypes) {
			/* Tests if all lhs stmt subtypes + proc are valid as arguments
			*/
			std::map<std::string, EntityType> testMap;
			std::string val = "syn";
			for (const auto& stmt : stmtSet) {

				std::stringstream ss;
				ss << stmt.val << " " << val << ";" << "Select "
					<< val << " such that Uses(" << val << ",_)";

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
				SuchThat expectedSuchThat{ ca, RelRefType::Uses };

				addST(expectedQuery, expectedSuchThat);

				assertQueryEqual(expectedQuery, testTokens);
			}

			// Test for procedure
			Token procToken = PQLToken::DesignEntity::Procedure;
			std::stringstream ss;
			ss << procToken.val << " " << val << ";" << "Select "
				<< val << " such that Uses(" << val << ",_)";

			std::list<Token> testTokens = tokenizer.tokenize(ss);
			testMap = std::map<std::string, EntityType>{
				{val, EntityType::Procedure}
			};

			// Reset the query
			expectedQuery = {};
			expectedQuery.addSelectArg(Synonym{ val, EntityType::Procedure });

			ca = ClauseArgs{};
			ca.addRelArg(Synonym{ val, EntityType::Procedure });
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::Uses };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		/* Invalid extractions */
		TEST_METHOD(testExtractUsesSInvalidLhs) {
			std::stringstream ss;
			ss << templateStr << "Uses(_, _)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractUsesSInvalidRhs) {
			std::stringstream ss;
			ss << templateStr << "Uses(" << printSyn.getVal() << ", 2)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		// UsesS using an undeclared synonym
		TEST_METHOD(testExtractUsesSUndeclaredLhs) {
			std::stringstream ss;
			ss << templateStr << "Uses(undeclaredSyn, _)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractUsesSUndeclaredRhs) {
			std::stringstream ss;
			ss << templateStr << "Uses(_, undeclaredSyn)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		// UsesS with invalid synonym types
		TEST_METHOD(testExtractUsesSWrongSynTypeLhs) {
			const std::set<Token> invalidSynSet = {
					PQLToken::DesignEntity::Constant,
					PQLToken::DesignEntity::Variable,
			};
			std::string val = "syn";
			for (const auto& syn : invalidSynSet) {
				std::stringstream ss;
				ss << syn.val << " " << val << ";" << "Select "
					<< val << " such that Uses(" << val << ",_)";
				std::list<Token> testTokens = tokenizer.tokenize(ss);

				auto func = [=] {
					QueryParser parser(testTokens);
					parser.parseQuery();
				};
				Assert::ExpectException<std::logic_error>(func);
			};
		}

		TEST_METHOD(testExtractUsesSWrongSynTypeRhs) {
			// Only variable-syn allowed
			const std::set<Token> invalidSynSet = {
				PQLToken::DesignEntity::Assign,
				PQLToken::DesignEntity::Call,
				PQLToken::DesignEntity::Constant,
				PQLToken::DesignEntity::If,
				PQLToken::DesignEntity::Print,
				PQLToken::DesignEntity::Procedure,
				PQLToken::DesignEntity::Read,
				PQLToken::DesignEntity::Statement,
				PQLToken::DesignEntity::While
			};
			std::string val = "syn";
			for (const auto& syn : invalidSynSet) {
				std::stringstream ss;
				ss << syn.val << " " << val << ";" << "Select "
					<< val << " such that Uses(_," << val << ")";
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