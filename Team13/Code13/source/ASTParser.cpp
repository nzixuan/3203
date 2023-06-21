#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <stack>
#include <queue>

#include "PKB.h"
#include "AST.h"
#include "CFG.h"
#include "DesignExtractorVisitor.h"
#include "SimpleParser.h"
#include "Token.h"

// ASTParser Functions
// Parses one procedure at a time
ASTNode* ASTParser::parseTokenList() {
	// Initialize parsing
	stmtNumber = 0;

	// Creat root
	ASTNode* root = new ASTRoot();

	while (tokens.empty() != true) {
		root->addChildren(parseProcedure());
	}

	return root;
};

ASTNode* ASTParser::parseProcedure() {
	// Ensure procedure <procedureName> { is present 
	require({ TokenType::Identifier, "procedure" }, true);
	std::string procedureName = require({ TokenType::Identifier, "procedureName" }, false).val;
	ProcedureNode* newProcedure = new ProcedureNode(procedureName);
	currentProcedure = newProcedure;

	require({ TokenType::Delimiter, "{" }, true);

	// Parse inner statements
	ASTNode* childNode = parseStmtList();

	// End of procedure parsing and return AST Tree
	require({ TokenType::Delimiter, "}" }, true);
	newProcedure->addChildren(childNode);

	return newProcedure;
};

ASTNode* ASTParser::parseIf() {
	// Initialize if node
	require({ TokenType::Identifier, "if" }, true);
	stmtNumber = stmtNumber + 1;
	ASTNode* newIfNode = new IfNode(stmtNumber, currentProcedure);
	
	// Parse condition
	require({ TokenType::Delimiter, "(" }, true);
	std::list<Token> tokenList;
	popTokensToRightParenthesis(&tokens,&tokenList);
	ASTNode* operatorNode = parseConditionalExpression(tokenList);
	require({ TokenType::Delimiter, ")" }, true);
	newIfNode->addChildren(operatorNode);

	// Parse if statementList
	require({ TokenType::Identifier, "then" }, true);
	require({ TokenType::Delimiter, "{" }, true);

	ASTNode* ifStmtLst = parseStmtList();

	require({ TokenType::Delimiter, "}" }, true);

	newIfNode->addChildren(ifStmtLst);
	
	// Parse else statementList
	if (tokens.front().val == "else") {
		require({ TokenType::Identifier, "else" }, true);
		require({ TokenType::Delimiter, "{" }, true);

		ASTNode* elseStmtLst = parseStmtList();

		require({ TokenType::Delimiter, "}" }, true);

		newIfNode->addChildren(elseStmtLst);
	}

	return newIfNode;
}

ASTNode* ASTParser::parseWhile() {
	// Initialize while node
	require({ TokenType::Identifier, "while" }, true);
	stmtNumber = stmtNumber + 1;
	ASTNode* newWhileNode = new WhileNode(stmtNumber, currentProcedure);

	// Parse condition
	require({ TokenType::Delimiter, "(" }, true);
	std::list<Token> tokenList;
	popTokensToRightParenthesis(&tokens,&tokenList);
	newWhileNode->addChildren(parseConditionalExpression(tokenList));
	require({ TokenType::Delimiter, ")" }, true);

	// Parse while statementList
	require({ TokenType::Delimiter, "{" }, true);

	ASTNode* whileStmtLst = parseStmtList();

	require({ TokenType::Delimiter, "}" }, true);

	newWhileNode->addChildren(whileStmtLst);

	return newWhileNode;
}

ASTNode* ASTParser::parseStmtList() {
	ASTNode* newStmtList = new StmtListNode(currentProcedure);

	while (!tokens.empty() && tokens.front().val != "}") {
		Token first = tokens.front();
		tokens.pop_front();
		Token second = tokens.front();
		tokens.push_front(first);

		if (first.val == "if" && second.val != "=") {
			newStmtList->addChildren(parseIf());
		} else if (first.val == "while" && second.val != "=") {
			newStmtList->addChildren(parseWhile());
		} else {
			newStmtList->addChildren(parseStmt());
		}
	};

	return newStmtList;
};

