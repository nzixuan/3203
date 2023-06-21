#include "stdafx.h"
#include "CppUnitTest.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(QueryParserModifiesTest)
	{
	public:
		Tokenizer tokenizer;
		// Template string such that query is valid
		std::string templateStr = "variable " + varSyn.getVal() + "; read " +
			readSyn.getVal() + "; Select " + varSyn.getVal() + " such that ";
		ClauseArgs ca;
		Query expectedQuery;

		TEST_METHOD_INITIALIZE(initializeModifiesQuery) {
			// Reinitalize the query after every method
			expectedQuery = {};
			
			expectedQuery.addSelectArg(varSyn);
			ca = ClauseArgs{};
		}

		/* Valid Extractions (ModifiesS) */
		// W = wildcard '_', S = synonym, I = integer, Src = "IDENT"
		// WI = wildcard on lhs, integer on rhs, and so on for the others.
		TEST_METHOD(testExtractModifiesIW) {
			/* Test if we can successfully extract a ModifiesS relationship
			   lhs = integer, rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Modifies(" << number.getVal() << ", _)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(number);
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::Modifies };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractModifiesIS) {
			/* Test if we can successfully extract a ModifiesS relationship
			   lhs = integer, rhs = declared synonym
			*/
			std::stringstream ss;
			ss << templateStr << "Modifies(" << number.getVal() << "," << varSyn.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(number);
			ca.addRelArg(varSyn);
			SuchThat expectedSuchThat{ ca, RelRefType::Modifies };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractModifiesISrc) {
			/* Test if we can successfully extract a ModifiesS relationship
			   lhs = integer, rhs = source argument
			*/
			std::stringstream ss;
			ss << templateStr << "Modifies(" << number.getVal() << ", " << "\"x\"" << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(number);
			ca.addRelArg(SourceArg{"x"});
			SuchThat expectedSuchThat{ ca, RelRefType::Modifies };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}
		
		TEST_METHOD(testExtractModifiesSSrc) {
			/* Test if we can successfully extract a ModifiesS relationship
			   lhs = declared synonym, rhs = source argument
			*/
			std::stringstream ss;
			ss << templateStr << "Modifies(" << readSyn.getVal() << ",\"x\"" << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(readSyn);
			ca.addRelArg(SourceArg{ "x" });
			SuchThat expectedSuchThat{ ca, RelRefType::Modifies };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}
		
		TEST_METHOD(testExtractModifiesSW) {
			/* Test if we can successfully extract a ModifiesS relationship
			   lhs = declared synonym, rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Modifies(" << readSyn.getVal() << ",_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(readSyn);
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::Modifies };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractModifiesSS) {
			/* Test if we can successfully extract a ModifiesS relationship
			   lhs = declared synonym, rhs = declared synonym
			*/
			std::stringstream ss;
			ss << templateStr << "Modifies(" << readSyn.getVal() << "," << varSyn.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(readSyn);
			ca.addRelArg(varSyn);
			SuchThat expectedSuchThat{ ca, RelRefType::Modifies };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		/* Valid Extractions (ModifiesP) */
		// Only need to test if ModifiesP can take SourceArg in LHS
		TEST_METHOD(testExtractModifiesSrcW) {
			/* Test if we can successfully extract a ModifiesP relationship
			   lhs = SourceArg, rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Modifies(\"x\",_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(SourceArg{"x"});
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::Modifies };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractModifiesSrcS) {
			/* Test if we can successfully extract a ModifiesP relationship
			   lhs = SourceArg, rhs = Synonym
			*/
			std::stringstream ss;
			ss << templateStr << "Modifies(\"x\"," << varSyn.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(SourceArg{ "x" });
			ca.addRelArg(varSyn);
			SuchThat expectedSuchThat{ ca, RelRefType::Modifies };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractModifiesSrcSrc) {
			/* Test if we can successfully extract a ModifiesP relationship
			   lhs = SourceArg, rhs = SourceArg
			*/
			std::stringstream ss;
			ss << templateStr << "Modifies(\"x\",\"y\")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(SourceArg{ "x" });
			ca.addRelArg(SourceArg{ "y" });
			SuchThat expectedSuchThat{ ca, RelRefType::Modifies };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractModifiesLhsSubtypes) {
			/* Tests if all lhs stmt subtypes + proc are valid as arguments
			*/
			std::map<std::string, EntityType> testMap;
			std::string val = "syn";
			for (const auto& stmt : stmtSet) {

				std::stringstream ss;
				ss << stmt.val << " " << val << ";" << "Select "
					<< val << " such that Modifies(" << val << ",_)";

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
				SuchThat expectedSuchThat{ ca, RelRefType::Modifies };

				addST(expectedQuery, expectedSuchThat);

				assertQueryEqual(expectedQuery, testTokens);
			}

			// Test for procedure
			Token procToken = PQLToken::DesignEntity::Procedure;
			std::stringstream ss;
			ss << procToken.val << " " << val << ";" << "Select "
				<< val << " such that Modifies(" << val << ",_)";

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
			SuchThat expectedSuchThat{ ca, RelRefType::Modifies };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		/* Invalid extractions */
		TEST_METHOD(testExtractModifiesSInvalidLhs) {
			std::stringstream ss;
			ss << templateStr << "Modifies(_, _)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractModifiesSInvalidRhs) {
			std::stringstream ss;
			ss << templateStr << "Modifies(" << readSyn.getVal() << ", 2)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		// ModifiesS using an undeclared synonym
		TEST_METHOD(testExtractModifiesSUndeclaredLhs) {
			std::stringstream ss;
			ss << templateStr << "Modifies(undeclaredSyn, _)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractModifiesSUndeclaredRhs) {
			std::stringstream ss;
			ss << templateStr << "Modifies(_, undeclaredSyn)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		// ModifiesS with invalid synonym types
		TEST_METHOD(testExtractModifiesSWrongSynTypeLhs) {
			const std::set<Token> invalidSynSet = {
					PQLToken::DesignEntity::Constant,
					PQLToken::DesignEntity::Variable,
			};
			std::string val = "syn";
			for (const auto& syn : invalidSynSet) {
				std::stringstream ss;
				ss << syn.val << " " << val << ";" << "Select "
					<< val << " such that Modifies(" << val << ",_)";
				std::list<Token> testTokens = tokenizer.tokenize(ss);

				auto func = [=] {
					QueryParser parser(testTokens);
					parser.parseQuery();
				};
				Assert::ExpectException<std::logic_error>(func);
			};
		}

		TEST_METHOD(testExtractModifiesSWrongSynTypeRhs) {
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
					<< val << " such that Modifies(_," << val << ")";
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