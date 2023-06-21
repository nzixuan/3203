#include <memory>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "stdafx.h"
#include "CppUnitTest.h"
#include "AST.h"
#include "DesignExtractor.h"
#include "PKB.h"
#include "Tokenizer.h"
#include "SimpleParser.h"
#include "CFG.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(DesignExtractorTest)
	{
	public:
		std::list<CFGRoot*> DummyCFGList;

		TEST_METHOD(testExtractUsesStatement) {
			PKB* pkb = new PKB();

			// Create testAST
			/*
			procedure test {
			1	x = 5
			2	y = (10 + x) * y;
			3	while((x == z)&&(a == b)) {
			4		x = y;
				}
			5	if(x == z){
			6		print x;
				} else {
			7		print y;
				}
			}
			*/

			ASTRoot* testASTRoot = new ASTRoot();
			ProcedureNode* testAST = new ProcedureNode("test");
			StmtListNode* procedureClause = new StmtListNode(testAST);
			AssignmentNode* assignment = new AssignmentNode(1, testAST);
			VariableNode* x = new VariableNode("x", testAST);
			ConstantNode* five = new ConstantNode(5, testAST);
	
			AssignmentNode* assignment1 = new AssignmentNode(2, testAST);
			VariableNode* y = new VariableNode("y", testAST);
			OperatorNode* plus = new OperatorNode("+", testAST);
			VariableNode* x1 = new VariableNode("x", testAST);
			ConstantNode* ten = new ConstantNode(10, testAST);
			OperatorNode* times = new OperatorNode("*", testAST);
			VariableNode* y2 = new VariableNode("y", testAST);

			WhileNode* whilenode = new WhileNode(3, testAST);
			StmtListNode* whileClause = new StmtListNode(testAST);
			OperatorNode* and = new OperatorNode("&&", testAST);
			OperatorNode* equals = new OperatorNode("==", testAST);
			VariableNode* x2 = new VariableNode("x", testAST);
			VariableNode* z = new VariableNode("z", testAST);
			OperatorNode* equals2 = new OperatorNode("==", testAST);
			VariableNode* a = new VariableNode("a", testAST);
			VariableNode* b = new VariableNode("b", testAST);

			AssignmentNode* assignment2 = new AssignmentNode(4, testAST);
			VariableNode* x3 = new VariableNode("x", testAST);
			VariableNode* y3 = new VariableNode("y", testAST);

			IfNode* ifnode = new IfNode(5, testAST);
			StmtListNode* ifClause = new StmtListNode(testAST);
			OperatorNode* equals3 = new OperatorNode("==", testAST);
			VariableNode* x4 = new VariableNode("x", testAST);
			VariableNode* z2 = new VariableNode("z", testAST);
			PrintNode* print = new PrintNode(6, "x", testAST);
			StmtListNode* elseClause = new StmtListNode(testAST);
			PrintNode* print2 = new PrintNode(7, "y", testAST);

			// x = 5
			assignment->addChildren(x);
			assignment->addChildren(five);

			// y = (10 + x) * y
			assignment1->addChildren(y);
			plus->addChildren(x1);
			plus->addChildren(ten);
			times->addChildren(plus);
			times->addChildren(y2);
			assignment1->addChildren(times);
			
			
			//x = y;
			assignment2->addChildren(x3);
			assignment2->addChildren(y3);
	
			//while((x==z)&&(a==b))
			
			equals->addChildren(x2);
			equals->addChildren(z);

			equals2->addChildren(a);
			equals2->addChildren(b);
			and ->addChildren(equals);
			and ->addChildren(equals2);

			whileClause->addChildren(assignment2);
			whilenode->addChildren(and);
			whilenode->addChildren(whileClause);

			//if(x==z) 
			ifClause->addChildren(print);
			equals3->addChildren(x4);
			equals3->addChildren(z2);
			
			elseClause->addChildren(print2);

			ifnode->addChildren(equals3);
			ifnode->addChildren(ifClause);
			ifnode->addChildren(elseClause);
	
			// procedureTest
			procedureClause->addChildren(assignment);
			procedureClause->addChildren(assignment1);
			procedureClause->addChildren(whilenode);
			procedureClause->addChildren(ifnode);
			testAST->addChildren(procedureClause);
			testASTRoot->addChildren(testAST);

			// Extract Relationship
			DesignExtractor::extract(testASTRoot, DummyCFGList, pkb);

			// Check if the right number of relationships are stored in the PKB
			Assert::IsTrue(pkb->getAllStmts().size() == 7);
			Assert::IsTrue(pkb->getAssignStmts().size() == 3);

			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::Assign,
				EntityType::Variable,
				RelRefType::Uses).size() == 3);

			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::While,
				EntityType::Variable,
				RelRefType::Uses).size() == 5);

			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::If,
				EntityType::Variable,
				RelRefType::Uses).size() == 3);

			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::Print,
				EntityType::Variable,
				RelRefType::Uses).size() == 2);
		}

		TEST_METHOD(testExtractParentStatement) {
			PKB* pkb = new PKB();
			// Create testAST
			/*
			procedure test {
			1	x = 5;
			2	if (x > 5) then {
			3		if (x > 10) then {
			4			x = 10;
					}
				}
			}
			*/

			ASTRoot* testASTRoot = new ASTRoot();
			ProcedureNode* testAST = new ProcedureNode("test");
			StmtListNode* procedureClause = new StmtListNode(testAST);

			AssignmentNode* assignment = new AssignmentNode(1, testAST);
			VariableNode* x = new VariableNode("x", testAST);
			ConstantNode* five = new ConstantNode(5, testAST);

			IfNode* if1 = new IfNode(2, testAST);
			OperatorNode* conditional = new OperatorNode(">", testAST);
			VariableNode* x2 = new VariableNode("x", testAST);
			ConstantNode* five1 = new ConstantNode(5, testAST);

			StmtListNode* if1StmtList = new StmtListNode(testAST);

			IfNode* if2 = new IfNode(3, testAST);
			OperatorNode* conditional1 = new OperatorNode(">", testAST);
			VariableNode* x3 = new VariableNode("x", testAST);
			ConstantNode* ten = new ConstantNode(10, testAST);

			StmtListNode* if2StmtList = new StmtListNode(testAST);

			AssignmentNode* assignment1 = new AssignmentNode(4, testAST);
			VariableNode* x4 = new VariableNode("x", testAST);
			ConstantNode* ten1 = new ConstantNode(10, testAST);

			// x = 5
			assignment->addChildren(x);
			assignment->addChildren(five);

			// if (x > 5) then {
			conditional->addChildren(x2);
			conditional->addChildren(five1);
			if1->addChildren(conditional);

			// if (x > 10) then {
			conditional1->addChildren(x3);
			conditional1->addChildren(ten);
			if2->addChildren(conditional1);

			// x = 10
			assignment1->addChildren(x4);
			assignment1->addChildren(ten1);
			if2StmtList->addChildren(assignment1);
			if2->addChildren(if2StmtList);
			if1StmtList->addChildren(if2);
			if1->addChildren(if1StmtList);

			// procedureTest
			procedureClause->addChildren(assignment);
			procedureClause->addChildren(if1);
			testAST->addChildren(procedureClause);
			testASTRoot->addChildren(testAST);

			// Extract Relationship
			DesignExtractor::extract(testASTRoot, DummyCFGList, pkb);

			// Check if the right number of relationships are stored in the PKB
			Assert::IsTrue(pkb->getAllStmts().size() == 4);
			Assert::IsTrue(pkb->getAssignStmts().size() == 2);

			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(EntityType::If, EntityType::If, RelRefType::Parent).size() == 1);
			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(EntityType::If, EntityType::Assign, RelRefType::Parent).size() == 1);
		}

		TEST_METHOD(testExtractModifiesStatement) {
			PKB* pkb = new PKB();
			
			// Create testAST
			/*
			procedure test {
			1	x = 5;
			2	read z;
			3	while(z==3) {
			4	x = y;
				}
			5	if(z==3){
			6	y = 2;
				}
			}
			*/

			ASTRoot* testASTRoot = new ASTRoot();
			ProcedureNode* testAST = new ProcedureNode("test");
			StmtListNode* procedureClause = new StmtListNode(testAST);
			AssignmentNode* assignment = new AssignmentNode(1, testAST);
			VariableNode* x = new VariableNode("x", testAST);
			ConstantNode* five = new ConstantNode(5, testAST);
			ReadNode* read = new ReadNode(2, "z", testAST);
			WhileNode* whilenode = new WhileNode(3, testAST);
			OperatorNode* equals = new OperatorNode("==", testAST);
			VariableNode* z = new VariableNode("z", testAST);
			ConstantNode* three = new ConstantNode(3, testAST);
			StmtListNode* whileClause = new StmtListNode(testAST);

			AssignmentNode* assignment2 = new AssignmentNode(4, testAST);
			VariableNode* x2 = new VariableNode("x", testAST);
			VariableNode* y = new VariableNode("y", testAST);

			IfNode* ifnode = new IfNode(5, testAST);
			StmtListNode* ifClause = new StmtListNode(testAST);
			OperatorNode* equals2 = new OperatorNode("==", testAST);
			VariableNode* z2 = new VariableNode("z", testAST);
			ConstantNode* three2 = new ConstantNode(3, testAST);
			AssignmentNode* assignment3 = new AssignmentNode(6, testAST);
			VariableNode* y2 = new VariableNode("y", testAST);
			ConstantNode* two = new ConstantNode(2, testAST);


			// x = 5
			assignment->addChildren(x);
			assignment->addChildren(five);

			//x=y
			assignment2->addChildren(x2);
			assignment2->addChildren(y);
			

			// while(z==3)
			equals->addChildren(z);
			equals->addChildren(three);
			whileClause->addChildren(assignment2);
			whilenode->addChildren(equals);
			whilenode->addChildren(whileClause);

			// y = 2
			assignment3->addChildren(y2);
			assignment3->addChildren(two);

			// if(z==3)
			equals2->addChildren(z2);
			equals2->addChildren(three2);
			ifClause->addChildren(assignment3);
			ifnode->addChildren(equals2);
			ifnode->addChildren(ifClause);

			
			// procedureTest
			procedureClause->addChildren(assignment);
			procedureClause->addChildren(read);
			procedureClause->addChildren(whilenode);
			procedureClause->addChildren(ifnode);
			testAST->addChildren(procedureClause);
			testASTRoot->addChildren(testAST);

			// Extract Relationship
			DesignExtractor::extract(testASTRoot, DummyCFGList, pkb);

			// Check if the right number of relationships are stored in the PKB
			Assert::IsTrue(pkb->getAllStmts().size() == 6);
			Assert::IsTrue(pkb->getAssignStmts().size() == 3);
			Assert::IsTrue(pkb->getWhileStmts().size() == 1);
			Assert::IsTrue(pkb->getIfStmts().size() == 1);
			Assert::IsTrue(pkb->getReadStmts().size() == 1);

			
			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::Assign,
				EntityType::Variable,
				RelRefType::Modifies).size() == 3);

			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::Read,
				EntityType::Variable,
				RelRefType::Modifies).size() == 1);

			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::While,
				EntityType::Variable,
				RelRefType::Modifies).size() == 1);

			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::If,
				EntityType::Variable,
				RelRefType::Modifies).size() == 1);

		}

		TEST_METHOD(testExtractFollowsStatement) {
			PKB* pkb = new PKB();

			// Create testAST
			/*
			procedure test {
			1	x = 5;
			2	read z;
			3	while(z==3) {
			4		x = y;
				}
			5	if(z==3){
			6		y = 2;
			7		print x;
			8		print y;
				}
			}
			*/

			ASTRoot* testASTRoot = new ASTRoot();
			ProcedureNode* testAST = new ProcedureNode("test");
			StmtListNode* procedureClause = new StmtListNode(testAST);
			AssignmentNode* assignment = new AssignmentNode(1, testAST);
			VariableNode* x = new VariableNode("x", testAST);
			ConstantNode* five = new ConstantNode(5, testAST);
			ReadNode* read = new ReadNode(2, "z", testAST);
			WhileNode* whilenode = new WhileNode(3, testAST);
			OperatorNode* equals = new OperatorNode("==", testAST);
			VariableNode* z = new VariableNode("z", testAST);
			ConstantNode* three = new ConstantNode(3, testAST);
			StmtListNode* whileClause = new StmtListNode(testAST);

			AssignmentNode* assignment2 = new AssignmentNode(4, testAST);
			VariableNode* x2 = new VariableNode("x", testAST);
			VariableNode* y = new VariableNode("y", testAST);

			IfNode* ifnode = new IfNode(5, testAST);
			StmtListNode* ifClause = new StmtListNode(testAST);
			OperatorNode* equals2 = new OperatorNode("==", testAST);
			VariableNode* z2 = new VariableNode("z", testAST);
			ConstantNode* three2 = new ConstantNode(3, testAST);
			AssignmentNode* assignment3 = new AssignmentNode(6, testAST);
			VariableNode* y2 = new VariableNode("y", testAST);
			ConstantNode* two = new ConstantNode(2, testAST);
			PrintNode* print1 = new PrintNode(7, "x", testAST);
			PrintNode* print2 = new PrintNode(8, "y", testAST);

			// x = 5
			assignment->addChildren(x);
			assignment->addChildren(five);

			//x=y
			assignment2->addChildren(x2);
			assignment2->addChildren(y);


			// while(z==3)
			equals->addChildren(z);
			equals->addChildren(three);
			whileClause->addChildren(assignment2);
			whilenode->addChildren(equals);
			whilenode->addChildren(whileClause);

			// y = 2
			assignment3->addChildren(y2);
			assignment3->addChildren(two);

			// if(z==3)
			equals2->addChildren(z2);
			equals2->addChildren(three2);
			ifClause->addChildren(assignment3);
			ifClause->addChildren(print1);
			ifClause->addChildren(print2);
			ifnode->addChildren(equals2);
			ifnode->addChildren(ifClause);


			// procedureTest
			procedureClause->addChildren(assignment);
			procedureClause->addChildren(read);
			procedureClause->addChildren(whilenode);
			procedureClause->addChildren(ifnode);
			testAST->addChildren(procedureClause);
			testASTRoot->addChildren(testAST);

			// Extract Relationship
			DesignExtractor::extract(testASTRoot, DummyCFGList, pkb);

			// Check if the right number of relationships are stored in the PKB
			Assert::IsTrue(pkb->getAllStmts().size() == 8);
			Assert::IsTrue(pkb->getAssignStmts().size() == 3);
			Assert::IsTrue(pkb->getWhileStmts().size() == 1);
			Assert::IsTrue(pkb->getIfStmts().size() == 1);
			Assert::IsTrue(pkb->getReadStmts().size() == 1);
			Assert::IsTrue(pkb->getPrintStmts().size() == 2);


			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::Assign,
				EntityType::Read,
				RelRefType::Follows).size() == 1);

			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::Read,
				EntityType::While,
				RelRefType::Follows).size() == 1);

			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::Read,
				EntityType::Assign,
				RelRefType::Follows).size() == 0);

			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::While,
				EntityType::If,
				RelRefType::Follows).size() == 1);

			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::While,
				EntityType::Assign,
				RelRefType::Follows).size() == 0);

			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::Assign,
				EntityType::Print,
				RelRefType::Follows).size() == 1);

			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::If,
				EntityType::Assign,
				RelRefType::Follows).size() == 0);

			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::Print,
				EntityType::Print,
				RelRefType::Follows).size() == 1);
		}

		TEST_METHOD(testExtractCallsStatement) {
			PKB* pkb = new PKB();
			Tokenizer tokenizer;

			std::string exampleDelimiters = "procedure One {  a = 1; } procedure Two { call One; } procedure Three { call Two; }";

			std::stringstream ss;
			ss << exampleDelimiters;


			const auto tokens = tokenizer.tokenize(ss);
			// Initialize Parser
			ASTParser* parser = new ASTParser(tokens);

			// Generate AST
			parser->generateAST();

			// Get root node
			ASTRoot* generatedRoot = (ASTRoot*)parser->returnASTTree();

			// Extract Relationships
			DesignExtractor::extract(generatedRoot, DummyCFGList, pkb);

			Assert::IsTrue(pkb->getAllStmts().size() == 3);

			// There should be one calls
			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::Procedure,
				EntityType::Procedure,
				RelRefType::Calls).size() == 2);
		
		}

		TEST_METHOD(testExtractUsesProcedureAndCall) {
			PKB* pkb = new PKB();
			Tokenizer tokenizer;

			std::string exampleDelimiters = "procedure One {  a = b; a = 1; } procedure Two { call One; }";

			std::stringstream ss;
			ss << exampleDelimiters;


			const auto tokens = tokenizer.tokenize(ss);
			// Initialize Parser
			ASTParser* parser = new ASTParser(tokens);

			// Generate AST
			parser->generateAST();

			// Get root node
			ASTRoot* generatedRoot = (ASTRoot*)parser->returnASTTree();

			// Extract Relationships
			DesignExtractor::extract(generatedRoot, DummyCFGList, pkb);

			Assert::IsTrue(pkb->getAllStmts().size() == 3);

			// There should be a uses procedure
			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::Procedure,
				EntityType::Variable,
				RelRefType::Uses).size() == 2);

			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::Call,
				EntityType::Variable,
				RelRefType::Uses).size() == 1);

		}

		TEST_METHOD(testExtractModifiesProcedureAndCall) {
			PKB* pkb = new PKB();
			Tokenizer tokenizer;

			std::string exampleDelimiters = "procedure One { a = 1; } procedure Two { call One; }";

			std::stringstream ss;
			ss << exampleDelimiters;


			const auto tokens = tokenizer.tokenize(ss);
			// Initialize Parser
			ASTParser* parser = new ASTParser(tokens);

			// Generate AST
			parser->generateAST();

			// Get root node
			ASTRoot* generatedRoot = (ASTRoot*)parser->returnASTTree();

			// Extract Relationships
			DesignExtractor::extract(generatedRoot, DummyCFGList, pkb);

			Assert::IsTrue(pkb->getAllStmts().size() == 2);

			// There should be a uses procedure
			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::Procedure,
				EntityType::Variable,
				RelRefType::Modifies).size() == 2);

			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::Call,
				EntityType::Variable,
				RelRefType::Modifies).size() == 1);

		}

		TEST_METHOD(testExtractPatternStatement) {
			PKB* pkb = new PKB();
			Tokenizer tokenizer;

			std::string exampleDelimiters = "procedure test { x = v + x * y + z * t;}";

			std::stringstream ss;
			ss << exampleDelimiters;


			const auto tokens = tokenizer.tokenize(ss);
			// Initialize Parser
			ASTParser* parser = new ASTParser(tokens);

			// Generate AST
			parser->generateAST();

			// Get root node
			ASTRoot* generatedRoot = (ASTRoot*)parser->returnASTTree();

			// Extract Relationships
			DesignExtractor::extract(generatedRoot, DummyCFGList, pkb);

			Assert::IsTrue(pkb->getAllStmts().size() == 1);

			// Pattern tests
			Assert::IsTrue(pkb->getPatternAssignLhsConstRhsExp("x", "v x y * + z t * +", true).size() == 1);
			Assert::IsFalse(pkb->getPatternAssignLhsConstRhsExp("x", "v", true).size() == 1);
			Assert::IsTrue(pkb->getPatternAssignLhsWildcardRhsExp("v", false).size() == 1);
			Assert::IsTrue(pkb->getPatternAssignLhsWildcardRhsExp("x y *", false).size() == 1);
			Assert::IsFalse(pkb->getPatternAssignLhsWildcardRhsExp("v x +", false).size() == 1);
			Assert::IsTrue(pkb->getPatternAssignLhsWildcardRhsExp("v x y * +", false).size() == 1);
			Assert::IsFalse(pkb->getPatternAssignLhsWildcardRhsExp("y z t * +", false).size() == 1);
			Assert::IsFalse(pkb->getPatternAssignLhsWildcardRhsExp("x y * z t * +", false).size() == 1);
			Assert::IsTrue(pkb->getPatternAssignLhsWildcardRhsExp("v x y * + z t * +", false).size() == 1);

		}

		TEST_METHOD(testExtractNextStatement) {
			PKB* pkb = new PKB();
			Tokenizer tokenizer;
			// procedure One { 
			// if (x==1) then {
			//     a = 2; 
			//     while(a==3) { 
			//         a = 4;
			//     } 
			//     a = 5; 
			// } else {
			//     a = 6; 
			//    } 
			// a = 7; 
			// } 

			std::string exampleDelimiters = "procedure One { if (x==1) then {a = 2; while(a==3) { a = 4;} a = 5; } else {a = 6; } a = 7; }";

			std::stringstream ss;
			ss << exampleDelimiters;

			const auto tokens = tokenizer.tokenize(ss);
			// Initialize Parser
			ASTParser* AST = new ASTParser(tokens);
			CFGParser* CFG = new CFGParser(tokens);

			// Generate AST and CFG
			AST->generateAST();
			CFG->generateCFG();

			// Extract Relationships
			DesignExtractor::extract(AST->returnASTTree(), CFG->returnCFGList(), pkb);

			Assert::IsTrue(pkb->getAllStmts().size() == 7);

			// Next tests
			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::If,
				EntityType::Assign,
				RelRefType::Next).size() == 2);

			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::While,
				EntityType::Assign,
				RelRefType::Next).size() == 2);

			Assert::IsTrue(pkb->getLhsSynRhsSynForRel(
				EntityType::Assign,
				EntityType::Assign,
				RelRefType::Next).size() == 2);
		}

		// Invalid cases
		// In progress
		TEST_METHOD(cyclicCalls) {
			PKB* pkb = new PKB();
			Tokenizer tokenizer;

			std::string exampleDelimiters = "procedure One { call Three; } procedure Two { call One; } procedure Three { call Two; }";

			std::stringstream ss;
			ss << exampleDelimiters;

			const auto tokens = tokenizer.tokenize(ss);
			// Initialize Parser
			ASTParser* parser = new ASTParser(tokens);

			// Generate AST
			parser->generateAST();

			// Get root node
			ASTRoot* generatedRoot = (ASTRoot*)parser->returnASTTree();

			// Extract Relationships
			auto func = [=] {
				DesignExtractor::extract(generatedRoot, DummyCFGList, pkb);
			};

			Assert::ExpectException<std::logic_error>(func);;

		}

	};
}