ASTNode* ASTParser::parseStmt() {
	// Get 2nd token
	Token first = tokens.front();
	tokens.pop_front();
	Token second = tokens.front();
	tokens.push_front(first);

	// Check what statement it is
	if (first.val == "read" && second.val != "=") {
		// Read call
		return parseRead();
	} else if (first.val == "print" && second.val != "=") {
		return parsePrint();
	} else if (first.val == "call" && second.val != "=") {
		return parseCall();
	} else if (first.tokenType == TokenType::Identifier && second.val == "=") {
		// Assignment Type
		return parseAssign();
	} else {
		throw std::logic_error("Invalid statement");
	}
}

ASTNode* ASTParser::parseRead() {
	stmtNumber = stmtNumber + 1;
	
	ASTNode* readNode;
	require({ TokenType::Identifier, "read" }, true);
	readNode = new ReadNode(stmtNumber, require({ TokenType::Identifier, "" }, false).val, 
		currentProcedure);

	// Get ;
	require({ TokenType::Delimiter, ";" }, true);

	return readNode;
}

ASTNode* ASTParser::parsePrint() {
	stmtNumber = stmtNumber + 1;

	ASTNode* printNode;
	require({ TokenType::Identifier, "print" }, true);
	printNode = new PrintNode(stmtNumber, require({ TokenType::Identifier, "" }, false).val,
		currentProcedure);

	// Get ;
	require({ TokenType::Delimiter, ";" }, true);

	return printNode;
}

ASTNode* ASTParser::parseCall() {
	stmtNumber = stmtNumber + 1;

	ASTNode* callNode;
	require({ TokenType::Identifier, "call" }, true);
	callNode = new CallNode(stmtNumber, require({ TokenType::Identifier, "" }, false).val,
		currentProcedure);

	// Get ;
	require({ TokenType::Delimiter, ";" }, true);

	return callNode;
}

ASTNode* ASTParser::parseAssign() {
	stmtNumber = stmtNumber + 1;

	AssignmentNode* newAssignment = new AssignmentNode(stmtNumber, currentProcedure);

	// Get variable to be assigned to
	std::string assigneeName = require({ TokenType::Identifier, "assigneeName" }, false).val;
	VariableNode* assignee = new VariableNode(assigneeName, currentProcedure);
	newAssignment->addChildren(assignee);

	// Get Operator
	require({ TokenType::Operator, "=" }, true);

	// Get assigning variable / number 
	
	std::list<Token> tokenList;
	popTokens(&tokenList, ";");
	ASTNode* RHS = parseExpression(tokenList);
	newAssignment->addChildren(RHS);

	// Get ;
	require({ TokenType::Delimiter, ";" }, true);

	return newAssignment;
};

ASTNode* ASTParser::parseConditionalExpression(std::list<Token> tokenList) {

	std::list<Token> expressionStack;
	std::stack<Token> operatorStack;
	std::stack<ASTNode*> outputQueue;
	while (!tokenList.empty()) {
		Token t = tokenList.front();
		tokenList.pop_front();

		if (t.val == "!") {
			pushNot(t, &tokenList, &outputQueue, &expressionStack);
		}
		else if (t.val == "(") {
			expressionStack.push_front(t);
			operatorStack.push(t);
		}
		else if (t.tokenType == TokenType::BooleanOperator) {
			operatorStack.push(t);
		}
		else if (t.tokenType == TokenType::ComparisonOperator) {
			operatorStack.push(t);
			expressionStack.push_front(t);
		}
		else if (t.val == ")") {
			pushRightParenthesis(t, &operatorStack, &outputQueue, &expressionStack);
		}
		else {
			expressionStack.push_front(t);
		}
	}

	if (operatorStack.size() > 1)
		throw std::logic_error("Invalid logic operators");

	if (operatorStack.size() == 1 ) {
		queueLastOperator(&operatorStack, &outputQueue, &expressionStack);
	}
	
	if(outputQueue.size() != 1)
		throw std::logic_error("Invalid conditional statement");
	return outputQueue.top();
}

void ASTParser::pushNot(Token t, std::list<Token>* tokenList, std::stack<ASTNode*>* outputQueue, std::list<Token>* expressionStack) {
	if (tokenList->empty()) {
		throw std::logic_error("no tokens");
	}
	else if (tokenList->front().val == "(") {
		//parsing !( ) operator
		std::list<Token> insideParenthesis;
		tokenList->pop_front();
		popTokensToRightParenthesis(tokenList, &insideParenthesis);
		tokenList->pop_front();
		if (insideParenthesis.empty())
			throw std::logic_error("Empty relational expression");
		OperatorNode* op = new OperatorNode(t.val, currentProcedure);
		op->addChildren(parseConditionalExpression(insideParenthesis));
		outputQueue->push(op);
	}
	else {
		expressionStack->push_front(t);
	}
}

