#pragma once
#include<string>

class PKB;
struct PKBEntity;
class IfNode;
class WhileNode;
class ProcedureNode;
class StmtListNode;
class AssignmentNode;
class OperatorNode;
class VariableNode;
class ConstantNode;
class ReadNode;
class PrintNode;
class CallNode;
class ASTNode;
class ASTNode;
class StmtNode;

struct QueryConstant;
enum class EntityType;
enum class RelRefType;

class Visitor
{
protected:
	PKB* pkb;

public:
	Visitor(PKB* pkb) : pkb(pkb) {}

	virtual void visitIf(IfNode *node) =0;
	virtual void visitWhile( WhileNode *node)=0;
	virtual void visitProcedure(ProcedureNode* node) = 0;
	virtual void visitStmtList(StmtListNode* node) = 0;
	virtual void visitAssignment(AssignmentNode* node) = 0;
	virtual void visitOperator(OperatorNode* node) = 0;
	virtual void visitVariable(VariableNode* node) = 0;
	virtual void visitConstant(ConstantNode* node) = 0;
	virtual void visitRead(ReadNode* node) = 0;
	virtual void visitPrint(PrintNode* node) = 0;
	virtual void visitCall(CallNode* node) = 0;

};

class DesignExtractorVisitor :public Visitor{
protected:
	ASTNode* root;
public:
	DesignExtractorVisitor(ASTNode* root, PKB* pkb) : root(root), Visitor(pkb)  {}
	
	void visitIf(IfNode* node);
	void visitWhile(WhileNode* node);
	void visitProcedure(ProcedureNode* node);
	void visitStmtList(StmtListNode* node);
	void visitAssignment(AssignmentNode* node);
	void visitOperator(OperatorNode* node);
	void visitVariable(VariableNode* node);
	void visitConstant(ConstantNode* node);
	void visitRead(ReadNode* node);
	void visitPrint(PrintNode* node);
	void visitCall(CallNode* node);
	void visitContainerNode(StmtNode* node, PKBEntity* lhs, EntityType type);
	void storeRHSRelationshipsInLHS(PKBEntity* lhs, QueryConstant rhsQuery);
	void storeRelationshipInLHS(PKBEntity* lhs, QueryConstant rhsQuery, EntityType entityType, RelRefType relRef);

};

class AssignVisitor :public Visitor {
protected:
	PKBEntity* lhs;
	int stmtNo;
	std::string lhsExpression;
public:
	AssignVisitor(PKB* pkb, PKBEntity* lhs, int stmtNo, std::string lhsExpression) : lhs(lhs), stmtNo(stmtNo), lhsExpression(lhsExpression), Visitor(pkb) {}

	void visitIf(IfNode* node); // Not relavent
	void visitWhile(WhileNode* node); // Not relavent
	void visitProcedure(ProcedureNode* node); // Not relavent
	void visitStmtList(StmtListNode* node); // Not relavent
	void visitAssignment(AssignmentNode* node); // Not relavent
	void visitOperator(OperatorNode* node);
	void visitVariable(VariableNode* node);
	void visitConstant(ConstantNode* node);
	void visitRead(ReadNode* node); // Not relavent
	void visitPrint(PrintNode* node); // Not relavent
	void visitCall(CallNode* node);// Not relavent
};

class IfWhileVisitor :public Visitor {
protected:
	PKBEntity* lhs;
	int stmtNo;
	EntityType type;
public:
	IfWhileVisitor(PKB* pkb, PKBEntity* lhs, int stmtNo, EntityType type) : lhs(lhs), stmtNo(stmtNo), type(type), Visitor(pkb) {}

	void visitIf(IfNode* node); // Not relavent
	void visitWhile(WhileNode* node); // Not relavent
	void visitProcedure(ProcedureNode* node); // Not relavent
	void visitStmtList(StmtListNode* node); // Not relavent
	void visitAssignment(AssignmentNode* node); // Not relavent
	void visitOperator(OperatorNode* node);
	void visitVariable(VariableNode* node);
	void visitConstant(ConstantNode* node);
	void visitRead(ReadNode* node); // Not relavent
	void visitPrint(PrintNode* node); // Not relavent
	void visitCall(CallNode* node);// Not relavent
};