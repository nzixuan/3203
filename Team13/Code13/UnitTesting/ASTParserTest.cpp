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

		ASTParser* parser;

		TEST_METHOD(testProcedureParsing)
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
			parser = new ASTParser(tokens);

			// Generate AST
			parser->generateAST();

			// Get root node
			ASTRoot* generatedRoot = (ASTRoot*)parser->returnASTTree();

			// Get AST of testProcedure
			ProcedureNode* generatedAST = (ProcedureNode*)generatedRoot->getChildren().front();

			// Check if generated children is correct
			Assert::IsTrue(generatedAST->getChildren().front()->getChildren().size() == 0);

			std::string generated = generatedAST->getNodeName();

			// Check if the right Procedure is generated
			Assert::IsTrue(generated == "testProcedure");
		}

		TEST_METHOD(testRead)
		{
			/*
			* SIMPLE test code *
			procedure testProcedure {
				read file;
			}
			*/

			std::list<Token> tokens = {
				{ TokenType::Identifier, "procedure" },
				{ TokenType::Identifier, "testProcedure" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "read"},
				{ TokenType::Identifier, "file"},
				{ TokenType::Delimiter, ";" },
				{ TokenType::Delimiter, "}" },
			};

			// Initialize Parser
			parser = new ASTParser(tokens);

			// Generate AST
			parser->generateAST();

			// Get root node
			ASTRoot* generatedRoot = (ASTRoot*)parser->returnASTTree();

			// Get AST of testProcedure
			ProcedureNode* generatedAST = (ProcedureNode*)generatedRoot->getChildren().front();

			// Check if generated children is correct
			Assert::IsTrue(generatedAST->getChildren().front()->getChildren().size() == 1);

			std::string generated = generatedAST->getNodeName();

			// Check if the right Procedure is generated
			Assert::IsTrue(generated == "testProcedure");

			// Check if read is created
			ReadNode* generatedRead = (ReadNode*)generatedAST->getChildren().front()->getChildren().front();
			Assert::IsTrue(generatedRead->getReadTarget() == "file");
		}

		TEST_METHOD(testPrint)
		{
			/*
			* SIMPLE test code *
			procedure testProcedure {
				print file;
			}
			*/

			std::list<Token> tokens = {
				{ TokenType::Identifier, "procedure" },
				{ TokenType::Identifier, "testProcedure" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "print"},
				{ TokenType::Identifier, "file"},
				{ TokenType::Delimiter, ";" },
				{ TokenType::Delimiter, "}" },
			};

			// Initialize Parser
			parser = new ASTParser(tokens);

			// Generate AST
			parser->generateAST();

			// Get root node
			ASTRoot* generatedRoot = (ASTRoot*)parser->returnASTTree();

			// Get AST of testProcedure
			ProcedureNode* generatedAST = (ProcedureNode*)generatedRoot->getChildren().front();

			// Check if generated children is correct
			Assert::IsTrue(generatedAST->getChildren().front()->getChildren().size() == 1);

			std::string generated = generatedAST->getNodeName();

			// Check if the right Procedure is generated
			Assert::IsTrue(generated == "testProcedure");

			// Check if read is created
			PrintNode* generatedPrint = (PrintNode*)generatedAST->getChildren().front()->getChildren().front();
			Assert::IsTrue(generatedPrint->getPrintTarget() == "file");
		}

		TEST_METHOD(testCalls)
		{
			/*
			* SIMPLE test code *
			procedure testProcedure {
				call file;
			}
			*/

			std::list<Token> tokens = {
				{ TokenType::Identifier, "procedure" },
				{ TokenType::Identifier, "testProcedure" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "call"},
				{ TokenType::Identifier, "file"},
				{ TokenType::Delimiter, ";" },
				{ TokenType::Delimiter, "}" },
			};

			// Initialize Parser
			parser = new ASTParser(tokens);

			// Generate AST
			parser->generateAST();

			// Get root node
			ASTRoot* generatedRoot = (ASTRoot*)parser->returnASTTree();

			// Get AST of testProcedure
			ProcedureNode* generatedAST = (ProcedureNode*)generatedRoot->getChildren().front();

			// Check if generated children is correct
			Assert::IsTrue(generatedAST->getChildren().front()->getChildren().size() == 1);

			std::string generated = generatedAST->getNodeName();

			// Check if the right Procedure is generated
			Assert::IsTrue(generated == "testProcedure");

			// Check if read is created
			CallNode* generatedCall = (CallNode*)generatedAST->getChildren().front()->getChildren().front();
			Assert::IsTrue(generatedCall->getCallTarget() == "file");
		}

		TEST_METHOD(testAssignStmt)
		{
			/*
			* SIMPLE test code *
			procedure testProcedure {
				x = 5;
			}
			*/

			std::list<Token> tokens = {
				{ TokenType::Identifier, "procedure" },
				{ TokenType::Identifier, "testProcedure" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "x"},
				{ TokenType::Operator, "="},
				{ TokenType::Number, "5"},
				{ TokenType::Delimiter, ";"},
				{ TokenType::Delimiter, "}" },
			};

			// Initialize Parser
			parser = new ASTParser(tokens);

			// Generate AST
			parser->generateAST();

			// Get root node
			ASTRoot* generatedRoot = (ASTRoot*)parser->returnASTTree();

			// Get generated AST for testProcedure
			ProcedureNode* generatedAST = (ProcedureNode*)generatedRoot->getChildren().front();

			// Check if procedure name is correct
			Assert::IsTrue("testProcedure" == generatedAST->getNodeName());

			// Get testProcedure stmtList
			StmtListNode* generatedStmtList = (StmtListNode*)generatedAST->getChildren().front();

			// Check if stmtList has the right amount of children
			Assert::IsTrue(1 == generatedStmtList->getChildren().size());

			// Get simple assignment statement
			AssignmentNode* generatedAssignment = (AssignmentNode*)generatedStmtList->getChildren().front();

			// Check if assign statement number is correct
			Assert::IsTrue(1 == generatedAssignment->getStatementNumber());

			// Get lhs of assignment
			VariableNode* generatedAssignmentLHS = (VariableNode*)generatedAssignment->getChildren().front();

			Assert::IsTrue("x" == generatedAssignmentLHS->getVariable());

			// Get rhs of assignment
			std::list<ASTNode*> assignmentStmtList = generatedAssignment->getChildren();
			assignmentStmtList.pop_front();
			ConstantNode* generatedAssignmentRHS = (ConstantNode*)assignmentStmtList.front();

			Assert::IsTrue(5 == generatedAssignmentRHS->getConstant());
		}

		TEST_METHOD(testIfStmt)
		{
			/*
			* SIMPLE test code *
			procedure testProcedure {
				x = 6;
				if (x<5) then {
					x = 5;
				}
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
				{ TokenType::Delimiter, "}" },
				{ TokenType::Delimiter, "}" },
			};

			// Initialize Parser
			parser = new ASTParser(tokens);

			// Generate AST
			parser->generateAST();

			// Get root node
			ASTRoot* generatedRoot = (ASTRoot*)parser->returnASTTree();

			// Get generated AST for testProcedure
			ProcedureNode* generatedAST = (ProcedureNode*)generatedRoot->getChildren().front();

			// Check if procedure name is correct
			Assert::IsTrue("testProcedure" == generatedAST->getNodeName());

			StmtListNode* generatedStmtList = (StmtListNode*)generatedAST->getChildren().front();

			// Check if stmtList has the right amount of children
			Assert::IsTrue(2 == generatedStmtList->getChildren().size());

			AssignmentNode* generatedAssignment = (AssignmentNode*)generatedStmtList->getChildren().front();

			// Check if assign statement number is correct
			Assert::IsTrue(1 == generatedAssignment->getStatementNumber());

			// Check if ifNode statement number is correct
			std::list<ASTNode*> stmtList = generatedStmtList->getChildren();
			stmtList.pop_front();
			IfNode* generatedIf = (IfNode*)stmtList.front();

			Assert::IsTrue(2 == generatedIf->getStatementNumber());

			// Check if stmtList has the right amount of children
			Assert::IsTrue(2 == generatedIf->getChildren().size());
		}

		TEST_METHOD(testIfElseStmt)
		{
			/*
			* SIMPLE test code *
			procedure testProcedure {
				x = 6;
				if (x<5) then {
					x = 5;
				} else {
					x = 7;
				}
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
				{ TokenType::Delimiter, "}" },
				{ TokenType::Identifier, "else" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "x"},
				{ TokenType::Operator, "="},
				{ TokenType::Number, "7"},
				{ TokenType::Delimiter, ";"},
				{ TokenType::Delimiter, "}" },
				{ TokenType::Delimiter, "}" },
			};

			// Initialize Parser
			parser = new ASTParser(tokens);

			// Generate AST
			parser->generateAST();

			// Get root node
			ASTRoot* generatedRoot = (ASTRoot*)parser->returnASTTree();

			// Get generated AST for testProcedure
			ProcedureNode* generatedAST = (ProcedureNode*)generatedRoot->getChildren().front();

			// Check if procedure name is correct
			Assert::IsTrue("testProcedure" == generatedAST->getNodeName());

			StmtListNode* generatedStmtList = (StmtListNode*)generatedAST->getChildren().front();

			// Check if stmtList has the right amount of children
			Assert::IsTrue(2 == generatedStmtList->getChildren().size());

			AssignmentNode* generatedAssignment = (AssignmentNode*)generatedStmtList->getChildren().front();

			// Check if assign statement number is correct
			Assert::IsTrue(1 == generatedAssignment->getStatementNumber());

			// Check if ifNode statement number is correct
			std::list<ASTNode*> stmtList = generatedStmtList->getChildren();
			stmtList.pop_front();
			IfNode* generatedIf = (IfNode*)stmtList.front();

			Assert::IsTrue(2 == generatedIf->getStatementNumber());

			// Check if stmtList has the right amount of children
			Assert::IsTrue(3 == generatedIf->getChildren().size());

			// Check if then stmt Number
			std::list<ASTNode*> ifChildren = generatedIf->getChildren();
			ifChildren.pop_front();
			StmtListNode* thenStmtList = (StmtListNode*)ifChildren.front();
			AssignmentNode* generatedAssignmentIfThen = (AssignmentNode*)thenStmtList->getChildren().front();

			Assert::IsTrue(3 == generatedAssignmentIfThen->getStatementNumber());

			ifChildren.pop_front();
			StmtListNode* elseStmtList = (StmtListNode*)ifChildren.front();
			AssignmentNode* generatedAssignmentIfElse = (AssignmentNode*)elseStmtList->getChildren().front();

			Assert::IsTrue(4 == generatedAssignmentIfElse->getStatementNumber());
		}

		TEST_METHOD(testWhileStmt)
		{
			/*
			* SIMPLE test code *
			procedure testProcedure {
				x = 6;
				while (x<5) {
					x = 5;
				}
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
				{ TokenType::Delimiter, "}" },
			};

			// Initialize Parser
			parser = new ASTParser(tokens);

			// Generate AST
			parser->generateAST();

			// Get root node
			ASTRoot* generatedRoot = (ASTRoot*)parser->returnASTTree();

			// Get generated AST for testProcedure
			ProcedureNode* generatedAST = (ProcedureNode*)generatedRoot->getChildren().front();

			// Check if procedure name is correct
			Assert::IsTrue("testProcedure" == generatedAST->getNodeName());

			StmtListNode* generatedStmtList = (StmtListNode*)generatedAST->getChildren().front();

			// Check if stmtList has the right amount of children
			Assert::IsTrue(2 == generatedStmtList->getChildren().size());

			AssignmentNode* generatedAssignment = (AssignmentNode*)generatedStmtList->getChildren().front();

			// Check if assign statement number is correct
			Assert::IsTrue(1 == generatedAssignment->getStatementNumber());

			// Check if whileNode statement number is correct
			std::list<ASTNode*> stmtList = generatedStmtList->getChildren();
			stmtList.pop_front();
			WhileNode* generatedWhile = (WhileNode*)stmtList.front();

			Assert::IsTrue(2 == generatedWhile->getStatementNumber());

			// Check if stmtList has the right amount of children
			Assert::IsTrue(2 == generatedWhile->getChildren().size());
		}

		TEST_METHOD(testShuntingYardOperator)
		{
			//x = (y-6%3)*z/2+a
			std::list<Token> tokens = {
				{ TokenType::Identifier, "procedure" },
				{ TokenType::Identifier, "testProcedure" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "x" },
				{ TokenType::Operator, "="},
				{ TokenType::Delimiter, "(" },
				{ TokenType::Identifier, "y" },
				{ TokenType::Operator, "-"},
				{ TokenType::Number, "6"},
				{ TokenType::Operator, "%"},
				{ TokenType::Number, "3"},
				{ TokenType::Delimiter, ")" },

				{ TokenType::Operator, "*"},
				{ TokenType::Identifier, "z" },
				{ TokenType::Operator, "/"},
				{ TokenType::Number, "2"},
				{ TokenType::Operator, "+"},
				{ TokenType::Identifier, "a" },
				{ TokenType::Delimiter, ";" },
				{ TokenType::Delimiter, "}" },

			};

			// Initialize Parser
			parser = new ASTParser(tokens);

			// Generate AST
			parser->generateAST();

			// Get root node
			ASTRoot* generatedRoot = (ASTRoot*)parser->returnASTTree();

			// Get AST of testProcedure
			ProcedureNode* generatedAST = (ProcedureNode*)generatedRoot->getChildren().front();

			// Check if generated children is correct
			Assert::IsTrue(generatedAST->getChildren().front()->getChildren().size() == 1);

			std::string generated = generatedAST->getNodeName();

			// Check if the right Procedure is generated
			Assert::IsTrue(generated == "testProcedure");

			StmtListNode* generatedStmtList = (StmtListNode*)generatedAST->getChildren().front();

			// Check if stmtList has the right amount of children
			Assert::IsTrue(1 == generatedStmtList->getChildren().size());

			AssignmentNode* generatedAssignment = (AssignmentNode*)generatedStmtList->getChildren().front();

			// Check if assign statement number is correct
			Assert::IsTrue(1 == generatedAssignment->getStatementNumber());

			// Check if assign has the right amount of children
			Assert::IsTrue(2 == generatedAssignment->getChildren().size());

			VariableNode* generatedVariableX = (VariableNode*)generatedAssignment->getChildren().front();
			
			//Check if LHS of Assignment is variable x 
			Assert::IsTrue("x" == generatedVariableX->getVariable());

			std::list<ASTNode*> tempList = generatedAssignment->getChildren();
			tempList.pop_front();

			OperatorNode* generatedOperatorPlus = (OperatorNode*)tempList.front();
			std::string postfix = generatedOperatorPlus->getPostfix();

			//Check if RHS of Assignment is operator +
			Assert::IsTrue("+" == generatedOperatorPlus->getOperator());
			
			//Continue
			
		}

		TEST_METHOD(testShuntingYardConditional)
		{
			/*
			* SIMPLE test code *
			procedure testProcedure { 
				while( (x<=y) && (( (1>=2)||(x<3) ) || (!(c>4))) ) {
					a=5;
				}
			}
			*/

			std::list<Token> tokens = {
				{ TokenType::Identifier, "procedure" },
				{ TokenType::Identifier, "testProcedure" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "while" },
				{ TokenType::Delimiter, "(" },
				{ TokenType::Delimiter, "(" },
				{ TokenType::Identifier, "x" },
				{ TokenType::ComparisonOperator, "<="},
				{ TokenType::Identifier, "y" },
				{ TokenType::Delimiter, ")" },
				{ TokenType::BooleanOperator, "&&" },
				{ TokenType::Delimiter, "(" },
				{ TokenType::Delimiter, "(" },
				{ TokenType::Delimiter, "(" },
				{ TokenType::Number, "1"},
				{ TokenType::ComparisonOperator, ">="},
				{ TokenType::Number, "2"},
				{ TokenType::Delimiter, ")" },
				{ TokenType::BooleanOperator, "||"},
				{ TokenType::Delimiter, "(" },
				{ TokenType::Identifier, "x"},
				{ TokenType::ComparisonOperator, "<"},
				{ TokenType::Number, "3"},
				{ TokenType::Delimiter, ")" },
				{ TokenType::Delimiter, ")" },
				{ TokenType::BooleanOperator, "||"},
				{ TokenType::Delimiter, "(" },
				{ TokenType::BooleanOperator, "!"},
				{ TokenType::Delimiter, "(" },
				{ TokenType::Identifier, "c"},
				{ TokenType::ComparisonOperator, ">"},
				{ TokenType::Number, "4"},
				{ TokenType::Delimiter, ")" },
				{ TokenType::Delimiter, ")" },
				{ TokenType::Delimiter, ")" },
				{ TokenType::Delimiter, ")" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "a"},
				{ TokenType::Operator, "="},
				{ TokenType::Number, "5"},
				{ TokenType::Delimiter, ";"},
				{ TokenType::Delimiter, "}" },
				{ TokenType::Delimiter, "}" },
			};
			// Initialize Parser
			parser = new ASTParser(tokens);

			// Generate AST
			parser->generateAST();

			// Get root node
			ASTRoot* generatedRoot = (ASTRoot*)parser->returnASTTree();

			// Get AST of testProcedure
			ProcedureNode* generatedAST = (ProcedureNode*)generatedRoot->getChildren().front();

			// Check if generated children is correct
			Assert::IsTrue(generatedAST->getChildren().front()->getChildren().size() == 1);

			std::string generated = generatedAST->getNodeName();

			// Check if the right Procedure is generated
			Assert::IsTrue(generated == "testProcedure");

			StmtListNode* generatedStmtList = (StmtListNode*)generatedAST->getChildren().front();

			WhileNode* generatedWhileNode = (WhileNode*)generatedStmtList->getChildren().front();

			OperatorNode* conditionalRoot = (OperatorNode*)generatedWhileNode->getChildren().front();

			Assert::IsTrue(conditionalRoot->getOperator() == "&&");
			Assert::IsTrue(conditionalRoot->getChildren().size() == 2);
			
			// x<=y
			OperatorNode* rootLHS = (OperatorNode*)conditionalRoot->getChildren().front();
			
			Assert::IsTrue(rootLHS->getOperator() == "<=");
			Assert::IsTrue(rootLHS->getChildren().size() == 2);

			VariableNode* x = (VariableNode*)rootLHS->getChildren().front();

			Assert::IsTrue(x->getVariable() == "x");
			Assert::IsTrue(x->getChildren().size() == 0);


			rootLHS->removeFirstChild();
			VariableNode* y = (VariableNode*)rootLHS->getChildren().front();

			Assert::IsTrue(y->getVariable() == "y");
			Assert::IsTrue(y->getChildren().size() == 0);

			
			conditionalRoot->removeFirstChild();
			// ((1>=2)||(x<3)) || (!(c>4))
			OperatorNode* rootRHS = (OperatorNode*)conditionalRoot->getChildren().front();

			Assert::IsTrue(rootRHS->getOperator() == "||");
			Assert::IsTrue(rootRHS->getChildren().size() == 2);

			// (1>=2) || (x<3)
			OperatorNode* RHSLHS = (OperatorNode*)rootRHS->getChildren().front();
				
			Assert::IsTrue(RHSLHS->getOperator() == "||");
			Assert::IsTrue(RHSLHS->getChildren().size() == 2);
			// 1>=2
			OperatorNode* oneGreaterEqualThanTwo = (OperatorNode*)RHSLHS->getChildren().front();

			Assert::IsTrue(oneGreaterEqualThanTwo->getOperator() == ">=");
			Assert::IsTrue(oneGreaterEqualThanTwo->getChildren().size() == 2);

			ConstantNode* one = (ConstantNode*)oneGreaterEqualThanTwo->getChildren().front();

			Assert::IsTrue(one->getConstant() == 1);
			Assert::IsTrue(one->getChildren().size() == 0);

			oneGreaterEqualThanTwo->removeFirstChild();
			ConstantNode* two = (ConstantNode*)oneGreaterEqualThanTwo->getChildren().front();

			Assert::IsTrue(two->getConstant() == 2);
			Assert::IsTrue(two->getChildren().size() == 0);

			RHSLHS->removeFirstChild();
			// x<3
			OperatorNode* xSmallerThanThree = (OperatorNode*)RHSLHS->getChildren().front();

			Assert::IsTrue(xSmallerThanThree->getOperator() == "<");
			Assert::IsTrue(xSmallerThanThree->getChildren().size() == 2);

			x = (VariableNode*)xSmallerThanThree->getChildren().front();

			Assert::IsTrue(x->getVariable() == "x");
			Assert::IsTrue(x->getChildren().size() == 0);


			xSmallerThanThree->removeFirstChild();
			ConstantNode* three = (ConstantNode*)xSmallerThanThree->getChildren().front();

			Assert::IsTrue(three->getConstant() == 3);
			Assert::IsTrue(three->getChildren().size() == 0);

			rootRHS->removeFirstChild();
			// !(c>4)
			OperatorNode* RHSRHS = (OperatorNode*)rootRHS->getChildren().front();

			Assert::IsTrue(RHSRHS->getOperator() == "!");
			Assert::IsTrue(RHSRHS->getChildren().size() == 1);
			
			// c>4
			OperatorNode* cGreaterThan4 = (OperatorNode*)RHSRHS->getChildren().front();

			Assert::IsTrue(cGreaterThan4->getOperator() == ">");
			Assert::IsTrue(cGreaterThan4->getChildren().size() == 2);

			VariableNode* c = (VariableNode*)cGreaterThan4->getChildren().front();

			Assert::IsTrue(c->getVariable() == "c");
			Assert::IsTrue(c->getChildren().size() == 0);


			cGreaterThan4->removeFirstChild();
			ConstantNode* four = (ConstantNode*)cGreaterThan4->getChildren().front();

			Assert::IsTrue(four->getConstant() == 4);
			Assert::IsTrue(four->getChildren().size() == 0);

		}

		TEST_METHOD(testTwoProcedures)
		{
			std::list<Token> tokens = {
				{ TokenType::Identifier, "procedure" },
				{ TokenType::Identifier, "testProcedure1" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "x"},
				{ TokenType::Operator, "="},
				{ TokenType::Number, "5"},
				{ TokenType::Delimiter, ";"},
				{ TokenType::Delimiter, "}" },
				{ TokenType::Identifier, "procedure" },
				{ TokenType::Identifier, "testProcedure2" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "x"},
				{ TokenType::Operator, "="},
				{ TokenType::Number, "5"},
				{ TokenType::Delimiter, ";"},
				{ TokenType::Delimiter, "}" },
			};

			// Initialize Parser
			parser = new ASTParser(tokens);

			// Generate AST
			parser->generateAST();

			// Get root node
			ASTRoot* generatedRoot = (ASTRoot*)parser->returnASTTree();
			std::list<ASTNode*> procedureList = generatedRoot->getChildren();

			// Get AST of first test procedure
			ProcedureNode* generatedASTOne = (ProcedureNode*)procedureList.front();

			// Check if generated children is correct
			Assert::IsTrue(generatedASTOne->getChildren().front()->getChildren().size() == 1);

			std::string generatedOne = generatedASTOne->getNodeName();

			// Check if the right Procedure is generated
			Assert::IsTrue(generatedOne == "testProcedure1");

			// Check second node
			procedureList.pop_front();
			ProcedureNode* generatedASTTwo = (ProcedureNode*)procedureList.front();

			// Check if generated children is correct
			Assert::IsTrue(generatedASTTwo->getChildren().front()->getChildren().size() == 1);

			std::string generatedTwo = generatedASTTwo->getNodeName();

			// Check if the right Procedure is generated
			Assert::IsTrue(generatedTwo == "testProcedure2");
		}

		// Invalid Cases
		TEST_METHOD(missingOpenCurlyBrace)
		{
			std::list<Token> tokens = {
				{ TokenType::Identifier, "procedure" },
				{ TokenType::Identifier, "testProcedure" },
				{ TokenType::Delimiter, "}" },
			};

			// Initialize Parser
			parser = new ASTParser(tokens);

			auto func = [=] {
				parser->generateAST();
			};
			Assert::ExpectException<std::logic_error>(func);;
		}

		TEST_METHOD(missingCloseCurlyBrace)
		{
			std::list<Token> tokens = {
				{ TokenType::Identifier, "procedure" },
				{ TokenType::Identifier, "testProcedure" },
				{ TokenType::Delimiter, "{" },
			};

			// Initialize Parser
			parser = new ASTParser(tokens);

			auto func = [=] {
				parser->generateAST();
			};
			Assert::ExpectException<std::logic_error>(func);;
		}

		TEST_METHOD(missingSemicolon)
		{
			std::list<Token> tokens = {
				{ TokenType::Identifier, "procedure" },
				{ TokenType::Identifier, "testProcedure" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "x"},
				{ TokenType::Operator, "="},
				{ TokenType::Number, "5"},
				{ TokenType::Delimiter, "}" },
			};

			// Initialize Parser
			parser = new ASTParser(tokens);

			auto func = [=] {
				parser->generateAST();
			};
			Assert::ExpectException<std::logic_error>(func);;
		}

		TEST_METHOD(missingOpenBrace)
		{
			std::list<Token> tokens = {
				{ TokenType::Identifier, "procedure" },
				{ TokenType::Identifier, "testProcedure" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "while" },
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
			parser = new ASTParser(tokens);

			auto func = [=] {
				parser->generateAST();
			};
			Assert::ExpectException<std::logic_error>(func);;
		}

		TEST_METHOD(missingCloseBrace)
		{
			std::list<Token> tokens = {
				{ TokenType::Identifier, "procedure" },
				{ TokenType::Identifier, "testProcedure" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "while" },
				{ TokenType::Delimiter, "(" },
				{ TokenType::Identifier, "x"},
				{ TokenType::ComparisonOperator, "<"},
				{ TokenType::Number, "5"},
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "x"},
				{ TokenType::Operator, "="},
				{ TokenType::Number, "5"},
				{ TokenType::Delimiter, ";"},
				{ TokenType::Delimiter, "}" },
				{ TokenType::Delimiter, "}" },
			};

			// Initialize Parser
			parser = new ASTParser(tokens);

			auto func = [=] {
				parser->generateAST();
			};
			Assert::ExpectException<std::logic_error>(func);;
		}

		TEST_METHOD(missingConditional)
		{
			std::list<Token> tokens = {
				{ TokenType::Identifier, "procedure" },
				{ TokenType::Identifier, "testProcedure" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "while" },
				{ TokenType::Delimiter, "(" },
				{ TokenType::Identifier, "x"},
				{ TokenType::Delimiter, ")"},
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "x"},
				{ TokenType::Operator, "="},
				{ TokenType::Number, "5"},
				{ TokenType::Delimiter, ";"},
				{ TokenType::Delimiter, "}" },
				{ TokenType::Delimiter, "}" },
			};

			// Initialize Parser
			parser = new ASTParser(tokens);

			auto func = [=] {
				parser->generateAST();
			};
			Assert::ExpectException<std::logic_error>(func);;
		}

		TEST_METHOD(missingEquals)
		{
			std::list<Token> tokens = {
				{ TokenType::Identifier, "procedure" },
				{ TokenType::Identifier, "testProcedure" },
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "while" },
				{ TokenType::Delimiter, "(" },
				{ TokenType::Identifier, "x"},
				{ TokenType::ComparisonOperator, "<"},
				{ TokenType::Number, "5"},
				{ TokenType::Delimiter, ")"},
				{ TokenType::Delimiter, "{" },
				{ TokenType::Identifier, "x"},
				{ TokenType::Delimiter, ";"},
				{ TokenType::Delimiter, "}" },
				{ TokenType::Delimiter, "}" },
			};

			// Initialize Parser
			parser = new ASTParser(tokens);

			auto func = [=] {
				parser->generateAST();
			};
			Assert::ExpectException<std::logic_error>(func);;
		}
	};
}