void ASTParser::pushRightParenthesis(Token t, std::stack<Token>* operatorStack, std::stack<ASTNode*>* outputQueue, std::list<Token>* expressionStack){
	if (expressionStack->front().val == "(")
		expressionStack->pop_front();
	else {
		expressionStack->push_front(t);
	}

	if (operatorStack->top().val == "(") {
		//Pop matching parenthesis in operators
		operatorStack->pop();
	}
	else if (operatorStack->top().tokenType == TokenType::ComparisonOperator) {
		//Queue Relational Expression when operation stack is "(" "op" ")"  
		queueRelationalExpression(t, operatorStack, outputQueue, expressionStack);
	}
	else if (operatorStack->top().tokenType == TokenType::BooleanOperator) {
		//Parse boolean operator by combining with 2 nodes in outputQueue
		queueConditionalExpression(operatorStack, outputQueue);
	}
	else
		throw std::logic_error("Expected parenthesis with operator " + t.val);
}



void ASTParser::queueRelationalExpression(Token t, std::stack<Token>* operatorStack, std::stack<ASTNode*>* outputQueue, std::list<Token>* expressionStack) {
	Token op = operatorStack->top();
	//Pop off Operator
	operatorStack->pop();
	if (operatorStack->empty() || operatorStack->top().val != "(")
		throw std::logic_error("Expected parenthesis with operator " + op.val);
	//Remove first bounding parenthesis 
	operatorStack->pop();

	if (expressionStack->empty() || expressionStack->front().val != ")")
		throw std::logic_error("Expected parenthesis with operator " + t.val);

	//Find Relational expression by traversing back in expression stack to find corresponding left parenthesis
	std::list<Token> insideParenthesis;
	int rightParenthesis = 1;
	expressionStack->pop_front();
	while (rightParenthesis != 0) {
		if (expressionStack->empty())
			throw std::logic_error("Expected parenthesis with operator " + t.val);
		if (expressionStack->front().val == ")")
			rightParenthesis += 1;
		else if (expressionStack->front().val == "(") {
			rightParenthesis -= 1;
		}
		if (rightParenthesis == 0)
			break;
		insideParenthesis.push_front(expressionStack->front());
		expressionStack->pop_front();
	}
	expressionStack->pop_front();


	if (insideParenthesis.empty())
		throw std::logic_error("Empty relational expression");

	//Parse and Queue
	outputQueue->push(parseRelationalExpression(insideParenthesis));
}

void ASTParser::queueConditionalExpression(std::stack<Token>* operatorStack, std::stack<ASTNode*>* outputQueue) {
	Token op = operatorStack->top();
	operatorStack->pop();

	//Remove first bounding parenthesis 
	if (operatorStack->empty() || operatorStack->top().val != "(")
		throw std::logic_error("Expected parenthesis with operator " + op.val);
	operatorStack->pop();

	//Restack operator and parse
	if (outputQueue->size() < 2)
		throw std::logic_error("Expected conditional statements");
	operatorStack->push(op);
	queueOperator(operatorStack, outputQueue);
}


void ASTParser::queueLastOperator(std::stack<Token>* operatorStack, std::stack<ASTNode*>* outputQueue, std::list<Token>* expressionStack) {
	if (operatorStack->top().tokenType == TokenType::BooleanOperator)
		queueOperator(operatorStack, outputQueue);
	else if (operatorStack->top().tokenType == TokenType::ComparisonOperator) {
		std::list<Token> conditionalList;
		while (!expressionStack->empty()) {
			conditionalList.push_front(expressionStack->front());
			expressionStack->pop_front();
		}
		outputQueue->push(parseRelationalExpression(conditionalList));
	}
}



ASTNode* ASTParser::parseRelationalExpression(std::list<Token> tokenList) {
	std::list<Token> LHS;
	std::stack<Token> operatorStack;
	std::stack<ASTNode*> outputQueue;

	//Find Conditional Operator
	while (!tokenList.empty() && tokenList.front().tokenType!= TokenType::ComparisonOperator) {
		LHS.push_back(tokenList.front());
		tokenList.pop_front();
	}

	if (tokenList.empty())
		throw std::logic_error("Invalid relational statement");
	
	operatorStack.push(tokenList.front());
	tokenList.pop_front();
	
	//Parse LHS and RHS
	outputQueue.push(parseExpression(LHS));
	outputQueue.push(parseExpression(tokenList));

	//Create operator node
	queueOperator(&operatorStack, &outputQueue);

	return outputQueue.top();
}

