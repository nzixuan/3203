#include "DesignExtractorVisitor.h"
#include "AST.h"
#include "common.h"
#include "PKB.h"
#include "DesignExtractor.h"


void DesignExtractorVisitor::visitIf(IfNode* node) {
	pkb->addStatement(node->getStatementNumber(), EntityType::If);
	PKBEntity lhs = pkb->getStmtEntity(node->getStatementNumber());
	visitContainerNode(node, &lhs, EntityType::If);
	
}

void DesignExtractorVisitor::visitWhile (WhileNode* node) {
	pkb->addStatement(node->getStatementNumber(), EntityType::While);
	PKBEntity lhs = pkb->getStmtEntity(node->getStatementNumber());
	visitContainerNode(node, &lhs, EntityType::While);
}

void DesignExtractorVisitor::visitContainerNode(StmtNode* node, PKBEntity* lhs, EntityType type) {
	std::list<ASTNode*> child = node->getChildren();
	std::list<ASTNode*>::iterator it = child.begin();
	IfWhileVisitor* visitor = new IfWhileVisitor(pkb, lhs, node->getStatementNumber(), type);
	ASTNode* ConditionalNode = *it;
	ConditionalNode->accept(visitor);
	it++;
	for (; it != child.end(); ++it) {
		std::list<ASTNode*>list_child = ((StmtListNode*)(*it))->getChildren();
		for (auto child_it : list_child) {
			PKBEntity rhs = pkb->getStmtEntity(((StmtNode*)(child_it))->getStatementNumber());
			//Parent RS
			pkb->storeRel(*(lhs), rhs, RelRefType::Parent);
			QueryConstant rhsQuery = { rhs.val, QueryConstantType::Number };
			storeRHSRelationshipsInLHS(lhs, rhsQuery);

		}
	}
}

void DesignExtractorVisitor::storeRHSRelationshipsInLHS(PKBEntity* lhs, QueryConstant rhsQuery) {
	//Uses RS
	storeRelationshipInLHS(lhs, rhsQuery, EntityType::Variable, RelRefType::Uses);
	//Modifies RS
	storeRelationshipInLHS(lhs, rhsQuery, EntityType::Variable, RelRefType::Modifies);
}

void DesignExtractorVisitor::storeRelationshipInLHS(PKBEntity* lhs, QueryConstant rhsQuery, EntityType entityType, RelRefType relRef) {
	std::list<PKBEntity> listOfRel = pkb->getLhsConstantRhsSynForRel(rhsQuery, entityType, relRef);
	for (auto entity : listOfRel) {
		pkb->storeRel(*(lhs), entity, relRef);
	}
}

void DesignExtractorVisitor::visitProcedure(ProcedureNode* node) {
	std::string name = node->getNodeName();
	pkb->addProcedure(name);
	//Store Relationship in parent

	std::list<ASTNode*> child = node->getChildren();
	std::list<ASTNode*>::iterator ite = child.begin();
	ASTNode* statementList = *ite;
	std::list<ASTNode*> stmtChild = statementList->getChildren();
	PKBEntity lhs = {node->getNodeName(), EntityType::Procedure};

	for (auto child_it : stmtChild) {
		PKBEntity rhs = pkb->getStmtEntity(((StmtNode*)(child_it))->getStatementNumber());
		QueryConstant rhsQuery = { rhs.val, QueryConstantType::Number };
		storeRHSRelationshipsInLHS(&lhs, rhsQuery);
	}
}

void DesignExtractorVisitor::visitStmtList(StmtListNode* node) {
	//Follows Relationship Extraction 
	std::list<ASTNode*> child = node->getChildren();
	std::list<ASTNode*>::iterator it = child.begin();
	std::list<ASTNode*>::iterator it_next = child.begin();
	if (it != child.end())
		it_next++;
	// Iterate through all statements in statment list
	for (; it_next != child.end(); it++, it_next++) {
		PKBEntity lhs = pkb->getStmtEntity(((StmtNode*)(*it))->getStatementNumber());
		PKBEntity rhs = pkb->getStmtEntity(((StmtNode*)(*it_next))->getStatementNumber());
		pkb->storeRel(lhs, rhs, RelRefType::Follows);
	}
}

void DesignExtractorVisitor::visitAssignment(AssignmentNode* node) {
	// Modifies(a, v)
	pkb->addStatement(node->getStatementNumber(), EntityType::Assign);
	PKBEntity lhs = pkb->getStmtEntity(node->getStatementNumber());
	std::list<ASTNode*> child = node->getChildren();
	std::list<ASTNode*>::iterator it = child.begin();
	PKBEntity rhs = {((VariableNode*)(*it))->getVariable(), EntityType::Variable };
	pkb->storeRel(lhs, rhs, RelRefType::Modifies);
	std::string var = ((VariableNode*)(*it))->getVariable();
	// Uses(a, v)
	AssignVisitor* uses = new AssignVisitor(pkb, &lhs, node->getStatementNumber(), var);
	// Accept all nodes on RHS of assignmentNode
	it++;
	ASTNode* assignRHS = *it;
	pkb->storeFullExpAssignMap(node->getStatementNumber(), var, assignRHS->getPostfix());
	assignRHS->accept(uses);
}

void DesignExtractorVisitor::visitOperator(OperatorNode* node) {}

void DesignExtractorVisitor::visitVariable(VariableNode* node) {
	std::string name = node->getVariable();
	pkb->addVariable(name);
}

void DesignExtractorVisitor::visitConstant(ConstantNode* node) {
	int val = node->getConstant();
	pkb->addConstant(std::to_string(val));
}

