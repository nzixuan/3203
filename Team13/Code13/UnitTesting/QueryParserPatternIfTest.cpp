#include "stdafx.h"
#include "CppUnitTest.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(QueryParserPatternIfTest)
	{
	public:
		Tokenizer tokenizer;
		// Template string such that query is valid
		std::string templateStr = "if " + ifSyn.getVal() + "; variable "
			+ varSyn.getVal() + "; Select " + ifSyn.getVal() + " ";

		ClauseArgs ca;
		Query expectedQuery;

		TEST_METHOD_INITIALIZE(initializePatternQuery) {
			// Reinitalize the query after every method
			expectedQuery = {};
			
			expectedQuery.addSelectArg(ifSyn);
		}

		/* Valid Extractions */
		// W = wildcard '_', S = synonym, I = integer
		// WI = wildcard on lhs, integer on rhs, and so on for the others.
		TEST_METHOD(testExtractPatternWWW) {
			/* Test if we can successfully extract a Pattern relationship
			   lhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << ifSyn.getVal() << "(_,_,_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);
			
			ca.addRelArg(wildcard);
			ca.addRelArg(wildcard);
			ca.addRelArg(wildcard);
			Pattern expectedPattern{ ca, ifSyn };

			addP(expectedQuery, expectedPattern);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractPatternSynWW) {
			/* Test if we can successfully extract a Pattern relationship
			   lhs = synonym
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << ifSyn.getVal() 
				<< "(" << varSyn.getVal() << ",_,_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(varSyn);
			ca.addRelArg(wildcard);
			ca.addRelArg(wildcard);
			Pattern expectedPattern{ ca, ifSyn };

			addP(expectedQuery, expectedPattern);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractPatternSrcWW) {
			/* Test if we can successfully extract a Pattern relationship
			   lhs = source argument
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << ifSyn.getVal() << "(\"x\",_,_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(SourceArg{"x"});
			ca.addRelArg(wildcard);
			ca.addRelArg(wildcard);
			Pattern expectedPattern{ ca, ifSyn };

			addP(expectedQuery, expectedPattern);

			assertQueryEqual(expectedQuery, testTokens);
		}

		/* Invalid extractions */
		TEST_METHOD(testExtractPatternInvalidSyn) {
			/* Syn used is not syn-assign, syn-if or syn-while
			*/
			const std::set<Token> invalidSynSet = {
					PQLToken::DesignEntity::Constant,
					PQLToken::DesignEntity::Procedure,
					PQLToken::DesignEntity::Variable,
					PQLToken::DesignEntity::Print,
					PQLToken::DesignEntity::Read,
					PQLToken::DesignEntity::Statement,
					PQLToken::DesignEntity::Call
			};
			std::string val = "syn";
			for (const auto& syn : invalidSynSet) {
				std::stringstream ss;
				ss << syn.val << " " << val << ";" << "Select "
					<< val << " pattern" << val << "(_,_,_)";
				std::list<Token> testTokens = tokenizer.tokenize(ss);

				auto func = [=] {
					QueryParser parser(testTokens);
					parser.parseQuery();
				};
				Assert::ExpectException<std::logic_error>(func);
			};
		}

		TEST_METHOD(testExtractPatternInvalidLhsSyn) {
			/* Synonym in lhs should only be variable
			*/
			const std::set<Token> invalidSynSet = {
					PQLToken::DesignEntity::Constant,
					PQLToken::DesignEntity::Procedure,
					PQLToken::DesignEntity::Print,
					PQLToken::DesignEntity::Read,
					PQLToken::DesignEntity::Statement,
					PQLToken::DesignEntity::Call,
					PQLToken::DesignEntity::While,
					PQLToken::DesignEntity::If,
					PQLToken::DesignEntity::Assign
			};
			std::string val = "syn";
			for (const auto& syn : invalidSynSet) {
				std::stringstream ss;
				ss << syn.val << " " << val << ";" << "Select "
					<< val << " pattern" << ifSyn.getVal() << "(" << val << ",_,_)";
				std::list<Token> testTokens = tokenizer.tokenize(ss);

				auto func = [=] {
					QueryParser parser(testTokens);
					parser.parseQuery();
				};
				Assert::ExpectException<std::logic_error>(func);
			};
		}
		TEST_METHOD(testExtractPatternUndeclaredPatternCond) {
			/* Undeclared synonym used
			*/
			std::stringstream ss;
			ss << templateStr << "pattern undeclaredSyn(_,_,_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractPatternUndeclaredLhs) {
			/* Undeclared synonym used
			*/
			std::stringstream ss;
			ss << templateStr << "pattern ifs(undeclaredSyn,_,_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractPatternSecondArgNotWildcard) {
			/* Both 2nd and 3rd args must be _
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << ifSyn.getVal() << "(_,\"x\",_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractPatternThirdArgNotWildcard) {
			/* Both 2nd and 3rd args must be _
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << ifSyn.getVal() << "(_,_,\"x\")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}
	};
}