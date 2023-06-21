#include "CFGVisitor.h"
#include "PKB.h"
#include "CFG.h"

void CFGNextVisitor::visitRoot(CFGRoot* node) {
	if (!node->checkVisited()) {
		node->visit();
		node->getNext()->accept(this);
	}
}
void CFGNextVisitor::visitNode(CFGNode* node){
	if (!node->checkVisited()) {
		node->visit();
		//Assume node->getStmts for both are not empty
		storeNextRelWithinNode(node);
		storeNextRelAndVisit(node,node->getNext());
	}
}
void CFGNextVisitor::visitIf(CFGIf* node){
	if (!node->checkVisited()) {
		node->visit();
		storeNextRelWithinNode(node);
		storeNextRelAndVisit(node, node->getThen());
		if (node->getElse()) {
			storeNextRelAndVisit(node, node->getElse());
		}
		else {
			storeNextRelAndVisit(node, node->getNext());
		}
	}
}
void CFGNextVisitor::visitWhile(CFGWhile* node){
	if (!node->checkVisited()) {
		node->visit();
		storeNextRelWithinNode(node);
		storeNextRelAndVisit(node, node->getWhile());
		storeNextRelAndVisit(node, node->getNext());
	}
}

void CFGNextVisitor::storeNextRelWithinNode(CFGAbstract* node) {
	std::vector<int> stmtList = node->getStmts();
	if (stmtList.size() > 0 ) {
	for (int i = 0; i < stmtList.size() - 1 ; i++) {
		pkb->storeRel(pkb->getStmtEntity(stmtList.at(i)), pkb->getStmtEntity(stmtList.at(i + 1)), RelRefType::Next);
	}
	}
}

void CFGNextVisitor::storeNextRelAndVisit(CFGAbstract* from, CFGAbstract* to) {
	if (to != NULL && from->getStmts().size() > 0 && to->getStmts().size() > 0) {
		pkb->storeRel(pkb->getStmtEntity(from->getStmts().back()),
			pkb->getStmtEntity(to->getStmts().at(0)),
			RelRefType::Next);
		to->accept(this);
	}
	else if (to != NULL) {
		to->accept(this);
	}
}
