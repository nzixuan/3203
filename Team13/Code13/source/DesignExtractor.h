#pragma once
class ASTNode;
class Visitor;
class PKB;
class CFGRoot;

void acceptAllAST(ASTNode* root, Visitor* visitor);

namespace DesignExtractor {
int extract(ASTNode* ASTRoot, std::list<CFGRoot*> CFGRootList, PKB* pkb);
}