void DesignExtractorVisitor::visitRead(ReadNode* node) {
	pkb->addStatement(node->getStatementNumber(), EntityType::Read);
	pkb->addVariable(node->getReadTarget());
	pkb->addVarNameForRead(node->getStatementNumber(), node->getReadTarget());

	PKBEntity lhs = pkb->getStmtEntity(node->getStatementNumber());

	PKBEntity rhs = { node->getReadTarget(), EntityType::Variable};
	pkb->storeRel(lhs, rhs, RelRefType::Modifies);

}

void DesignExtractorVisitor::visitPrint(PrintNode* node) {
	pkb->addStatement(node->getStatementNumber(), EntityType::Print);
	pkb->addVariable(node->getPrintTarget());
	pkb->addVarNameForPrint(node->getStatementNumber(), node->getPrintTarget());
	PKBEntity lhs = pkb->getStmtEntity(node->getStatementNumber());

	PKBEntity rhs = { node->getPrintTarget(), EntityType::Variable };
	pkb->storeRel(lhs, rhs, RelRefType::Uses);
}

void DesignExtractorVisitor::visitCall(CallNode* node) {
	pkb->addStatement(node->getStatementNumber(), EntityType::Call);
	pkb->addProcNameForCall(node->getStatementNumber(), node->getCallTarget());

	QueryConstant targetQuery = { node->getCallTarget(), QueryConstantType::Identifier };
	QueryConstant currentQuery = { node->getProcedure()->getNodeName(), QueryConstantType::Identifier };
	PKBEntity currentProc = { node->getProcedure()->getNodeName(), EntityType::Procedure };
	PKBEntity callStmt = pkb->getStmtEntity(node->getStatementNumber());


	if (!pkb->hasLhsConstantRhsConstantForRel(targetQuery, currentQuery, RelRefType::CallsT) && !(node->getCallTarget() == node->getProcedure()->getNodeName())) {

	for (auto procedureNode : root->getChildren()) {
		if (((ProcedureNode*)procedureNode)->getNodeName() == node->getCallTarget()) {
			PKBEntity targetProc = { node->getCallTarget(), EntityType::Procedure };
			pkb->storeRel(currentProc, targetProc, RelRefType::Calls);
			acceptAllAST(procedureNode, this);
			storeRHSRelationshipsInLHS(&callStmt, targetQuery);
			return;
		}
	}
		throw std::logic_error("Call procedure Not Found");

	} else {
		throw std::logic_error("Cyclic procedure calls");
	}
};

// Assign Visitor
void AssignVisitor::visitIf(IfNode* node) {}; // Not relavent
void AssignVisitor::visitWhile(WhileNode* node) {}; // Not relavent
void AssignVisitor::visitProcedure(ProcedureNode* node) {}; // Not relavent
void AssignVisitor::visitStmtList(StmtListNode* node) {}; // Not relavent
void AssignVisitor::visitAssignment(AssignmentNode* node) {}; // Not relavent

void AssignVisitor::visitOperator(OperatorNode* node) {
	// Run Uses Visitor on both its children
	std::list<ASTNode*> children = node->getChildren();
	pkb->storeSubExpsAssignMap(stmtNo, lhsExpression, node->getPostfix());
	// Run Visitor on lhs
	children.front()->accept(this);
	
	while (children.size() > 1) {
		children.pop_front();
		children.front()->accept(this);
	}
};

void AssignVisitor::visitVariable(VariableNode* node) {
	PKBEntity rhs = { node->getVariable(), EntityType::Variable };
	pkb->storeRel(*lhs, rhs, RelRefType::Uses);
	pkb->storeSubExpsAssignMap(stmtNo, lhsExpression, node->getPostfix());

};

void AssignVisitor::visitConstant(ConstantNode* node) {
	pkb->storeSubExpsAssignMap(stmtNo, lhsExpression, node->getPostfix());
};

void AssignVisitor::visitRead(ReadNode* node) {}; // Not relavent
void AssignVisitor::visitPrint(PrintNode* node) {}; // Not relavent
void AssignVisitor::visitCall(CallNode* node) {}; // Not relavent


//If While Visitor 

void IfWhileVisitor::visitIf(IfNode* node) {}; // Not relavent
void IfWhileVisitor::visitWhile(WhileNode* node) {}; // Not relavent
void IfWhileVisitor::visitProcedure(ProcedureNode* node) {}; // Not relavent
void IfWhileVisitor::visitStmtList(StmtListNode* node) {}; // Not relavent
void IfWhileVisitor::visitAssignment(AssignmentNode* node) {}; // Not relavent

void IfWhileVisitor::visitOperator(OperatorNode* node) {
	// Run Uses Visitor on both its children
	std::list<ASTNode*> children = node->getChildren();
	// Run Visitor on lhs
	children.front()->accept(this);

	while (children.size() > 1) {
		children.pop_front();
		children.front()->accept(this);
	}
};

void IfWhileVisitor::visitVariable(VariableNode* node) {
	PKBEntity rhs = { node->getVariable(), EntityType::Variable };
	pkb->storeRel(*lhs, rhs, RelRefType::Uses);
	pkb->addControlVariable(type, stmtNo, node->getVariable());
};

void IfWhileVisitor::visitConstant(ConstantNode* node) {};// Not relavent
void IfWhileVisitor::visitRead(ReadNode* node) {}; // Not relavent
void IfWhileVisitor::visitPrint(PrintNode* node) {}; // Not relavent
void IfWhileVisitor::visitCall(CallNode* node) {}; // Not relavent