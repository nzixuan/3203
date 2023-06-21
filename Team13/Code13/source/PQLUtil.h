#pragma once
#include <memory>
#include <stack>
#include <string>

#include "PQLGrammar.h"
#include "PQLList.h"
#include "RelArg.h"

std::string infixToPostfix(PQLList tokens);

std::string processTokens(Token token, std::stack<Token>* stk);

// Helper functions to empty the stack
// Empties the stack and accumulates the Token values into res
std::string emptyStack(std::stack<Token>* stk);

// Empties the stack until the top matches cond
std::string emptyStackUntilMatch(std::stack<Token>* stk, Token cond);

// Empties the stack until the prec(cond) > prec(stk.top())
std::string emptyStackUntilLowerEqPrec(std::stack<Token>* stk, Token cond);

// Helper function to get precedence of operators
int prec(Token token);

// Determines the WithRefType of the given RelArg
WithRefType getWithRefType(std::shared_ptr<RelArg> relArg);
