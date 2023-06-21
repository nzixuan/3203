#include "stdafx.h"
#include "CppUnitTest.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(QueryParserPatternWhileTest)
	{
	public:
		Tokenizer tokenizer;
		// Template string such that query is valid
		std::string templateStr = "while " + whileSyn.getVal() + "; variable "
			+ varSyn.getVal() + "; Select " + whileSyn.getVal() + " ";
		ClauseArgs ca;
		Query expectedQuery;

		TEST_METHOD_INITIALIZE(initializePatternQuery) {
			// Reinitalize the query after every method
			expectedQuery = {};
			
			expectedQuery.addSelectArg(whileSyn);
			ca = ClauseArgs{};
		}

		/* Valid Extractions */
		// W = wildcard '_', S = synonym, I = integer
		// WI = wildcard on lhs, integer on rhs, and so on for the others.
		TEST_METHOD(testExtractPatternWW) {
			/* Test if we can successfully extract a Pattern relationship
			   lhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << whileSyn.getVal() << "(_,_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(wildcard);
			Pattern expectedPattern{ ca, whileSyn };

			addP(expectedQuery, expectedPattern);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractPatternSynW) {
			/* Test if we can successfully extract a Pattern relationship
			   lhs = synonym
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << whileSyn.getVal() 
				<< "(" << varSyn.getVal() << ",_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(varSyn);
			ca.addRelArg(wildcard);
			Pattern expectedPattern{ ca, whileSyn };

			addP(expectedQuery, expectedPattern);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractPatternSrcW) {
			/* Test if we can successfully extract a Pattern relationship
			   lhs = source argument
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << whileSyn.getVal() << "(\"x\",_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(SourceArg{"x"});
			ca.addRelArg(wildcard);
			Pattern expectedPattern{ ca, whileSyn };

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
					<< val << " pattern" << val << "(_,_)";
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
			const std::set<Token> invalidEntSet = {
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
			for (const auto& ent : invalidEntSet) {
				std::stringstream ss;
				// ent syn; Select syn pattern w(syn, _)
				ss << ent.val << " " << val << "; Select " << val << " pattern " 
					<< whileSyn.getVal() << "(" << val << ", _)";
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
			ss << templateStr << "pattern undeclaredSyn(_,_)";
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
			ss << templateStr << "pattern w(undeclaredSyn,_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractPatternRhsNotWildcard) {
			/* 2nd arg must be _
			*/
			std::stringstream ss;

			ss << templateStr << "pattern " << whileSyn.getVal() << "(_,2)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}
	};
}