#include <stdio.h>
#include <string>

#include "CFG.h"
#include "CFGVisitor.h"

// CFGAbstract functions
std::vector<int> CFGAbstract::getStmts() {
	return stmtList;
}

void CFGAbstract::addStmtNumber(int stmtNumber) {
	stmtList.emplace_back(stmtNumber);
}

CFGAbstract* CFGAbstract::getNext() {
	return nextNode;
}

CFGAbstract* CFGAbstract::getParent() {
	return parentNode;
}

void CFGAbstract::setParent(CFGAbstract* parent) {
	parentNode = parent;
}

CFGAbstract* CFGAbstract::getProcedure() {
	return procedure;
}

bool CFGAbstract::checkVisited() {
	return isVisited;
}

void CFGAbstract::visit() {
	isVisited = true;
}

// CFGRoot functions
void CFGRoot::accept(CFGVisitor* v) {
	v->visitRoot(this);
}

void CFGRoot::setNext(CFGAbstract* next) {
	nextNode = next;
	nextNode->setParent(this);
}

std::string CFGRoot::getProcedureName() {
	return procedureName;
}

int CFGRoot::getLast() {
	return lastStmt;
}

void CFGRoot::setLast(int last) {
	lastStmt = last;
}

std::string CFGRoot::toString() {
	std::string result = "[CFGRoot,";

	std::string stmts = std::to_string(stmtList.front());

	return result + " First Statement: " + stmts + " ]";
}

// CFGNode functions
void CFGNode::accept(CFGVisitor* v) {
	v->visitNode(this);
}

void CFGNode::setNext(CFGAbstract* next) {
	nextNode = next;
	nextNode->setParent(this);
}

std::string CFGNode::toString() {
	std::string result = "[CFGNode,";

	std::string stmts = std::to_string(stmtList.front());

	return result + " First Statement: " + stmts + " ]";
}

// CFGIf functions
void CFGIf::accept(CFGVisitor* v) {
	v->visitIf(this);
}

void CFGIf::setNext(CFGAbstract* next) {
	nextNode = next;

	// Look for the end of the thenBlock
	CFGAbstract* thenPointer = thenBlock;
	if (thenPointer != NULL) {
		while (thenPointer->getNext() != NULL) {
			thenPointer = thenPointer->getNext();
		}
	}

	CFGAbstract* elsePointer = elseBlock;
	if (elsePointer != NULL) {
		while (elsePointer->getNext() != NULL) {
			// Traverse to the end of thenBlock statements
			elsePointer = elsePointer->getNext();
		}
	}
	if (thenPointer != NULL) {
		thenPointer->setNext(next);
	}
	if (elsePointer != NULL) {
		elsePointer->setNext(next);
	}
	

	nextNode->setParent(this);
}

CFGAbstract* CFGIf::getThen() {
	return thenBlock;
}

void CFGIf::setThen(CFGAbstract* then) {
	CFGAbstract* pointer = thenBlock;
	if (pointer == NULL) {
		thenBlock = then;
	}
	else {
		// Look for the end of whileBlock
		while (pointer->getNext() != NULL && pointer->getNext() != this) {
			pointer = pointer->getNext();
		}
		pointer->setNext(then);
		then->setNext(nextNode);
	}
}

CFGAbstract* CFGIf::getElse() {
	return elseBlock;
}

void CFGIf::setElse(CFGAbstract* elseThen) {
	CFGAbstract* pointer = elseBlock;
	if (pointer == NULL) {
		elseBlock = elseThen;
	}
	else {
		// Look for the end of whileBlock
		while (pointer->getNext() != NULL && pointer->getNext() != this) {
			pointer = pointer->getNext();
		}
		pointer->setNext(elseThen);
		elseThen->setNext(nextNode);
	}
}

std::string CFGIf::toString() {
	std::string result = "[CFGIf,";

	return result + " First Statement: " + std::to_string(stmtList.front()) +
		" ThenBlock First Statement: " + std::to_string(thenBlock->getStmts().front()) +
		" ElseBlock First Statement" + std::to_string(elseBlock->getStmts().front()) + " ]";
}

// CFGWhile functions
void CFGWhile::accept(CFGVisitor* v) {
	v->visitWhile(this);
}

void CFGWhile::setNext(CFGAbstract* next) {
	nextNode = next;

	CFGAbstract* pointer = whileBlock;
	if (pointer != NULL) {
		// Look for the end of whileBlock
		while (pointer->getNext() != NULL && pointer->getNext() != this) {
			pointer = pointer->getNext();
		}
		if (pointer->getNext() != this) {
			pointer->setNext(this);
		}
	}
}

CFGAbstract* CFGWhile::getWhile() {
	return whileBlock;
}

void CFGWhile::setWhile(CFGAbstract* whileThen) {
	CFGAbstract* pointer = whileBlock;
	if (pointer == NULL) {
		whileBlock = whileThen;
		
	}
	else {
		// Look for the end of whileBlock
		while (pointer->getNext() != NULL && pointer->getNext() != this) {
			pointer = pointer->getNext();
		}
		pointer->setNext(whileThen);
	}
	pointer = whileThen;
	while (pointer->getNext() != NULL && pointer->getNext() != this) {
		pointer = pointer->getNext();
	}
	pointer->setNext(this);
}

std::string CFGWhile::toString() {
	std::string result = "[CFGWhile,";

	return result + " First Statement: " + std::to_string(stmtList.front()) +
		" WhileBlock First Statement: " + std::to_string(whileBlock->getStmts().front()) + " ]";
}