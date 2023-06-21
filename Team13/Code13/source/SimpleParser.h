#pragma once
#include <memory>
#include <stack>
#include <queue>

#include "Tokenizer.h"
#include "Token.h"
#include "AST.h"
#include "CFG.h"
 
class SIMPLEParser
{
protected:
	std::list<Token> tokens;
	int stmtNumber = 0;
	ProcedureNode* currentProcedure = new ProcedureNode("temp");
	Token require(Token token, bool checkVal) {
		// Checks if next token is the right token type, returns true is so, else false
		if (tokens.empty()) {
			throw std::logic_error("No more tokens");
		}
		else {
			if (tokens.front().tokenType == token.tokenType && !checkVal) {
				Token currentToken = tokens.front();
				tokens.pop_front();
				return currentToken;
			}
			else if (tokens.front().tokenType == token.tokenType && tokens.front().val == token.val) {
				Token currentToken = tokens.front();
				tokens.pop_front();
				return currentToken;
			}
			throw std::logic_error("Invalid Token, Expected " + token.val +
				" but received " + tokens.front().val);
		}
	}
public:
	SIMPLEParser(std::list<Token> tokens) : tokens(tokens) {	}
};

class ASTParser : public SIMPLEParser
{
protected:
	ASTNode* root = new ASTRoot();
	ASTNode* parseTokenList();
	ASTNode* parseProcedure();
	ASTNode* parseIf();
	ASTNode* parseWhile();
	ASTNode* parseStmtList();
	ASTNode* parseStmt();
	ASTNode* parseRead();
	ASTNode* parsePrint();
	ASTNode* parseCall();
	ASTNode* parseAssign();
	ASTNode* parseExpression(std::list<Token> tokenList);
	ASTNode* parseRelationalExpression(std::list<Token> tokenList);
	ASTNode* parseConditionalExpression(std::list<Token> tokenList);

	int opPrecedence(Token t);
	void queueOperator(std::stack<Token>* operatorStack, std::stack<ASTNode*>* outputQueue);
	void popTokens(std::list<Token>* outputTokens, std::string endingDelimiter);
	void popTokensToRightParenthesis(std::list<Token>* inputToken, std::list<Token>* outputTokens);
	void pushNot(Token t, std::list<Token>* tokenList, std::stack<ASTNode*>* outputQueue, std::list<Token>* expressionStack);
	void pushRightParenthesis(Token t, std::stack<Token>* operatorStack, std::stack<ASTNode*>* outputQueue, std::list<Token>* expressionStack);
	void queueRelationalExpression(Token t, std::stack<Token>* operatorStack, std::stack<ASTNode*>* outputQueue, std::list<Token>* expressionStack);
	void queueConditionalExpression(std::stack<Token>* operatorStack, std::stack<ASTNode*>* outputQueue);
	void queueLastOperator(std::stack<Token>* operatorStack, std::stack<ASTNode*>* outputQueue, std::list<Token>* expressionStack);


public:
	ASTParser(std::list<Token> tokens) : SIMPLEParser(tokens) {	}

	void generateAST() {
		root = parseTokenList();
	}

	ASTNode* returnASTTree() {
		return root;
	}
};

class CFGParser : public SIMPLEParser
{
protected:
	std::list<CFGRoot*> CFGList;
	CFGRoot* currentProcedure = new CFGRoot("temp");
	std::list<CFGRoot*> parseTokenList();
	CFGRoot* parseProcedure();
	CFGAbstract* parseIf();
	CFGAbstract* parseWhile();
	CFGAbstract* parseStmtList();
	CFGAbstract* parseStmts();

public:
	CFGParser(std::list<Token> tokens) : SIMPLEParser(tokens) {	}

	void generateCFG() {
		CFGList = parseTokenList();
	}

	std::list<CFGRoot*> returnCFGList() {
		return CFGList;
	}
};
