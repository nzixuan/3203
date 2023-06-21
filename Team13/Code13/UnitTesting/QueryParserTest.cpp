#include <list>
#include <string>
#include "stdafx.h"
#include "CppUnitTest.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(QueryParserTest)
	{
		Tokenizer tokenizer;
		std::string selectSyn1 = "s";
		std::string selectSyn2 = "s2";
		std::string selectSyn3 = "r";
		std::string selectSyn4 = "c";
		std::string attrRef1 = "s2.stmt#";
		std::string attrRef2 = "r.varName";
		std::string attrRef3 = "c.value";
		std::string templateStr = "stmt s, s2; read r; constant c; Select ";
		ClauseArgs ca;
		std::map<std::string, EntityType> expectedMap = {
			{selectSyn1, EntityType::Stmt},
			{selectSyn2, EntityType::Stmt},
			{selectSyn3, EntityType::Read},
			{selectSyn4, EntityType::Constant},
		};
		Query expectedQuery;
	public:
		TEST_METHOD_INITIALIZE(initializeFollowsQuery) {
			// Reinitalize the query after every method
			expectedQuery = {};
			ca = ClauseArgs{};
		}

		/* Valid Extractions */
		TEST_METHOD(testDeclarationOneSynonym) {
			// Test if "DesignEntity synName;" is valid. Should work for all DesignEntity
			std::list<Token> designEntities = {
				{ TokenType::Identifier, "stmt" },
				{ TokenType::Identifier, "read" },
				{ TokenType::Identifier, "print" },
				{ TokenType::Identifier, "while" },
				{ TokenType::Identifier, "if" },
				{ TokenType::Identifier, "assign" },
				{ TokenType::Identifier, "variable" },
				{ TokenType::Identifier, "constant" },
				{ TokenType::Identifier, "procedure" },
			};

			// Synonym should not matter regardless of design entity
			std::string syn = "synName";

			for (auto const& designEnt : designEntities) {
				std::stringstream ss;
				ss << designEnt.val << " " << syn << "; Select " << syn;
				std::list<Token> testTokens = tokenizer.tokenize(ss);

				QueryParser parser(testTokens);
				Query query = parser.parseQuery();

				Assert::IsTrue(query.getSelectArgs().size() == 1);
				Assert::IsTrue(**query.getSelectArgs().begin() == Synonym{ syn, tokenToEntMap.at(designEnt) });
			}
		}

		TEST_METHOD(testDesignEntityManySynonym) {
			// Test if "DesignEntity synName1, synName2, synName3;" is valid. Should work for all DesignEntity
			std::list<Token> designEntities = {
				{ TokenType::Identifier, "stmt" },
				{ TokenType::Identifier, "read" },
				{ TokenType::Identifier, "print" },
				{ TokenType::Identifier, "while" },
				{ TokenType::Identifier, "if" },
				{ TokenType::Identifier, "assign" },
				{ TokenType::Identifier, "variable" },
				{ TokenType::Identifier, "constant" },
				{ TokenType::Identifier, "procedure" },
			};

			// Synonym should not matter regardless of design entity
			std::string syn1 = "synName1";
			std::string syn2 = "synName2";
			std::string syn3 = "synName3";

			for (auto const& designEnt : designEntities) {

				std::stringstream ss;
				ss << designEnt.val << " " << syn1 << ","
					<< syn2 << "," << syn3 << "; Select <"
					<< syn1 << "," << syn2 << "," << syn3 << ">";
				std::list<Token> testTokens = tokenizer.tokenize(ss);

				std::map<std::string, EntityType> expectedMap = {
					{syn1, tokenToEntMap.at(designEnt)},
					{syn2, tokenToEntMap.at(designEnt)},
					{syn3, tokenToEntMap.at(designEnt)}
				};

				QueryParser parser(testTokens);
				Query query = parser.parseQuery();

				Assert::IsTrue(query.getSelectArgs().size() == 3);
				for (const auto& syn : query.getSelectArgs()) {
					Assert::IsTrue(
						*syn == Synonym{ syn1, tokenToEntMap.at(designEnt) } ||
						*syn == Synonym{ syn2, tokenToEntMap.at(designEnt) } ||
						*syn == Synonym{ syn3, tokenToEntMap.at(designEnt) }
					);
				}
			}
		}

		TEST_METHOD(testSelectSynonym) {
			// Test if we can select a single synonym
			std::stringstream ss;
			ss << templateStr << selectSyn1;
			std::list<Token> testTokens = tokenizer.tokenize(ss);
			
			expectedQuery.addSelectArg(Synonym{ selectSyn1, expectedMap.at(selectSyn1) });
			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testSelectAttrRef) {
			// Test if we can select a single AttrRef
			std::stringstream ss;
			ss << templateStr << attrRef1;
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			expectedQuery.addSelectArg(AttrRef{ {selectSyn2, expectedMap.at(selectSyn2)}, AttrName::StmtNo });
			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testSelectAllValidAttrRefs) {
			// All valid attrRefs
			std::string declStr = "stmt s; read r; print pr; call c; while w; if ifs; assign a; variable v; constant cst; procedure p; ";
			std::string validAttr = "Select <p.procName, c.procName, v.varName, r.varName, pr.varName, cst.value, ";
			std::string validAttr2 = "s.stmt#, r.stmt#, pr.stmt#, c.stmt#, w.stmt#, ifs.stmt#, a.stmt#>";
			std::stringstream ss;
			ss << declStr << validAttr << validAttr2;
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			expectedQuery.addSelectArg(AttrRef{ {"p", EntityType::Procedure}, AttrName::ProcName});
			expectedQuery.addSelectArg(AttrRef{ {"c", EntityType::Call}, AttrName::ProcName });
			expectedQuery.addSelectArg(AttrRef{ {"v", EntityType::Variable}, AttrName::VarName });
			expectedQuery.addSelectArg(AttrRef{ {"r", EntityType::Read}, AttrName::VarName });
			expectedQuery.addSelectArg(AttrRef{ {"pr",EntityType::Print}, AttrName::VarName });
			expectedQuery.addSelectArg(AttrRef{ {"cst",EntityType::Constant}, AttrName::Value });
			expectedQuery.addSelectArg(AttrRef{ {"s", EntityType::Stmt}, AttrName::StmtNo });
			expectedQuery.addSelectArg(AttrRef{ {"r", EntityType::Read}, AttrName::StmtNo });
			expectedQuery.addSelectArg(AttrRef{ {"pr", EntityType::Print}, AttrName::StmtNo });
			expectedQuery.addSelectArg(AttrRef{ {"c", EntityType::Call}, AttrName::StmtNo });
			expectedQuery.addSelectArg(AttrRef{ {"w", EntityType::While}, AttrName::StmtNo });
			expectedQuery.addSelectArg(AttrRef{ {"ifs",EntityType::If}, AttrName::StmtNo });
			expectedQuery.addSelectArg(AttrRef{ {"a", EntityType::Assign}, AttrName::StmtNo });

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testSelectSynonymTuple) {
			// Test if we can select a tuple of synonyms
			std::string tupleSyn = "<" + selectSyn1 + "," + selectSyn2 + "," + selectSyn3 + ">";
			std::stringstream ss;
			ss << templateStr << tupleSyn;
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			expectedQuery.addSelectArg(Synonym{ selectSyn1, expectedMap.at(selectSyn1) });
			expectedQuery.addSelectArg(Synonym{ selectSyn2, expectedMap.at(selectSyn2) });
			expectedQuery.addSelectArg(Synonym{ selectSyn3, expectedMap.at(selectSyn3) });
			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testSelectBOOLEAN) {
			// Test if we can select BOOLEAN
			std::stringstream ss;
			ss << templateStr << "BOOLEAN";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			expectedQuery.setBooleanSelectFlag();
			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testSelectSynonymBOOLEAN) {
			// Test if we can select BOOLEAN which is declared as a synonym
			std::map<std::string, EntityType> newMap = expectedMap;

			std::stringstream ss;
			ss << "stmt s1, s2, BOOLEAN; read r; Select BOOLEAN";
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			expectedQuery.addSelectArg(Synonym{ "BOOLEAN", EntityType::Stmt});
			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testInvalidSelectSynonymTupleUndeclaredBOOLEAN) {
			// Test if error is thrown when trying to select tuple and BOOLEAN, BOOLEAN is undeclared.
			std::string tupleSyn = "<" + selectSyn1 + "," + "BOOLEAN" + "," + selectSyn3 + ">";
			std::stringstream ss;
			ss << templateStr << tupleSyn;
			std::list<Token> testTokens = tokenizer.tokenize(ss);

			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		}

		TEST_METHOD(testParseQueryPatternSuchThat) {
			/* Tests if a query can be parsed successfully with both such that 
			and pattern clauses */

			std::stringstream ss;
			ss << "assign a; Select a such that Follows(_,_) pattern a(_,_)";
			std::list<Token> testTokens = tokenizer.tokenize(ss);
			
			std::string expectedSyn = "a";
			std::map<std::string, EntityType> expectedMap = {
				{expectedSyn, EntityType::Assign}
			};

			ca.addRelArg(wildcard);
			ca.addRelArg(wildcard);
			SuchThat expectedSuchThat{ ca, RelRefType::Follows };
			Pattern expectedPattern{ ca, {expectedSyn, EntityType::Assign}};
			Query expectedQuery{};
			
			expectedQuery.addSelectArg(Synonym{ expectedSyn, EntityType::Assign });
			addST(expectedQuery, expectedSuchThat);
			addP(expectedQuery, expectedPattern);

			assertQueryEqual(expectedQuery, testTokens);
		}

		TEST_METHOD(testClauseArgs) {
			// Test clauseArgs if synonyms exist
			ClauseArgs ca;
			ca.addRelArg(assignSyn);
			ca.addRelArg(AttrRef{ assignSyn, AttrName::StmtNo });
			ca.addRelArg(constSyn);
			
			// This clause is not syntatically possible. However, it fits the purpose of this test.
			Clause dummyClauseWithSyn = SuchThat{ ca, RelRefType::Follows };
			std::set<Synonym> expectedSet{ assignSyn, constSyn };

			Assert::IsTrue(dummyClauseWithSyn.isUsingSynonyms());
			Assert::IsTrue(dummyClauseWithSyn.getSynonyms() == expectedSet);

			ca = ClauseArgs{};
			ca.addRelArg(number);
			ca.addRelArg(number2);
			ca.addRelArg(SourceArg{"notSynonym"});
			Clause dummyClauseWithoutSyn = SuchThat{ ca, RelRefType::Follows };

			Assert::IsTrue(!dummyClauseWithoutSyn.isUsingSynonyms());
			Assert::IsTrue(dummyClauseWithoutSyn.getSynonyms().empty());
		}

		/* Invalid Extractions */
		TEST_METHOD(testQueryBadSyntax) {
			std::list<Token> testTokens = {
				PQLToken::Bracket::LeftBracket
			};
			auto func = [=] {
				QueryParser parser(testTokens);
				parser.parseQuery();
			};
			Assert::ExpectException<std::logic_error>(func);
		
		}

		TEST_METHOD(testAttrRefWrongAttrName) {
			// AttrName does not match the synonym.
			std::string declStr = "stmt s; read r; print pr; call c; while w; if ifs; assign a; variable v; constant cst; procedure p; ";
			std::list<std::string> invalidAttrRef = {
				"p.varName", "p.value", "p.stmt#",
				"c.varName", "c.value",
				"v.procName", "v.value", "v.stmt#"
				"r.procName", "r.value",
				"pr.procName", "pr.value",
				"cst.procName", "cst.varName", "cst.stmt#"
				"s.procName", "s.varName", "s.value"
				"r.procName", "r.value",
				"pr.procName", "pr.value",
				"c.varName", "c.value",
				"w.procName", "w.varName", "w.value"
				"ifs.procName", "ifs.varName", "ifs.value"
				"a.procName", "a.varName", "a.value"
			};

			for (auto const& attrRef : invalidAttrRef) {

				std::stringstream ss;
				ss << declStr << "Select " << attrRef;
				std::list<Token> testTokens = tokenizer.tokenize(ss);

				auto func = [=] {
					QueryParser parser(testTokens);
					parser.parseQuery();
				};
				Assert::ExpectException<std::logic_error>(func);
			}
		}
	};
}