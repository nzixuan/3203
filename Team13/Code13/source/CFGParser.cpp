#pragma once
#include <list>
#include <stdio.h>
#include <string>
#include <vector>
#include <stack>
#include <queue>

#include "PKB.h"
#include "CFG.h"
#include "DesignExtractorVisitor.h"
#include "SimpleParser.h"
#include "Token.h"

std::list<CFGRoot*> CFGParser::parseTokenList() {
	// Initialize parsing
	stmtNumber = 0;

	std::list<CFGRoot*> CFGList;

	while (tokens.empty() != true) {
		CFGList.push_back(parseProcedure());
	}

	return CFGList;
};

CFGRoot* CFGParser::parseProcedure() {
	require({ TokenType::Identifier, "procedure" }, true);
	std::string procedureName = require({ TokenType::Identifier, "procedureName" }, false).val;

	// Create CFGRoot
	CFGRoot* newRoot = new CFGRoot(procedureName);

	currentProcedure = newRoot;

	require({ TokenType::Delimiter, "{" }, true);

	CFGAbstract* childNode = parseStmtList();

	// End of procedure parsing and return AST Tree
	require({ TokenType::Delimiter, "}" }, true);
	newRoot->setNext(childNode);
	newRoot->setLast(stmtNumber);

	return newRoot;
};

CFGAbstract* CFGParser::parseIf() {
	// Initialize if node
	require({ TokenType::Identifier, "if" }, true);
	stmtNumber = stmtNumber + 1;

	// Create CFG node
	CFGIf* CFGIfNode = new CFGIf(currentProcedure);
	CFGIfNode->addStmtNumber(stmtNumber);

	// Parse condition
	require({ TokenType::Delimiter, "(" }, true);
	while (tokens.front().val != "then") {
		tokens.pop_front();
	}

	// Parse if statementList
	require({ TokenType::Identifier, "then" }, true);
	require({ TokenType::Delimiter, "{" }, true);

	CFGIfNode->setThen(parseStmtList());

	require({ TokenType::Delimiter, "}" }, true);

	// Parse else statementList
	if (tokens.front().val == "else") {
		require({ TokenType::Identifier, "else" }, true);
		require({ TokenType::Delimiter, "{" }, true);

		CFGIfNode->setElse(parseStmtList());

		require({ TokenType::Delimiter, "}" }, true);
	}

	return CFGIfNode;
};

CFGAbstract* CFGParser::parseWhile() {
	// Initialize while node
	require({ TokenType::Identifier, "while" }, true);
	stmtNumber = stmtNumber + 1;

	// Create CFG node
	CFGWhile* CFGWhileNode = new CFGWhile(currentProcedure);
	CFGWhileNode->addStmtNumber(stmtNumber);

	// Parse condition
	require({ TokenType::Delimiter, "(" }, true);
	while (tokens.front().val != "{") {
		tokens.pop_front();
	}

	// Parse while statementList
	require({ TokenType::Delimiter, "{" }, true);

	CFGWhileNode->setWhile(parseStmtList());

	require({ TokenType::Delimiter, "}" }, true);

	return CFGWhileNode;
};

CFGAbstract* CFGParser::parseStmtList() {
	CFGAbstract* newStmtList = new CFGNode(currentProcedure);
	CFGAbstract* pointer = newStmtList;

	while (!tokens.empty() && tokens.front().val != "}") {
		Token first = tokens.front();
		tokens.pop_front();
		Token second = tokens.front();
		tokens.push_front(first);

		if (first.val == "if" && second.val != "=") {
			if (newStmtList->getStmts().size() == 0) {
				newStmtList = parseIf();
				pointer = newStmtList;
			}
			else {
				pointer->setNext(parseIf());
				pointer = pointer->getNext();
			}
		}
		else if (first.val == "while" && second.val != "=") {
			if (newStmtList->getStmts().size() == 0) {
				newStmtList = parseWhile();
				pointer = newStmtList;
			}
			else {
				pointer->setNext(parseWhile());
				pointer = pointer->getNext();
			}
		}
		else {
			if (newStmtList->getStmts().size() == 0) {
				newStmtList = parseStmts();
				pointer = newStmtList;
			}
			else {
				pointer->setNext(parseStmts());
				pointer = pointer->getNext();
			}
		}
	};

	return newStmtList;
};

CFGAbstract* CFGParser::parseStmts() {
	CFGNode* workingNode = new CFGNode(currentProcedure);

	Token first = tokens.front();
	tokens.pop_front();
	Token second = tokens.front();
	tokens.push_front(first);

	while (!(first.val == "if" && second.val != "=") && !(first.val == "while" && second.val != "=") && first.val != "}") {
		while (tokens.front().val != ";") {
			tokens.pop_front();
		}
		stmtNumber = stmtNumber + 1;
		workingNode->addStmtNumber(stmtNumber);

		tokens.pop_front();

		if (tokens.empty()) {
			throw std::logic_error("Missing delimiter");
		}
		first = tokens.front();
		if (first.val == "}") {
			break;
		}
		tokens.pop_front();
		second = tokens.front();
		tokens.push_front(first);
	}

	return workingNode;
};
 