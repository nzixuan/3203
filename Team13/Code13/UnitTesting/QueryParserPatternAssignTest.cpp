#include "stdafx.h"
#include "CppUnitTest.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(QueryParserPatternAssignTest)
	{
	public:
		Tokenizer tokenizer;
		// Template string such that query is valid
		std::string templateStr = "assign " + assignSyn.getVal() + "; variable " +
			varSyn.getVal() + "; Select " + assignSyn.getVal() + " ";
		ClauseArgs ca;
		Query expectedQuery;

		TEST_METHOD_INITIALIZE(initializePatternQuery) {
			// Reinitalize the query after every method
			expectedQuery = {};
			
			expectedQuery.addSelectArg(assignSyn);
			ca = ClauseArgs{};
		}

		/* Valid Extractions */
		// W = wildcard '_', S = synonym, I = integer
		// WI = wildcard on lhs, integer on rhs, and so on for the others.
		TEST_METHOD(testExtractPatternWW) {
			/* Test if we can successfully extract a Pattern relationship
			   lhs = '_', rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " <<  assignSyn.getVal() << "(_,_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(wildcard);
			Pattern expectedPattern{ ca, assignSyn };

			addP(expectedQuery, expectedPattern);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractPatternWSrcExact) {
			/* Test if we can successfully extract a Pattern relationship
			   lhs = '_', rhs = source argument (exact match)
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << assignSyn.getVal() << "(_,\"x\")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(SourceArg{"x"});
			Pattern expectedPattern{ ca, assignSyn };

			addP(expectedQuery, expectedPattern);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractPatternWSrcPartialIdent) {
			/* Test if we can successfully extract a Pattern relationship
			   lhs = '_', rhs = source argument (partial match, var_name)
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << assignSyn.getVal() << "(_,_\"x\"_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(SourceArg{"x", false});
			Pattern expectedPattern{ ca, assignSyn };

			addP(expectedQuery, expectedPattern);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractPatternWSrcPartialNumber) {
			/* Test if we can successfully extract a Pattern relationship
			   lhs = '_', rhs = source argument (partial match, const_value)
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << assignSyn.getVal() << "(_,_\"2\"_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(SourceArg{ "2", false });
			Pattern expectedPattern{ ca, assignSyn };

			addP(expectedQuery, expectedPattern);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractPatternSW) {
			/* Test if we can successfully extract a Pattern relationship
			   lhs = declared synonym(Should be variable), rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << assignSyn.getVal() 
				<< "(" << varSyn.getVal() << ",_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(varSyn);
			ca.addRelArg(wildcard);
			Pattern expectedPattern{ ca, assignSyn };

			addP(expectedQuery, expectedPattern);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractPatternSSrcExact) {
			/* Test if we can successfully extract a Pattern relationship
			   lhs = declared synonym(Should be variable), rhs = source arg (exact matching)
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << assignSyn.getVal() << "(" 
				<< varSyn.getVal() << "," << "\"x\")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(varSyn);
			ca.addRelArg(SourceArg{ "x"});
			Pattern expectedPattern{ ca, assignSyn };

			addP(expectedQuery, expectedPattern);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractPatternSSrcPartialIdent) {
			/* Test if we can successfully extract a Pattern relationship
			   lhs = declared synonym(Should be variable), rhs = source arg (partial match, var_name)
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << assignSyn.getVal() << "(" 
				<< varSyn.getVal() << "," << "_\"x\"_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(varSyn);
			ca.addRelArg(SourceArg{ "x", false });
			Pattern expectedPattern{ ca, assignSyn };

			addP(expectedQuery, expectedPattern);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractPatternSSrcPartialNumber) {
			/* Test if we can successfully extract a Pattern relationship
			   lhs = declared synonym(Should be variable), rhs = source arg (partial match, const_value)
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << assignSyn.getVal() << "(" 
				<< varSyn.getVal() << "," << "_\"2\"_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(varSyn);
			ca.addRelArg(SourceArg{ "2", false });
			Pattern expectedPattern{ ca, assignSyn };

			addP(expectedQuery, expectedPattern);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractPatternSrcW) {
			/* Test if we can successfully extract a Pattern relationship
			   lhs = source argument, rhs = '_'
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << assignSyn.getVal() << "(\"x\",_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(SourceArg{"x"});
			ca.addRelArg(wildcard);
			Pattern expectedPattern{ ca, assignSyn };

			addP(expectedQuery, expectedPattern);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractPatternSrcSrcExact) {
			/* Test if we can successfully extract a Pattern relationship
			   lhs = source argument, rhs = source arg (exact matching)
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << assignSyn.getVal() << "(\"x\",\"x\")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(SourceArg{ "x" });
			ca.addRelArg(SourceArg{ "x" });
			Pattern expectedPattern{ ca, assignSyn };

			addP(expectedQuery, expectedPattern);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractPatternSrcSrcPartialIdent) {
			/* Test if we can successfully extract a Pattern relationship
			   lhs = source argument, rhs = source arg (partial match, var_name)
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << assignSyn.getVal() << "(\"x\",_\"x\"_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(SourceArg{ "x" });
			ca.addRelArg(SourceArg{ "x", false });
			Pattern expectedPattern{ ca, assignSyn };

			addP(expectedQuery, expectedPattern);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractPatternSrcSrcPartialNumber) {
			/* Test if we can successfully extract a Pattern relationship
			   lhs = source argument, rhs = source arg (partial match, const_value)
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << assignSyn.getVal() << "(\"x\",_\"2\"_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(SourceArg{ "x" });
			ca.addRelArg(SourceArg{ "2", false });
			Pattern expectedPattern{ ca, assignSyn };
			
			addP(expectedQuery, expectedPattern);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractPatternWExprOpExact) {
			/* Test if we can successfully extract a Pattern relationship
			   lhs = '_', rhs = expression with operators only
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << assignSyn.getVal() << "(_,\"x + y - z * 2 / 3\")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(SourceArg{ "x y + z 2 * 3 / -" });
			Pattern expectedPattern{ ca, assignSyn };

			addP(expectedQuery, expectedPattern);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractPatternWExprOpPartial) {
			/* Test if we can successfully extract a Pattern relationship
			   lhs = '_', rhs = expression with operators only
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << assignSyn.getVal() << "(_,_\"x + y - z * 2 / 3\"_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(SourceArg{ "x y + z 2 * 3 / -", false });
			Pattern expectedPattern{ ca, assignSyn };

			addP(expectedQuery, expectedPattern);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractPatternWExprOpBracketsExact) {
			/* Test if we can successfully extract a Pattern relationship
			   lhs = '_', rhs = expression with operators only
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << assignSyn.getVal() << "(_,\"((xz + y)) - ((z * 2) / 3)\")";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(SourceArg{ "xz y + z 2 * 3 / -"});
			Pattern expectedPattern{ ca, assignSyn };
;
			addP(expectedQuery, expectedPattern);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractPatternWExprOpBracketsPartial) {
			/* Test if we can successfully extract a Pattern relationship
			   lhs = '_', rhs = expression with operators and brackets
			*/
			std::stringstream ss;
			ss << templateStr << "pattern " << assignSyn.getVal() << "(_,_\"((x + y)) - ((z * 2) / 3)\"_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(wildcard);
			ca.addRelArg(SourceArg{ "x y + z 2 * 3 / -", false });
			Pattern expectedPattern{ ca, assignSyn };

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
				ss << ent.val << " " << val << "; Select " 
					<< assignSyn.getVal() << " pattern "
					<< "(" << val << ", _)";
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
			ss << templateStr << "pattern undeclaredSyn(_, _)";
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
			ss << templateStr << "pattern a(undeclaredSyn, _)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractPatternBadExpr) {
			/* Expr is not a valid format
			*/
			std::stringstream ss;
			ss << templateStr << "pattern a(_, _\"x + 2 -*/ 3\"_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}


		TEST_METHOD(testExtractPatternExprUnbalBrackets) {
			/* Parentheses are not balanced
			*/
			std::stringstream ss;
			ss << templateStr << "pattern a(_, _\"((x + y)\"_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}
	};
}