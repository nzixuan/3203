#include <memory>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "AST.h"
#include "SimpleParser.h"
#include "Token.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ParserTest)
	{
	public:

		CFGParser* parser;

		TEST_METHOD(CFGtestProcedureParsing)
		{
			/*
			* SIMPLE test code *
			procedure testProcedure {}
			*/
			std::list<Token> tokens = {
				{ TokenType::Identifier, "procedure" },
				{ TokenType::Identifier, "testProcedure" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Delimiter, "}" },
			};

			// Initialize Parser
			parser = new CFGParser(tokens);

			// Generate AST
			parser->generateCFG();

			// Get CFG list
			std::list<CFGRoot*> CFGList = parser->returnCFGList();

			// Check size of CFG list
			Assert::IsTrue(CFGList.size() == 1);

			// Check the name of CFGRoot
			Assert::IsTrue(CFGList.front()->getProcedureName() == "testProcedure");
		}

		TEST_METHOD(CFGtestTwoProcedures)
		{
			/*
			* SIMPLE test code *
			procedure testProcedure1 {}
			procedure testProcedure2 {}
			*/
			std::list<Token> tokens = {
				{ TokenType::Identifier, "procedure" },
				{ TokenType::Identifier, "testProcedure1" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Delimiter, "}" },
				{ TokenType::Identifier, "procedure" },
				{ TokenType::Identifier, "testProcedure2" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Delimiter, "}" },
			};

			// Initialize Parser
			parser = new CFGParser(tokens);

			// Generate AST
			parser->generateCFG();

			// Get CFG list
			std::list<CFGRoot*> CFGList = parser->returnCFGList();

			// Check size of CFG list
			Assert::IsTrue(CFGList.size() == 2);

			// Check the name of CFGRoot
			Assert::IsTrue(CFGList.front()->getProcedureName() == "testProcedure1");
		}

		TEST_METHOD(CFGtestPopulatingStmts)
		{
			/*
			* SIMPLE test code *
			procedure testProcedure {
				read file;
				print line;
				call procedure2;
			}
			*/

			std::list<Token> tokens = {
				{ TokenType::Identifier, "procedure" },
				{ TokenType::Identifier, "testProcedure" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "read"},
				{ TokenType::Identifier, "file"},
				{ TokenType::Delimiter, ";" },
				{ TokenType::Identifier, "print"},
				{ TokenType::Identifier, "line"},
				{ TokenType::Delimiter, ";" },
				{ TokenType::Identifier, "call"},
				{ TokenType::Identifier, "procedure2"},
				{ TokenType::Delimiter, ";" },
				{ TokenType::Delimiter, "}" },
			};

			// Initialize Parser
			parser = new CFGParser(tokens);

			// Generate AST
			parser->generateCFG();

			// Get CFG list
			std::list<CFGRoot*> CFGList = parser->returnCFGList();

			// Check the name of CFGRoot
			CFGRoot* test = CFGList.front();
			Assert::IsTrue(test->getProcedureName() == "testProcedure");

			// Check the statements
			CFGNode* node = (CFGNode*)test->getNext();
			std::vector<int> stmtList = node->getStmts();
			Assert::IsTrue(stmtList.size() == 3);

			// Check the statement numbers
			Assert::IsTrue(stmtList.front() == 1);
			Assert::IsTrue(test->getLast() == 3);
		}

		TEST_METHOD(CFGtestIf)
		{
			/*
			* SIMPLE test code *
			procedure testProcedure {
			1	x = 6;
			2	if (x<5) then {
			3		x = 5;
			4		x = 10;
				} else {
			5		x = 7;
			6		x = 9;
				}
			7	x=8;
			}
			*/
			std::list<Token> tokens = {
				{ TokenType::Identifier, "procedure" },
				{ TokenType::Identifier, "testProcedure" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "x"},
				{ TokenType::Operator, "="},
				{ TokenType::Number, "6"},
				{ TokenType::Delimiter, ";"},
				{ TokenType::Identifier, "if" },
				{ TokenType::Delimiter, "(" },
				{ TokenType::Identifier, "x"},
				{ TokenType::ComparisonOperator, "<"},
				{ TokenType::Number, "5"},
				{ TokenType::Delimiter, ")" },
				{ TokenType::Identifier, "then" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "x"},
				{ TokenType::Operator, "="},
				{ TokenType::Number, "5"},
				{ TokenType::Delimiter, ";"},
				{ TokenType::Identifier, "x"},
				{ TokenType::Operator, "="},
				{ TokenType::Number, "10"},
				{ TokenType::Delimiter, ";"},
				{ TokenType::Delimiter, "}" },
				{ TokenType::Identifier, "else" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "x"},
				{ TokenType::Operator, "="},
				{ TokenType::Number, "7"},
				{ TokenType::Delimiter, ";"},
				{ TokenType::Identifier, "x"},
				{ TokenType::Operator, "="},
				{ TokenType::Number, "9"},
				{ TokenType::Delimiter, ";"},
				{ TokenType::Delimiter, "}" },
				{ TokenType::Identifier, "x"},
				{ TokenType::Operator, "="},
				{ TokenType::Number, "8"},
				{ TokenType::Delimiter, ";"},
				{ TokenType::Delimiter, "}" },
			};

			// Initialize Parser
			parser = new CFGParser(tokens);

			// Generate AST
			parser->generateCFG();

			// Get CFG list
			std::list<CFGRoot*> CFGList = parser->returnCFGList();

			// Check the name of CFGRoot
			CFGRoot* test = CFGList.front();
			Assert::IsTrue(test->getProcedureName() == "testProcedure");

			// Check the first node
			CFGNode* node = (CFGNode*)test->getNext();
			std::vector<int> stmtList = node->getStmts();
			Assert::IsTrue(stmtList.size() == 1);

			// Check the statement numbers
			Assert::IsTrue(stmtList.front() == 1);
			Assert::IsTrue(test->getLast() == 7);

			// Check the if node
			CFGIf* ifNode = (CFGIf*)node->getNext();
			std::vector<int> ifStmtList = ifNode->getStmts();
			Assert::IsTrue(ifStmtList.size() == 1);
			Assert::IsTrue(ifStmtList.front() == 2);

			// Check the then branch
			CFGNode* thenNode = (CFGNode*)ifNode->getThen();
			std::vector<int> thenStmtList = thenNode->getStmts();
			Assert::IsTrue(thenStmtList.size() == 2);
			Assert::IsTrue(thenStmtList.front() == 3);
			Assert::IsTrue(thenStmtList[1] == 4);
			Assert::IsTrue(thenNode->getNext()->getStmts().size() == 1);

			// Check the else branch
			CFGNode* elseNode = (CFGNode*)ifNode->getElse();
			std::vector<int> elseStmtList = elseNode->getStmts();
			Assert::IsTrue(elseStmtList.size() == 2);
			Assert::IsTrue(elseStmtList.front() == 5);
			Assert::IsTrue(elseStmtList[1] == 6);
			Assert::IsTrue(elseNode->getNext()->getStmts().size() == 1);
		}

		TEST_METHOD(CFGtestWhile1)
		{
			/*
			* SIMPLE test code *
			procedure testProcedure {
				while (x<5) {
					x = 5;
				}
			}
			*/
			std::list<Token> tokens = {
				{ TokenType::Identifier, "procedure" },
				{ TokenType::Identifier, "testProcedure" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "while" },
				{ TokenType::Delimiter, "(" },
				{ TokenType::Identifier, "x"},
				{ TokenType::ComparisonOperator, "<"},
				{ TokenType::Number, "5"},
				{ TokenType::Delimiter, ")" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "x"},
				{ TokenType::Operator, "="},
				{ TokenType::Number, "5"},
				{ TokenType::Delimiter, ";"},
				{ TokenType::Delimiter, "}" },
				{ TokenType::Delimiter, "}" },
			};

			// Initialize Parser
			parser = new CFGParser(tokens);

			// Generate AST
			parser->generateCFG();

			// Get CFG list
			std::list<CFGRoot*> CFGList = parser->returnCFGList();

			// Check the name of CFGRoot
			CFGRoot* test = CFGList.front();
			Assert::IsTrue(test->getProcedureName() == "testProcedure");

			// Check the while node
			CFGWhile* whileNode = (CFGWhile*)test->getNext();
			std::vector<int> whileStmtList = whileNode->getStmts();
			Assert::IsTrue(whileStmtList.size() == 1);
			Assert::IsTrue(whileStmtList.front() == 1);

			// Check the then branch
			CFGNode* thenNode = (CFGNode*)whileNode->getWhile();
			std::vector<int> thenStmtList = thenNode->getStmts();
			Assert::IsTrue(thenStmtList.size() == 1);
			Assert::IsTrue(thenStmtList.front() == 2);
			Assert::IsTrue(thenNode->getNext()->getStmts().size() == 1);
			Assert::IsTrue(thenNode->getNext()->getStmts().front() == 1);
		}

		TEST_METHOD(CFGtestWhile2)
		{
			/*
			* SIMPLE test code *
			procedure testProcedure {
				x = 6;
				while (x<5) {
					x = 5;
				}
				x = 4;
			}
			*/
			std::list<Token> tokens = {
				{ TokenType::Identifier, "procedure" },
				{ TokenType::Identifier, "testProcedure" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "x"},
				{ TokenType::Operator, "="},
				{ TokenType::Number, "6"},
				{ TokenType::Delimiter, ";"},
				{ TokenType::Identifier, "while" },
				{ TokenType::Delimiter, "(" },
				{ TokenType::Identifier, "x"},
				{ TokenType::ComparisonOperator, "<"},
				{ TokenType::Number, "5"},
				{ TokenType::Delimiter, ")" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "x"},
				{ TokenType::Operator, "="},
				{ TokenType::Number, "5"},
				{ TokenType::Delimiter, ";"},
				{ TokenType::Delimiter, "}" },
				{ TokenType::Identifier, "x"},
				{ TokenType::Operator, "="},
				{ TokenType::Number, "4"},
				{ TokenType::Delimiter, ";"},
				{ TokenType::Delimiter, "}" },
			};

			// Initialize Parser
			parser = new CFGParser(tokens);

			// Generate AST
			parser->generateCFG();

			// Get CFG list
			std::list<CFGRoot*> CFGList = parser->returnCFGList();

			// Check the name of CFGRoot
			CFGRoot* test = CFGList.front();
			Assert::IsTrue(test->getProcedureName() == "testProcedure");

			// Check the first node
			CFGNode* node = (CFGNode*)test->getNext();
			std::vector<int> stmtList = node->getStmts();
			Assert::IsTrue(stmtList.size() == 1);

			// Check the statement numbers
			Assert::IsTrue(stmtList.front() == 1);
			Assert::IsTrue(test->getLast() == 4);

			// Check the while node
			CFGWhile* whileNode = (CFGWhile*)node->getNext();
			std::vector<int> whileStmtList = whileNode->getStmts();
			Assert::IsTrue(whileStmtList.size() == 1);
			Assert::IsTrue(whileStmtList.front() == 2);

			// Check the then branch
			CFGNode* thenNode = (CFGNode*)whileNode->getWhile();
			std::vector<int> thenStmtList = thenNode->getStmts();
			Assert::IsTrue(thenStmtList.size() == 1);
			Assert::IsTrue(thenStmtList.front() == 3);
			Assert::IsTrue(thenNode->getNext()->getStmts().size() == 1);

			// Check the while Next
			CFGNode* whileNext = (CFGNode*)whileNode->getNext();
			std::vector<int> whileNextStmtList = whileNext->getStmts();
			Assert::IsTrue(whileNextStmtList.size() == 1);
			Assert::IsTrue(whileNextStmtList.front() == 4);
		}

		TEST_METHOD(CFGtestNested)
		{
			/*
			* SIMPLE test code *
			procedure testProcedure {
			1	x = 6;
			2	while (x<5) {
			3		if (x<3) then {
			4			x = 7;
			5			while (x<7) {
			6				x = 8
						}
			7			x = 8;
					} else {
			8			x = 9;
					}
				}
			9	x = 4;
			}
			*/
			Tokenizer tokenizer;

			std::string exampleDelimiters = "procedure testProcedure { x=6; while(x<5){ if(x<3)then{ x=7; while(x<7){ x=8; } x=8; }else{ x=9; }} x=4; }";

			std::stringstream ss;
			ss << exampleDelimiters;

			const auto tokens = tokenizer.tokenize(ss);

			// Initialize Parser
			parser = new CFGParser(tokens);

			// Generate AST
			parser->generateCFG();

			// Get CFG list
			std::list<CFGRoot*> CFGList = parser->returnCFGList();

			// Check the name of CFGRoot
			CFGRoot* test = CFGList.front();
			Assert::IsTrue(test->getProcedureName() == "testProcedure");

			// Check the first node
			CFGNode* node = (CFGNode*)test->getNext();
			std::vector<int> stmtList = node->getStmts();
			Assert::IsTrue(stmtList.size() == 1);

			// Check the statement numbers
			Assert::IsTrue(stmtList.front() == 1);
			Assert::IsTrue(test->getLast() == 9);

			// Check the while node
			CFGWhile* whileNode = (CFGWhile*)node->getNext();
			std::vector<int> whileStmtList = whileNode->getStmts();
			Assert::IsTrue(whileStmtList.size() == 1);
			Assert::IsTrue(whileStmtList.front() == 2);

			// Check the then branch
			CFGIf* ifNode = (CFGIf*)whileNode->getWhile();
			std::vector<int> ifStmtList = ifNode->getStmts();
			Assert::IsTrue(ifStmtList.size() == 1);
			Assert::IsTrue(ifStmtList.front() == 3);
			
			CFGNode* ifThenNode = (CFGNode*)ifNode->getThen();
			std::vector<int> ifThenStmtList = ifThenNode->getStmts();
			Assert::IsTrue(ifThenStmtList.size() == 1);
			Assert::IsTrue(ifThenStmtList.front() == 4);

			// Check the else branch
			CFGNode* ifElseNode = (CFGNode*)ifNode->getElse();
			std::vector<int> ifElseStmtList = ifElseNode->getStmts();
			Assert::IsTrue(ifElseStmtList.size() == 1);
			Assert::IsTrue(ifElseStmtList.front() == 8);

			Assert::IsTrue(ifElseNode->getNext()->getStmts().size() == 1);
			Assert::IsTrue(ifElseNode->getNext()->getStmts().front() == 2);

			// Check nested while node
			CFGWhile* nestedWhileNode = (CFGWhile*)ifThenNode->getNext();
			std::vector<int> nestedWhileStmtList = nestedWhileNode->getStmts();
			Assert::IsTrue(nestedWhileStmtList.size() == 1);
			Assert::IsTrue(nestedWhileStmtList.front() == 5);

			CFGNode* nestedWhileThenNode = (CFGNode*)nestedWhileNode->getWhile();
			std::vector<int> nestedWhileThenStmtList = nestedWhileThenNode->getStmts();
			Assert::IsTrue(nestedWhileThenStmtList.size() == 1);
			Assert::IsTrue(nestedWhileThenStmtList.front() == 6);

			CFGNode* nestedWhileNextNode = (CFGNode*)nestedWhileNode->getNext();
			std::vector<int> nestedWhileNextStmtList = nestedWhileNextNode->getStmts();
			Assert::IsTrue(nestedWhileNextStmtList.size() == 1);
			Assert::IsTrue(nestedWhileNextStmtList.front() == 7);

			Assert::IsTrue(nestedWhileNextNode->getNext()->getStmts().size() == 1);
			Assert::IsTrue(nestedWhileNextNode->getNext()->getStmts().front() == 2);
		}
	
		TEST_METHOD(CFGTwoWhileInsideWhile)
		{
			/* 
			procedure testProcedure {
				while (cat < dog) { 
					while (dog == dog) { 
						cat = cat + 3;
						read dog;
						call ifAndWhileInsideWhile;
					}
					while (cat < (cat + 5)) {
						print cat;
					}
				}
			}";
			*/

			Tokenizer tokenizer;

			std::string exampleDelimiters = "procedure testProcedure{while (cat < dog) {while (dog == dog) {cat = cat + 3;"
				"read dog;call ifAndWhileInsideWhile;}while (cat < (cat + 5)) {print cat;}}}";

			std::stringstream ss;
			ss << exampleDelimiters;

			const auto tokens = tokenizer.tokenize(ss);

			// Initialize Parser
			parser = new CFGParser(tokens);

			// Generate AST
			parser->generateCFG();

			// Get CFG list
			std::list<CFGRoot*> CFGList = parser->returnCFGList();

			// Check the name of CFGRoot
			CFGRoot* test = CFGList.front();
			Assert::IsTrue(test->getProcedureName() == "testProcedure");

			// Check the first node
			CFGNode* node = (CFGNode*)test->getNext();
			std::vector<int> stmtList = node->getStmts();
			Assert::IsTrue(stmtList.size() == 1);

			// Check the statement numbers
			Assert::IsTrue(stmtList.front() == 1);
			Assert::IsTrue(test->getLast() == 7);
		}

		TEST_METHOD(CFGTwoIfInsideIf)
		{
			/*
			procedure testProcedure {
				if (cat < dog) {
					if (dog == dog) then {
						cat = cat + 3;
						read dog;
						call ifAndWhileInsideWhile;
					} else {
						cat = dog;
					}

					if (cat < (cat + 5)) then {
						print cat;
					}

					cat = cat + 3;

				} else {
					call doggy;
				}
				dog = dog + 1;
			}
			*/

			Tokenizer tokenizer;

			std::string exampleDelimiters = "procedure testProcedure {if (cat < dog) then {if (dog == dog) then {cat = cat + 3;"
				"read dog;call ifAndWhileInsideWhile;} else {cat = dog;}if (cat < (cat + 5)) then {print cat;}cat = cat + 3;}"
				"else {call doggy;}dog = dog + 1;}";

			std::stringstream ss;
			ss << exampleDelimiters;

			const auto tokens = tokenizer.tokenize(ss);

			// Initialize Parser
			parser = new CFGParser(tokens);

			// Generate AST
			parser->generateCFG();

			// Get CFG list
			std::list<CFGRoot*> CFGList = parser->returnCFGList();

			// Check the name of CFGRoot
			CFGRoot* test = CFGList.front();
			Assert::IsTrue(test->getProcedureName() == "testProcedure");

			// Check the first node
			CFGNode* node = (CFGNode*)test->getNext();
			std::vector<int> stmtList = node->getStmts();
			Assert::IsTrue(stmtList.size() == 1);

			// Check the statement numbers
			Assert::IsTrue(stmtList.front() == 1);
			Assert::IsTrue(test->getLast() == 11);
		}
	};
}