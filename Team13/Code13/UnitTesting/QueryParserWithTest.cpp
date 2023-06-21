#include "stdafx.h"
#include "CppUnitTest.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(QueryParserWithTest)
	{
	public:
		Tokenizer tokenizer;
		// Template string such that query is valid
		std::string decl = "stmt s; read r; print pr; call c; while w; if ifs; assign a; variable v; constant const; procedure p; ";
		std::string templateStr = decl + "Select " + assignSyn.getVal() + " with ";
		std::string varName = "varName";
		std::string procName = "procName";
		std::string value = "value";
		std::string stmtNo = "stmt#";
		std::string str = "var";

		ClauseArgs ca;
		Query expectedQuery;

		TEST_METHOD_INITIALIZE(initializeWithQuery) {
			// Reinitalize the query after every method
			expectedQuery = {};
			expectedQuery.addSelectArg(assignSyn);
			ca = ClauseArgs{};
		}

		/* Valid Extractions */
		// W = wildcard '_', S = synonym, I = integer
		// WI = wildcard on lhs, integer on rhs, and so on for the others.
		TEST_METHOD(testExtractWithVarNameSrcArg) {
			// with v.varName = "var"
			std::stringstream ss;
			ss << templateStr << varSyn.getVal() << "." << varName << "="
				<< "\"" << str << "\"";;
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(AttrRef{ varSyn, AttrName::VarName});
			ca.addRelArg(SourceArg{str});
			With expectedWith{ ca, WithRefType::Name };

			addW(expectedQuery, expectedWith);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractWithVarNameVarName) {
			// with v.varName = r.varName
			std::stringstream ss;
			ss << templateStr << varSyn.getVal() << "." << varName << "=" 
				<< readSyn.getVal() << "." << varName;
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(AttrRef{ varSyn, AttrName::VarName });
			ca.addRelArg(AttrRef{ readSyn, AttrName::VarName });
			With expectedWith{ ca, WithRefType::Name };

			addW(expectedQuery, expectedWith);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractWithVarNameProcName) {
			// with v.varName = p.procName
			std::stringstream ss;
			ss << templateStr << varSyn.getVal() << "." << varName << "="
				<< procSyn.getVal() << "." << procName;
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(AttrRef{ varSyn, AttrName::VarName });
			ca.addRelArg(AttrRef{ procSyn, AttrName::ProcName });
			With expectedWith{ ca, WithRefType::Name };

			addW(expectedQuery, expectedWith);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractWithProcNameSrcArg) {
			// with p.procName = "var"
			std::stringstream ss;
			ss << templateStr << procSyn.getVal() << "." << procName << "="
				<< "\"" << str << "\"";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(AttrRef{ procSyn, AttrName::ProcName });
			ca.addRelArg(SourceArg{str});
			With expectedWith{ ca, WithRefType::Name };

			addW(expectedQuery, expectedWith);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractWithProcNameProcName) {
			// with p.procName = c.procName
			std::stringstream ss;
			ss << templateStr << procSyn.getVal() << "." << procName << "="
				<< callSyn.getVal() << "." << procName;
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(AttrRef{ procSyn, AttrName::VarName });
			ca.addRelArg(AttrRef{ callSyn, AttrName::ProcName });
			With expectedWith{ ca, WithRefType::Name };

			addW(expectedQuery, expectedWith);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractWithValueNumber) {
			// with const.value = 1
			std::stringstream ss;
			ss << templateStr << constSyn.getVal() << "." << value << "="
				<< number.getVal();
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(AttrRef{ constSyn, AttrName::Value });
			ca.addRelArg(number);
			With expectedWith{ ca, WithRefType::Integer };

			addW(expectedQuery, expectedWith);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractWithValueValue) {
			// with const.value = const.value
			std::stringstream ss;
			ss << templateStr << constSyn.getVal() << "." << value << "="
				<< constSyn.getVal() << "." << value;
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(AttrRef{ constSyn, AttrName::Value });
			ca.addRelArg(AttrRef{ constSyn, AttrName::Value });
			With expectedWith{ ca, WithRefType::Integer };

			addW(expectedQuery, expectedWith);

			assertQueryEqual(expectedQuery, testTokens);
		}


		TEST_METHOD(testExtractWithValueStmtNo) {
			// with const.value = a.stmt#
			std::stringstream ss;
			ss << templateStr << constSyn.getVal() << "." << value << "="
				<< assignSyn.getVal() << "." << stmtNo;
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(AttrRef{ constSyn, AttrName::Value });
			ca.addRelArg(AttrRef{ assignSyn, AttrName::StmtNo });
			With expectedWith{ ca, WithRefType::Integer };

			addW(expectedQuery, expectedWith);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractWithStmtNumNumber) {
			// with a.stmt# = 1
			std::stringstream ss;
			ss << templateStr << assignSyn.getVal() << "." << stmtNo << "="
				<< number.getVal();
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(AttrRef{ assignSyn, AttrName::StmtNo });
			ca.addRelArg(number);
			With expectedWith{ ca, WithRefType::Integer };

			addW(expectedQuery, expectedWith);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testExtractWithStmtNoStmtNo) {
			// with a.stmt# = r.stmt#
			std::stringstream ss;
			ss << templateStr << assignSyn.getVal() << "." << stmtNo << "="
				<< readSyn.getVal() << "." << stmtNo;
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(AttrRef{ assignSyn, AttrName::StmtNo });
			ca.addRelArg(AttrRef{ readSyn, AttrName::StmtNo });
			With expectedWith{ ca, WithRefType::Integer };

			addW(expectedQuery, expectedWith);

			assertQueryEqual(expectedQuery, testTokens);
		}

		
		TEST_METHOD(testExtractWithSynIsClauseKeyword) {
            /* Synonym used is a clause keyword, 'and', 'such', 'pattern'
            */
            std::stringstream ss;
            ss << "stmt and; constant such; while pattern; Select and "
                "with " << "and.stmt# = 1";
            std::list<Token> testTokens = tokenizer.tokenize(ss);

			ca.addRelArg(AttrRef{Synonym{"and", EntityType::Stmt}, AttrName::StmtNo});
            ca.addRelArg(Number{"1"});
            With expectedWith{ca, WithRefType::Integer};

			expectedQuery = {};
			expectedQuery.addSelectArg(Synonym{ "and", EntityType::Stmt });
            addW(expectedQuery, expectedWith);

            assertQueryEqual(expectedQuery, testTokens);

			ss = std::stringstream();
			ss << "stmt and; constant such; while pattern; Select such "
				"with " << "such.value = 1";
			testTokens = tokenizer.tokenize(ss);
			ca = ClauseArgs{};
            ca.addRelArg(AttrRef{Synonym{"such", EntityType::Constant}, AttrName::Value});
            ca.addRelArg(Number{"1"});
            expectedWith = With{ca, WithRefType::Integer};

			expectedQuery = {};
			expectedQuery.addSelectArg(Synonym{ "such", EntityType::Constant });
            addW(expectedQuery, expectedWith);

            assertQueryEqual(expectedQuery, testTokens);

			ss = std::stringstream();
			ss << "stmt and; constant such; read pattern; Select pattern "
				"with " << "pattern.varName = \"v\"";
			testTokens = tokenizer.tokenize(ss);

			ca = ClauseArgs{};
            ca.addRelArg(AttrRef{Synonym{"pattern", EntityType::Read}, AttrName::VarName});
            ca.addRelArg(SourceArg{"v"});
            expectedWith = With{ca, WithRefType::Integer};

			expectedQuery = {};
			expectedQuery.addSelectArg(Synonym{ "pattern", EntityType::Read });
            addW(expectedQuery, expectedWith);

            assertQueryEqual(expectedQuery, testTokens);
        }

		/* Invalid extractions */
		TEST_METHOD(testExtractWithInvalidTypes) {
			/* Both sides must be either NAME or INTEGER
			*/
			std::string decl = "stmt s; read r; print pr; call c; while w; if ifs; assign a; variable v; constant cst; procedure p; ";
			const std::set<std::string> invalidWith = {
				"1 = \"var\"",
				"p.procName = s.stmt#",
				"p.procName = 1",
				"\"var\" = s.stmt#",
				"v.varName = const.value",
				"v.varName = 1",
				"\"var\" = const.value",
			};

			for (const auto& str : invalidWith) {
				std::stringstream ss;
				ss << decl << "Select a with " << str;
				std::list<Token> testTokens = tokenizer.tokenize(ss);

				auto func = [=] {
					QueryParser parser(testTokens);
					parser.parseQuery();
				};
				Assert::ExpectException<std::logic_error>(func);
			};
		}

		TEST_METHOD(testExtractAttrRefInvalidTypes) {
			/* attrRef in with has invalid type
			*/
			std::string decl = "stmt s; read r; print pr; call c; while w; if ifs; assign a; variable v; constant cst; procedure p; ";
			const std::set<std::string> invalidWith = {
				"p.varName = \"var\"",
				"p.value = 1",
				"const.varName = \"var\"",
				"s.stmt# = \"var\""
			};

			for (const auto& str : invalidWith) {
				std::stringstream ss;
				ss << decl << "Select a with " << str;
				std::list<Token> testTokens = tokenizer.tokenize(ss);

				auto func = [=] {
					QueryParser parser(testTokens);
					parser.parseQuery();
				};
				Assert::ExpectException<std::logic_error>(func);
			};
		}

		TEST_METHOD(testExtractWithUndeclaredSyn) {
			/* Syn is undeclared
			*/
			std::stringstream ss;
			ss << templateStr << "someSyn.stmt# = 1";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testExtractWithSynNotAttrRefLHS) {
            /* LHS is a synonym, not attrRef
            */
            std::stringstream ss;
            ss << templateStr << "r = \"readSyn\"";
            std::list<Token> testTokens = tokenizer.tokenize(ss);

            auto func = [=] {
            QueryParser parser(testTokens);
            parser.parseQuery();
            };
            Assert::ExpectException<std::logic_error>(func);
        }

		TEST_METHOD(testExtractWithSynNotAttrRefRHS) {
          /* RHS is a synonym, not attrRef
           */
          std::stringstream ss;
          ss << templateStr << "\"readSyn\" = r";
          std::list<Token> testTokens = tokenizer.tokenize(ss);

          auto func = [=] {
            QueryParser parser(testTokens);
            parser.parseQuery();
          };
          Assert::ExpectException<std::logic_error>(func);
        }
	};
}