ASTNode* ASTParser::parseExpression(std::list<Token> tokenList) {
	std::stack<Token> operatorStack;
	std::stack<ASTNode*> outputQueue;
	if (tokenList.empty()) {
		throw std::logic_error("no tokens");
	}

	//Shunting Yard Algorithm
	while (!tokenList.empty()) {
		
		Token t = tokenList.front();
		tokenList.pop_front();
		if (t.tokenType == TokenType::Number)
			outputQueue.push(new ConstantNode(stoi(t.val), currentProcedure));
		else if (t.tokenType == TokenType::Identifier)
			outputQueue.push(new VariableNode(t.val, currentProcedure));
		else if (t.tokenType == TokenType::Operator && t.val != "=") {
			while (!operatorStack.empty() && operatorStack.top().val != "("
				&& opPrecedence(operatorStack.top()) >= opPrecedence(t)) {
				queueOperator(&operatorStack, &outputQueue);
			}
			operatorStack.push(t);
		}
		else if (t.val == "(")
			operatorStack.push(t);
		else if (t.val == ")") {
			while (operatorStack.top().val != "(") {
				if (operatorStack.empty())
					throw std::logic_error("Expected left parentesis");
				queueOperator(&operatorStack, &outputQueue);
			}

			if (operatorStack.empty() ||operatorStack.top().val != "(")
				throw std::logic_error("Expected left parentesis");
			operatorStack.pop();
		}
		else {
			throw std::logic_error("Unexpected token" + t.val);
		}
	}

		while (!operatorStack.empty()) {
			if (operatorStack.top().val == "(")
				throw std::logic_error("Expected right parentesis");
			queueOperator(&operatorStack, &outputQueue);
		}

		if(outputQueue.size() != 1)
			throw std::logic_error("Invalid Expression");

		return outputQueue.top();
}

int ASTParser::opPrecedence(Token t) {
	if (t.val == "+" || t.val == "-")
		return 2;
	else if (t.val == "*" || t.val == "/" || t.val == "%")
		return 3;
	return 0;
}

//Create Operator Node in outputQueue by combining the top two on outputQueue
void ASTParser::queueOperator(std::stack<Token>* operatorStack, std::stack<ASTNode*>* outputQueue) {
	Token t = operatorStack->top();
	operatorStack->pop();
	ASTNode* child[2] = {};
	for (int i = 0; i < 2; i++) {
		if (outputQueue->empty())
			throw std::logic_error("Invalid Token, Expected expression for operator " + t.val);
		child[i] = outputQueue->top();
		outputQueue->pop();
	}
	OperatorNode* op = new OperatorNode(t.val, currentProcedure);
	for (int i = 1; i >= 0; i--) {
		op->addChildren(child[i]);
	}

	outputQueue->push(op);
}

//Pop tokens from tokens into output tokens until ending Delimiter is reached 
void  ASTParser::popTokens(std::list<Token>* outputTokens, std::string endingDelimiter) {
	if (tokens.empty())
		throw std::logic_error("no tokens");

	while (tokens.front().val != endingDelimiter) {
		outputTokens->push_back(tokens.front());
		tokens.pop_front();
		if (tokens.empty())
			throw std::logic_error("no tokens");

	}
}

//Pop tokens from inputTokens into outputTokens until a corresponding right Parenthesis is mean
void  ASTParser::popTokensToRightParenthesis(std::list<Token>* inputToken,std::list<Token>* outputTokens) {
	int leftParenthesis = 0;
	if (inputToken->empty())
		throw std::logic_error("no tokens");

	while (!inputToken->empty() && !(inputToken->front().val == ")" && leftParenthesis == 0)) {
		if (inputToken->empty())
			throw std::logic_error("no tokens");
		if (inputToken->front().val == "(")
			leftParenthesis += 1;
		if (inputToken->front().val == ")")
			leftParenthesis -= 1;
		outputTokens->push_back(inputToken->front());
		inputToken->pop_front();
		if (inputToken->empty() && leftParenthesis !=0 )
			throw std::logic_error("no tokens");
	}
}