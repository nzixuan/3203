#pragma once
#include <list>
#include <string>
#include "DesignExtractorVisitor.h"

class ASTNode
{
protected:
	std::list<ASTNode*> children; // List of ASTNode pointers
	ProcedureNode* procedureNode = NULL;
	ASTNode* parentNode = NULL;
	bool isVisited = false;
public:
	ASTNode(ProcedureNode* procedureNode) : procedureNode(procedureNode) { }
	virtual ~ASTNode() {} // Virtual Deconstructor in the case we need to delete nodes
	virtual void accept(Visitor *v) = 0; // Virtual accept Method
	virtual std::string toString() = 0;
	virtual std::string getPostfix() = 0;

	std::list<ASTNode*> getChildren();
	ASTNode* removeFirstChild();
	void addChildren(ASTNode* child); // Adds ASTNode pointers to the list

	ProcedureNode* getProcedure();

	ASTNode* getParent();
	void setParent(ASTNode* parent);

	bool checkVisited();
	void visit();

	bool isProcedureNode();
};

class StmtNode : public ASTNode
{
protected:
	int statementNumber;
public:
	virtual void accept(Visitor* v) = 0;
	StmtNode(int statementNumber, ProcedureNode* procedureNode) 
		: statementNumber(statementNumber), ASTNode(procedureNode) { }
	int getStatementNumber();
	
	std::string toString();
	std::string getPostfix();

};

class ASTRoot : public ASTNode
{
public:
	ASTRoot() : ASTNode(NULL) { }
	void accept(Visitor* v);
	std::string toString();
	std::string getPostfix();

};

class IfNode : public StmtNode {
public:
	IfNode(int statementNumber, ProcedureNode* procedureNode)
		: StmtNode(statementNumber, procedureNode) { } //ifNode Constructor
	void accept(Visitor* v);
	std::string toString();
};

class WhileNode : public StmtNode {
public:
	WhileNode(int statementNumber, ProcedureNode* procedureNode)
		: StmtNode(statementNumber, procedureNode) { } //whileNode Constructor
	void accept(Visitor* v);
	std::string toString();
};

class ProcedureNode : public ASTNode {
	std::string name;
public:
	ProcedureNode(std::string name)
		: name(name), ASTNode(NULL) { }
	void accept(Visitor* v);
	std::string getNodeName();
	std::string toString();
	bool isProcedureNode();
	std::string getPostfix();
};

class StmtListNode : public ASTNode {
public:
	StmtListNode(ProcedureNode* procedureNode)
		: ASTNode(procedureNode) { }
	void accept(Visitor* v);
	std::string toString();
	std::string getPostfix();

};

class AssignmentNode : public StmtNode {
public:
	AssignmentNode(int statementNumber, ProcedureNode* procedureNode)
		: StmtNode(statementNumber, procedureNode) { }
	void accept(Visitor* v);
	std::string toString();
};

class OperatorNode : public ASTNode {
	std::string op;
public:
	OperatorNode(std::string op, ProcedureNode* procedureNode) 
		: op(op), ASTNode(procedureNode) { }
	std::string getOperator();
	void accept(Visitor* v);
	std::string toString();
	std::string getPostfix();
};

class VariableNode : public ASTNode {
	std::string variable;
public:
	VariableNode(std::string variable, ProcedureNode* procedureNode)
		: variable(variable), ASTNode(procedureNode) { }
	std::string getVariable();
	void accept(Visitor* v);
	std::string toString();
	std::string getPostfix();
};

class ConstantNode : public ASTNode {
	int constant;
public:
	ConstantNode(int constant, ProcedureNode* procedureNode)
		: constant(constant), ASTNode(procedureNode) { }
	int getConstant();
	void accept(Visitor* v);
	std::string toString();
	std::string getPostfix();
};

class ReadNode : public StmtNode {
	std::string readTarget;
public:
	ReadNode(int statementNumber, std::string readTarget, ProcedureNode* procedureNode) 
		: StmtNode(statementNumber, procedureNode), readTarget(readTarget) { }
	std::string getReadTarget();
	void accept(Visitor* v);
	std::string toString();
};

class PrintNode : public StmtNode {
	std::string printTarget;
public:
	PrintNode(int statementNumber, std::string printTarget, ProcedureNode* procedureNode) :
		StmtNode(statementNumber, procedureNode), printTarget(printTarget) { }
	std::string getPrintTarget();
	void accept(Visitor* v);
	std::string toString();
};

class CallNode : public StmtNode {
	std::string callTarget;
public:
	CallNode(int statementNumber, std::string callTarget, ProcedureNode* procedureNode) :
		StmtNode(statementNumber, procedureNode), callTarget(callTarget) { }
	std::string getCallTarget();
	void accept(Visitor* v);
	std::string toString();
};
