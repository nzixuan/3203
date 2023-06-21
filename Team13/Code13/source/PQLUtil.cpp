#include "PQLUtil.h"

#include "AttrRef.h"
#include "PQLExcept.h"

std::string infixToPostfix(PQLList tokens) {
  std::string val;
  Token curr;
  std::stack<Token> stk;
  while (!tokens.empty()) {
    curr = tokens.popFront();
    val += processTokens(curr, &stk);
  }

  val += emptyStack(&stk);

  // Remove last trailing space
  val = val.substr(0, val.size() - 1);
  return val;
}

std::string processTokens(Token token, std::stack<Token>* stk) {
  // We use a space to separate different variables
  std::string val;

  switch (token.tokenType) {
    case TokenType::Identifier:
    case TokenType::Number:
      return token.val + " ";
    case TokenType::Delimiter:
      if (token == PQLToken::Bracket::RightBracket) {
        val += emptyStackUntilMatch(stk, PQLToken::Bracket::LeftBracket);
        // We don't need the left bracket
        stk->pop();
        return val;
      } else if (token == PQLToken::Bracket::LeftBracket) {
        stk->push(token);
        return "";
      }
      break;
    case TokenType::Operator:
      val += emptyStackUntilLowerEqPrec(stk, token);
      stk->push(token);
      return val;
    default:
      // Shouldn't happen
      throwSyntaxError();
  }
}

// Helper functions to empty the stack in various ways
std::string emptyStack(std::stack<Token>* stk) {
  // Empty the stack
  std::string res;
  while (!stk->empty()) {
    res += stk->top().val + " ";
    stk->pop();
  }
  return res;
}

std::string emptyStackUntilMatch(std::stack<Token>* stk, Token cond) {
  std::string res;
  while (stk->top() != cond) {
    res += stk->top().val + " ";
    stk->pop();
  }
  return res;
}

std::string emptyStackUntilLowerEqPrec(std::stack<Token>* stk, Token cond) {
  std::string res;
  while (!stk->empty() && prec(cond) <= prec(stk->top())) {
    res += stk->top().val + " ";
    stk->pop();
  }
  return res;
}

int prec(Token token) {
  if (token == PQLToken::Operator::Star ||
      token == PQLToken::Operator::Divide || token == PQLToken::Operator::Mod) {
    return 2;
  } else if (token == PQLToken::Operator::Plus ||
             token == PQLToken::Operator::Minus) {
    return 1;
  } else {
    return -1;
  }
}

WithRefType getWithRefType(std::shared_ptr<RelArg> relArg) {
  switch (relArg->getRelArgType()) {
    case RelArgType::Number:
      return WithRefType::Integer;
    case RelArgType::SourceArg:
      return WithRefType::Name;
    case RelArgType::AttrRef:
      std::shared_ptr<AttrRef> ar = std::dynamic_pointer_cast<AttrRef>(relArg);
      if (ar->getAttrName() == AttrName::Value ||
          ar->getAttrName() == AttrName::StmtNo) {
        return WithRefType::Integer;
      }
      return WithRefType::Name;
  }
}
