#include <stdio.h>
#include <string>
#include <vector>

#include "AST.h"

// ASTNode functions
std::list<ASTNode*> ASTNode::getChildren() {
	return children;
}

ASTNode* ASTNode::removeFirstChild() {
	ASTNode* first = children.front();
	children.pop_front();
	return first;
}

void ASTNode::addChildren(ASTNode* child) {
	children.push_back(child);
	child->setParent(this);
}

ProcedureNode* ASTNode::getProcedure() {
	return procedureNode;
}


ASTNode* ASTNode::getParent() {
	return parentNode;
}

void ASTNode::setParent(ASTNode* parent) {
	parentNode = parent;
}

bool ASTNode::checkVisited() {
	return isVisited;
}

void ASTNode::visit() {
	isVisited = true;
}

bool ASTNode::isProcedureNode() {
	return false;
}

// StmtNode functions
int StmtNode::getStatementNumber() {
	return statementNumber;
};

std::string StmtNode::toString() {
	std::string result = "[StmtNode,";

	std::list<ASTNode*> children = this->getChildren();

	return result + " No. of Children: " + std::to_string(children.size()) +
		", Procedure Name:" + this->getProcedure()->getNodeName() + " ]";
}

std::string StmtNode::getPostfix() {
	return "";
}

std::string ASTRoot::toString() {
	std::string result = "[ASTRoot,";

	std::list<ASTNode*> children = this->getChildren();

	return result + " No. of Children: " + std::to_string(children.size()) + " ]";
}

void ASTRoot::accept(Visitor* v) {
}

std::string ASTRoot::getPostfix() {
	return "";
}

// ifNode functions
void IfNode::accept(Visitor* v) {
	v->visitIf(this);
}

std::string IfNode::toString() {
	std::string result = "[IfNode,";

	std::list<ASTNode*> children = this->getChildren();

	return result + " No. of Children: " + std::to_string(children.size()) + 
		", Procedure Name:" + this->getProcedure()->getNodeName() + " ]";
}

// whileNode functions
void WhileNode::accept(Visitor* v) {
	v->visitWhile(this);
}

std::string WhileNode::toString() {
	std::string result = "[WhileNode,";

	std::list<ASTNode*> children = this->getChildren();

	return result + " No. of Children: " + std::to_string(children.size()) +
		", Procedure Name:" + this->getProcedure()->getNodeName() + " ]";
}

// procedureNode functions
void ProcedureNode::accept(Visitor* v) {
	 v->visitProcedure(this);
}

std::string ProcedureNode::getNodeName() {
	return name;
};

std::string ProcedureNode::toString() {
	std::string result = "[ProcedureNode: ";

	std::list<ASTNode*> children = this->getChildren();

	return result + this->getNodeName() + ", No. of Children: " + std::to_string(children.size()) + " ]";
}

std::string ProcedureNode::getPostfix() {
	return "";
}

bool ProcedureNode::isProcedureNode() {
	return true;
}

// stmtListNode functions
void StmtListNode::accept(Visitor* v) {
	 v->visitStmtList(this);
}

std::string StmtListNode::toString() {
	std::string result = "[StmtListNode,";

	std::list<ASTNode*> children = this->getChildren();

	return result + " No. of Children: " + std::to_string(children.size()) +
		", Procedure Name:" + this->getProcedure()->getNodeName() + " ]";
}

std::string StmtListNode::getPostfix() {
	return "";
}

// assignmentNode functions
void AssignmentNode::accept(Visitor* v) {
	 v->visitAssignment(this);
}

std::string AssignmentNode::toString() {
	std::string result = "[AssignmentNode,";

	std::list<ASTNode*> children = this->getChildren();

	return result + " No. of Children: " + std::to_string(children.size()) +
		", Procedure Name:" + this->getProcedure()->getNodeName() + " ]";
}

// operatorNode functions
std::string OperatorNode::getOperator() {
	return op;
}

void OperatorNode::accept(Visitor* v) {
	 v->visitOperator(this);
}

std::string OperatorNode::toString() {
	std::string result = "[OperatorNode: ";

	std::list<ASTNode*> children = this->getChildren();

	return result + this->getOperator() + ", No. of Children: " + std::to_string(children.size()) +
		", Procedure Name:" + this->getProcedure()->getNodeName() + " ]";
}

std::string OperatorNode::getPostfix() {
	std::string postfix = "";
	for (auto child : getChildren()) {
		postfix += child->getPostfix() + " ";
	}
	return postfix + this->getOperator();
};

// variableNode functions
void VariableNode::accept(Visitor* v) {
	 v->visitVariable(this);
}

std::string VariableNode::getVariable() {
	return variable;
};

std::string VariableNode::toString() {
	std::string result = "[VariableNode: ";

	std::list<ASTNode*> children = this->getChildren();

	return result + this->getVariable() + ", No. of Children: " + std::to_string(children.size()) +
		", Procedure Name:" + this->getProcedure()->getNodeName() + " ]";
}

std::string VariableNode::getPostfix() {
	return this->getVariable();
}

// ConstantNode functions
int ConstantNode::getConstant() {
	return constant;
};

void ConstantNode::accept(Visitor* v) {
	v->visitConstant(this);
}
std::string ConstantNode::getPostfix() {
	return std::to_string(this->getConstant());
}

std::string ConstantNode::toString() {
	std::string result = "[ConstantNode: ";

	std::list<ASTNode*> children = this->getChildren();

	return result + std::to_string(this->getConstant()) + ", No. of Children: " + std::to_string(children.size()) +
		", Procedure Name:" + this->getProcedure()->getNodeName() + " ]";
}

// ReadNode functions
std::string ReadNode::getReadTarget() {
	return readTarget;
};

void ReadNode::accept(Visitor* v) {
	v->visitRead(this);
}

std::string ReadNode::toString() {
	std::string result = "[ReadNode,";

	std::list<ASTNode*> children = this->getChildren();

	return result + " No. of Children: " + std::to_string(children.size()) +
		", Procedure Name:" + this->getProcedure()->getNodeName() + " ]";
}

// PrintNode functions
std::string PrintNode::getPrintTarget() {
	return printTarget;
};

void PrintNode::accept(Visitor* v) {
	v->visitPrint(this); 
}

std::string PrintNode::toString() {
	std::string result = "[PrintNode,";

	std::list<ASTNode*> children = this->getChildren();

	return result + " No. of Children: " + std::to_string(children.size()) +
		", Procedure Name:" + this->getProcedure()->getNodeName() + " ]";
}

// CallNode functions
std::string CallNode::getCallTarget() {
	return callTarget;
};

void CallNode::accept(Visitor* v) {
	v->visitCall(this); 
}

std::string CallNode::toString() {
	std::string result = "[CallNode,";

	std::list<ASTNode*> children = this->getChildren();

	return result + " No. of Children: " + std::to_string(children.size()) +
		", Procedure Name:" + this->getProcedure()->getNodeName() + " ]";
}
