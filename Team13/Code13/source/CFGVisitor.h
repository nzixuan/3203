#pragma once
class PKB;
class CFGRoot;
class CFGNode;
class CFGIf;
class CFGWhile;
class CFGAbstract;

class CFGVisitor
{
protected:
	PKB* pkb;

public:
	CFGVisitor(PKB* pkb) : pkb(pkb) {}

	virtual void visitRoot(CFGRoot* node) = 0;
	virtual void visitNode(CFGNode* node) = 0;
	virtual void visitIf(CFGIf* node) = 0;
	virtual void visitWhile(CFGWhile* node) = 0;

};

class CFGNextVisitor :public CFGVisitor
{
public:
	CFGNextVisitor(PKB* pkb) : CFGVisitor(pkb) {}

	void visitRoot(CFGRoot* node);
	void visitNode(CFGNode* node);
	void visitIf(CFGIf* node);
	void visitWhile(CFGWhile* node);
	void storeNextRelWithinNode(CFGAbstract* node);
	void storeNextRelAndVisit(CFGAbstract* from, CFGAbstract* to);

};