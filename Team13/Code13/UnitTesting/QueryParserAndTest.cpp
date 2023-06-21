#include "stdafx.h"
#include "CppUnitTest.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(QueryParserAndTest)
	{
	public:
		Tokenizer tokenizer;
		// Template string such that query is valid
		// assign a; variable v; if ifs; constant cst; Select a 
		std::string templateStr = "assign " + assignSyn.getVal() + "; variable " +
			varSyn.getVal() + "; if " + ifSyn.getVal() + "; constant " + constSyn.getVal() + 
			"; Select " + assignSyn.getVal() + " ";
		std::string srcVal = "var";
		std::string suchThat1 = " Follows(" + assignSyn.getVal() + ", _) ";
		std::string suchThat2 = " Modifies(" + number.getVal() + ", " + varSyn.getVal() + ") ";
		std::string pattern1 = " " + assignSyn.getVal() + "(\""+ srcVal + "\", _) ";
		std::string pattern2 = " " + ifSyn.getVal() + "(" + varSyn.getVal() + ",_,_) ";
		std::string with1 = " " + varSyn.getVal() + ".varName = \"" + srcVal + "\" ";
		std::string with2 = " " + constSyn.getVal() + ".value = " + number.getVal();
		ClauseArgs ca;

		Query expectedQuery;

		TEST_METHOD_INITIALIZE(initializeFollowsQuery) {
			// Reinitalize the query after every method
			expectedQuery = {};
			
			expectedQuery.addSelectArg(assignSyn);
			ca = ClauseArgs{};
		}

		/* Valid and clauses */
		// Both clauses connected by 'and' should be the same type of clause
		TEST_METHOD(testExtractAndSuchThat) {
			/* Test if we can successfully extract a Such That and relationship.
			*/
			std::stringstream ss;
			ss << templateStr << "such that " << suchThat1 << " and " << suchThat2;
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(assignSyn);
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat1{ ca, RelRefType::Follows };
			ca = ClauseArgs{};
			ca.addRelArg(number);
			ca.addRelArg(varSyn);
			SuchThat expectedSuchThat2{ ca, RelRefType::Modifies };
			addST(expectedQuery, expectedSuchThat1);
			addST(expectedQuery, expectedSuchThat2);

			assertQueryEqual(expectedQuery, testTokens);
		}
		
		TEST_METHOD(testExtractAndPattern) {
			/* Test if we can successfully extract a Pattern and relationship.
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << pattern1 << " and " << pattern2;
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(SourceArg{srcVal});
			ca.addRelArg(wildcard);
			Pattern expectedPattern1{ ca , assignSyn };
			ca = ClauseArgs{};
			ca.addRelArg(varSyn);
			ca.addRelArg(wildcard);
			ca.addRelArg(wildcard);
			Pattern expectedPattern2{ ca, ifSyn };
			addP(expectedQuery, expectedPattern1);
			addP(expectedQuery, expectedPattern2);

			assertQueryEqual(expectedQuery, testTokens);
		}

		 //Uncomment when with implementation is complete
		TEST_METHOD(testExtractAndWith) {
			/* Test if we can successfully extract a With and relationship.
			*/
			std::stringstream ss;
			ss << templateStr << "with " << with1 << " and " << with2;
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(AttrRef{varSyn, AttrName::VarName});
			ca.addRelArg(SourceArg{ srcVal });
			With expectedWith1{ ca, WithRefType::Name };
			ca = ClauseArgs{};
			ca.addRelArg(AttrRef{ constSyn, AttrName::Value });
			ca.addRelArg(number);
			With expectedWith2{ ca, WithRefType::Integer };
			addW(expectedQuery, expectedWith1);
			addW(expectedQuery, expectedWith2);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractSuchThatAndPattern) {
			std::stringstream ss;
			ss << templateStr << "such that " << suchThat1 << " and " << pattern1;
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractSuchThatAndWith) {
			std::stringstream ss;
			ss << templateStr << "such that " << suchThat1 << " and " << with1;
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractPatternAndSuchThat) {
			std::stringstream ss;
			ss << templateStr << "pattern " << pattern1 << " and " << suchThat1;
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractPatternAndWith) {
			std::stringstream ss;
			ss << templateStr << "pattern " << pattern1 << " and " << with1;
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractWithAndSuchThat) {
			std::stringstream ss;
			ss << templateStr << "with " << with1 << " and " << suchThat1;
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractWithAndPattern) {
			std::stringstream ss;
			ss << templateStr << "with " << with1 << " and " << pattern1;
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}
	};
}