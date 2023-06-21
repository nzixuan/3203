#include<stdio.h>
#include <string>
#include <vector>
#include "AST.h"
#include "DesignExtractorVisitor.h"
#include "DesignExtractor.h"
#include "CFGVisitor.h"
#include "PKB.h"


void acceptAllAST(ASTNode* ASTRoot, Visitor* visitor) {

	std::list<ASTNode*> child = ASTRoot->getChildren();
	std::list<ASTNode*>::iterator it = child.begin();

	if(!ASTRoot->checkVisited()){
		for (it = child.begin(); it != child.end(); ++it) {
			acceptAllAST(*it, visitor);
		}
	
		ASTRoot->accept(visitor);
		ASTRoot->visit();
	}
}

int DesignExtractor::extract(ASTNode* ASTRoot, std::list<CFGRoot*> CFGRootList, PKB* pkb) {
	DesignExtractorVisitor* ASTVisitor = new DesignExtractorVisitor(ASTRoot, pkb);
	acceptAllAST(ASTRoot, ASTVisitor);

	CFGNextVisitor* CFGVisitor = new CFGNextVisitor(pkb);
	for (auto root : CFGRootList) {
		root->accept(CFGVisitor);
	}
	return 0;
}
