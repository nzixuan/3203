#include "stdafx.h"
#include "CppUnitTest.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(QueryParserCallsTTest)
	{
	public:
		Tokenizer tokenizer;
		// Template string such that query is valid
		Synonym proc2{ "p2", EntityType::Procedure };
		std::string templateStr = "procedure " + procSyn.getVal() + ", " +
			proc2.getVal() + "; Select " + procSyn.getVal() + " such that ";
		ClauseArgs ca;
		Query expectedQuery;

		TEST_METHOD_INITIALIZE(initializeCallsTQuery) {
			// Reinitalize the query after every method
			expectedQuery = {};
			
			expectedQuery.addSelectArg(procSyn);
			ca = ClauseArgs{};
		}

		/* Valid Extractions */
		// W = wildcard '_', S = synonym, I = integer
		// WI = wildcard on lhs, integer on rhs, and so on for the others.
		TEST_METHOD(testExtractCallsTWW) {
			/* Test if we can successfully extract a CallsT relationship
			   lhs = '_', rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Calls*(_,_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::CallsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractCallsTWS) {
			/* Test if we can successfully extract a CallsT relationship
			   lhs = '_', rhs = declared synonym (We use the predeclared assign synonym for convenience)
			*/

			std::stringstream ss;
			ss << templateStr << "Calls*(_," << procSyn.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(procSyn);
			SuchThat expectedSuchThat{ ca, RelRefType::CallsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractCallsTWSrc) {
			/* Test if we can successfully extract a CallsT relationship
			   lhs = '_', rhs = source arg
			*/
			std::stringstream ss;
			ss << templateStr << "Calls*(_,\"x\")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(SourceArg{ "x" });
			SuchThat expectedSuchThat{ ca, RelRefType::CallsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractCallsTSrcW) {
			/* Test if we can successfully extract a CallsT relationship
			   lhs = source arg, rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Calls*(\"x\",_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(SourceArg{ "x" });
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::CallsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractCallsTSrcSrc) {
			/* Test if we can successfully extract a CallsT relationship
			   lhs = source arg, rhs = source arg
			*/
			std::stringstream ss;
			ss << templateStr << "Calls*(\"x\",\"y\")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(SourceArg{ "x" });
			ca.addRelArg(SourceArg{ "y" });
			SuchThat expectedSuchThat{ ca, RelRefType::CallsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractCallsTSrcS) {
			/* Test if we can successfully extract a CallsT relationship
			   lhs = integer, rhs = declared synonym
			*/

			std::stringstream ss;
			ss << templateStr << "Calls*(\"x\"," << procSyn.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(SourceArg{ "x" });
			ca.addRelArg(procSyn);
			SuchThat expectedSuchThat{ ca, RelRefType::CallsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractCallsTSW) {
			/* Test if we can successfully extract a CallsT relationship
			   lhs = declared synonym, rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "Calls*(" << procSyn.getVal() << ",_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(procSyn);
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::CallsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractCallsTSSrc) {
			/* Test if we can successfully extract a CallsT relationship
			   lhs = declared synonym, rhs = source arg
			*/
			std::stringstream ss;
			ss << templateStr << "Calls*(" << procSyn.getVal() << ",\"x\")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(procSyn);
			ca.addRelArg(SourceArg{ "x" });
			SuchThat expectedSuchThat{ ca, RelRefType::CallsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractCallsTSS) {
			/* Test if we can successfully extract a CallsT relationship
			   lhs = declared synonym, rhs = declared synonym
			*/
			std::stringstream ss;
			ss << templateStr << "Calls*(" << procSyn.getVal() << "," << proc2.getVal() << ")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(procSyn);
			ca.addRelArg(proc2);
			SuchThat expectedSuchThat{ ca, RelRefType::CallsT };

			addST(expectedQuery, expectedSuchThat);

			assertQueryEqual(expectedQuery, testTokens);
		}

		/* Invalid extractions */
		TEST_METHOD(testExtractCallsTInvalidLhs) {
			std::stringstream ss;
			ss << templateStr << "Calls*(2, _)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractCallsTInvalidRhs) {
			std::stringstream ss;
			ss << templateStr << "Calls*(_, 2)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractCallsTUndeclaredLhs) {
			std::stringstream ss;
			ss << templateStr << "Calls*(undeclaredSyn, _)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractCallsTUndeclaredRhs) {
			std::stringstream ss;
			ss << templateStr << "Calls*(_, undeclaredSyn)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		// CallsT with invalid synonym types
		TEST_METHOD(testExtractCallsTWrongSynType) {
			const std::set<Token> invalidSynSet = {
				PQLToken::DesignEntity::Assign,
				PQLToken::DesignEntity::Call,
				PQLToken::DesignEntity::Constant,
				PQLToken::DesignEntity::If,
				PQLToken::DesignEntity::Print,
				PQLToken::DesignEntity::Read,
				PQLToken::DesignEntity::Statement,
				PQLToken::DesignEntity::Variable,
				PQLToken::DesignEntity::While
			};
			std::string val = "syn";
			// Lhs test
			for (const auto& syn : invalidSynSet) {
				std::stringstream ss;
				ss << syn.val << " " << val << ";" << "Select "
					<< val << " such that Calls*(" << val << ",_)";
				std::list<Token> testTokens = tokenizer.tokenize(ss);

				auto func = [=] {
					QueryParser parser(testTokens);
					parser.parseQuery();
				};
				Assert::ExpectException<std::logic_error>(func);
			};

			// Rhs test
			for (const auto& syn : invalidSynSet) {
				std::stringstream ss;
				ss << syn.val << " " << val << ";" << "Select "
					<< val << " such that Calls*(_," << val << ")";
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