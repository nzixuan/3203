#pragma once
#include <vector>
#include <string>
#include "DesignExtractorVisitor.h"
#include "CFGVisitor.h"

class CFGAbstract
{
protected:
	std::vector<int> stmtList;
	CFGAbstract* procedure = NULL;
	CFGAbstract* nextNode = NULL;
	CFGAbstract* parentNode = NULL;
	bool isVisited = false;
public:
	CFGAbstract(CFGAbstract* procedurer) : procedure(procedure) { }
	virtual ~CFGAbstract() {} // Virtual Deconstructor in the case we need to delete nodes
	virtual void accept(CFGVisitor* v) = 0; // Virtual accept Method
	virtual std::string toString() = 0;

	virtual bool isCFGNode() = 0;

	std::vector<int> getStmts();
	void addStmtNumber(int stmtNumber);

	CFGAbstract* getNext();
	virtual void setNext(CFGAbstract* next) = 0;

	CFGAbstract* getParent();
	void setParent(CFGAbstract* parent);

	CFGAbstract* getProcedure();

	bool checkVisited();
	void visit();
};

class CFGRoot : public CFGAbstract
{
protected:
	std::string procedureName = "";
	int lastStmt = 0;
public:
	CFGRoot(std::string procedureName) : procedureName(procedureName), CFGAbstract(NULL) { }
	void accept(CFGVisitor* v);
	std::string toString();

	bool isCFGNode() {
		return false;
	}

	void setNext(CFGAbstract* next);

	std::string getProcedureName();

	int getLast();
	void setLast(int last);
};

class CFGNode : public CFGAbstract
{
public:
	CFGNode(CFGAbstract* procedure) : CFGAbstract(procedure) { }
	void accept(CFGVisitor* v);

	bool isCFGNode() {
		return true;
	}

	void setNext(CFGAbstract* next);

	std::string toString();
};

class CFGIf : public CFGAbstract
{
	CFGAbstract* thenBlock = NULL;
	CFGAbstract* elseBlock = NULL;
public:
	CFGIf(CFGAbstract* procedure)
		: CFGAbstract(procedure) { } //ifNode Constructor
	void accept(CFGVisitor* v);
	std::string toString();

	bool isCFGNode() {
		return false;
	}

	void setNext(CFGAbstract* next);

	CFGAbstract* getThen();
	void setThen(CFGAbstract* then);

	CFGAbstract* getElse();
	void setElse(CFGAbstract* elseThen);

};

class CFGWhile : public CFGAbstract
{
	CFGAbstract* whileBlock = NULL;
public:
	CFGWhile(CFGAbstract* procedure)
		: CFGAbstract(procedure) { } //whileNode Constructor
	void accept(CFGVisitor* v);
	std::string toString();

	bool isCFGNode() {
		return false;
	}

	void setNext(CFGAbstract* next);

	CFGAbstract* getWhile();
	void setWhile(CFGAbstract* whileThen);
};