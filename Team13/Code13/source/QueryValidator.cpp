#include "QueryValidator.h"

#include <memory>

#include "PQLExcept.h"
#include "PQLUtil.h"

bool validateRefType(std::list<PQLList> relArgList, RelRefFormat formats,
                     std::map<std::string, EntityType>* synDesignEntMap) {
  if (relArgList.size() != formats.size()) {
    return false;
  }
  int n = formats.size();
  for (int i = 0; i < n; i++) {
    PQLList tokens = relArgList.front();
    RefType format = formats.front();
    relArgList.pop_front();
    formats.pop_front();

    switch (format) {
      case RefType::StmtRef:
        if (!validateStmtRef(tokens, synDesignEntMap)) return false;
        break;
      case RefType::EntRef:
        if (!validateEntRef(tokens, synDesignEntMap)) return false;
        break;
      case RefType::WithRef:
        if (!validateWithRef(tokens, synDesignEntMap)) return false;
        break;
      case RefType::ExprSpec:
        if (!validateExprSpec(tokens)) return false;
        break;
      case RefType::Wildcard:
        if (!validateWildCard(tokens)) return false;
        break;
      default:
        return false;
    }
  }
  // Each arg matches the format
  return true;
}

bool validateStmtRef(PQLList tokens,
                     std::map<std::string, EntityType>* synDesignEntMap) {
  // Check if we only have one token. StmtRef should be made up of only 1 token.
  // stmtRef: synonym | '_' | INTEGER
  if (tokens.empty() || tokens.size() > 1) {
    return false;
  }

  switch (tokens.peekFront().tokenType) {
    case TokenType::Identifier:
      // Synonym found
      return tokens.size() == 1;
    case TokenType::Number:
      // Integer
      return validateInteger(tokens.popFront());
    case TokenType::Delimiter:
      // Wildcard
      return tokens.advanceIfMatch(PQLToken::Delimiter::Underscore);
    default:
      // Unknown RefType/unrecognizable syntax for StmtRef.
      // Should not throw an error since input might fit other formats.
      return false;
  }
}

bool validateEntRef(PQLList tokens,
                    std::map<std::string, EntityType>* synDesignEntMap) {
  // Check if we only have one token.
  // entRef : synonym | '_' | '"' IDENT '"'
  if (tokens.empty()) {
    return false;
  }

  switch (tokens.peekFront().tokenType) {
    case TokenType::Identifier:
      // Synonym found
      return tokens.size() == 1;
    case TokenType::Delimiter:
      if (tokens.advanceIfMatch(PQLToken::Delimiter::Underscore) &&
          tokens.empty()) {
        // Wildcard
        return true;
      } else if (tokens.advanceIfFrontBackMatch(
                     PQLToken::Delimiter::DoubleQuotes)) {
        // "IDENT"
        return validateIdent(tokens.popFront()) && tokens.empty();
      }
    default:
      // Unknown RefType/unrecognizable syntax for EntRef.
      // Should not throw an error since input might fit other formats.
      return false;
  }
}

bool validateWithRef(PQLList tokens,
                     std::map<std::string, EntityType>* synDesignEntMap) {
  // withRef : '"' IDENT '"' | INTEGER | attrRef
  if (tokens.empty()) {
    return false;
  }

  Token token;
  switch (tokens.peekFront().tokenType) {
    case TokenType::Identifier:
      // Synonym found
      tokens.popFront();
      // We need an attrRef, no Synonym
      if (!tokens.advanceIfMatch(PQLToken::Delimiter::Period)) return false;

      token = tokens.popFront();
      if (token == PQLToken::DesignEntity::Statement) {
        // Trying to match stmt#
        return tokens.advanceIfMatch(PQLToken::Delimiter::Hashtag);
      }
      // Match the remaining possible AttrNames
      return tokenToAttrNameMap.count(token);
    case TokenType::Delimiter:
      // "IDENT"
      if (!tokens.advanceIfFrontBackMatch(PQLToken::Delimiter::DoubleQuotes)) {
        return false;
      }
      return validateIdent(tokens.popFront()) && tokens.empty();
    case TokenType::Number:
      // INTEGER
      return validateInteger(tokens.popFront()) && tokens.empty();
    default:
      // Unknown RefType/unrecognizable syntax for WithRef.
      // Should not throw an error since input might fit other formats.
      return false;
  }
}

bool validateExprSpec(PQLList tokens) {
  if (tokens.empty()) {
    return false;
  }

  if (tokens.peekFront().tokenType == TokenType::Delimiter) {
    if (tokens.size() == 1 &&
        tokens.advanceIfMatch(PQLToken::Delimiter::Underscore)) {
      // Wildcard
      return true;
    }
    // Strip the underscores if they exist both in front and back.
    tokens.advanceIfFrontBackMatch(PQLToken::Delimiter::Underscore);

    // Strip the quotation marks and validate the remaining expr
    return tokens.advanceIfFrontBackMatch(PQLToken::Delimiter::DoubleQuotes) &&
           validateExpr(tokens);
  }
  // Unknown RefType/unrecognizable syntax for ExprSpec.
  // Should not throw an error since input might fit other formats.
  return false;
}

bool validateExpr(PQLList tokens) {
  if (tokens.empty()) {
    return false;
  }
  // +1 whenever '(' is seen, -1 when ')' is seen. Should end with 0
  int bracketCount = 0;
  // Checks whether the expr fails the test at any point
  bool check = true;

  // Initialise values by checking the first token
  Token prevToken = tokens.popFront(), currToken;
  switch (prevToken.tokenType) {
    case TokenType::Delimiter:
      // If we start with '(', we must close with ')'
      check = prevToken == PQLToken::Bracket::LeftBracket && !tokens.empty();
      bracketCount++;
      break;
    case TokenType::Number:
      check = validateInteger(prevToken);
      break;
    case TokenType::Identifier:
      check = validateIdent(prevToken);
      break;
    default:
      check = false;
  }

  while (check && !tokens.empty()) {
    currToken = tokens.popFront();
    switch (currToken.tokenType) {
      case TokenType::Delimiter:
        if (currToken == PQLToken::Bracket::LeftBracket) {
          // Either '+(' [or any other operator] or '((' accepted
          check = prevToken.tokenType == TokenType::Operator ||
                  prevToken == currToken;
          bracketCount++;
        } else if (currToken == PQLToken::Bracket::RightBracket) {
          // Either 'x)', '2)', or '))' accepted
          check = prevToken.tokenType == TokenType::Identifier ||
                  prevToken.tokenType == TokenType::Number ||
                  prevToken == currToken;
          bracketCount--;
        } else {
          check = false;
        }
        break;
      case TokenType::Number:
        // Either '+2', '(2' accepted
        check = validateInteger(currToken) &&
                (prevToken.tokenType == TokenType::Operator ||
                 prevToken == PQLToken::Bracket::LeftBracket);
        break;
      case TokenType::Identifier:
        // Either '+x', '(x' accepted
        check = validateIdent(currToken) &&
                (prevToken.tokenType == TokenType::Operator ||
                 prevToken == PQLToken::Bracket::LeftBracket);
        break;
      case TokenType::Operator:
        // Either '2+', 'x+', ')-' accepted; expr also cannot end with an
        // operator
        check = (prevToken.tokenType == TokenType::Identifier ||
                 prevToken.tokenType == TokenType::Number ||
                 prevToken == PQLToken::Bracket::RightBracket) &&
                !tokens.empty();
        break;
      default:
        check = false;
    }
    prevToken = currToken;
  }

  return check && bracketCount == 0;
}

bool validateSemanticSuchThat(ClauseArgs clauseArgs, RelRefType relRef) {
  // Assumes tokens are syntactically valid
  std::shared_ptr<RelArg> lhs = clauseArgs.relArgs[0];
  std::shared_ptr<RelArg> rhs = clauseArgs.relArgs[1];

  EntityType lhsDesignEnt, rhsDesignEnt;
  bool isLhsValid = true, isRhsValid = true;

  switch (relRef) {
    case RelRefType::Modifies:
    case RelRefType::Uses:
      if (lhs->getRelArgType() == RelArgType::Wildcard) {
        return false;
      }
      if (lhs->getRelArgType() == RelArgType::Synonym) {
        // Is synonym. lhs is subset of stmt or procedure
        lhsDesignEnt = std::dynamic_pointer_cast<Synonym>(lhs)->getEntityType();
        isLhsValid = (lhsDesignEnt == EntityType::Procedure ||
                      stmtEntSet.count(lhsDesignEnt));
      }
      if (rhs->getRelArgType() == RelArgType::Synonym) {
        // rhs == variable
        rhsDesignEnt = std::dynamic_pointer_cast<Synonym>(rhs)->getEntityType();
        isRhsValid = rhsDesignEnt == EntityType::Variable;
      }
      break;
    case RelRefType::Parent:
    case RelRefType::ParentT:
    case RelRefType::Follows:
    case RelRefType::FollowsT:
    case RelRefType::Next:
    case RelRefType::NextT:
    case RelRefType::Affects:
    case RelRefType::AffectsT:
      // Synonyms must be stmt / stmt subset
      if (lhs->getRelArgType() == RelArgType::Synonym) {
        lhsDesignEnt = std::dynamic_pointer_cast<Synonym>(lhs)->getEntityType();
        isLhsValid = stmtEntSet.count(lhsDesignEnt);
      }
      if (rhs->getRelArgType() == RelArgType::Synonym) {
        rhsDesignEnt = std::dynamic_pointer_cast<Synonym>(rhs)->getEntityType();
        isRhsValid = stmtEntSet.count(rhsDesignEnt);
      }
      break;
    case RelRefType::Calls:
    case RelRefType::CallsT:
      // Synonyms must be procedure
      if (lhs->getRelArgType() == RelArgType::Synonym) {
        lhsDesignEnt = std::dynamic_pointer_cast<Synonym>(lhs)->getEntityType();
        isLhsValid = lhsDesignEnt == EntityType::Procedure;
      }
      if (rhs->getRelArgType() == RelArgType::Synonym) {
        rhsDesignEnt = std::dynamic_pointer_cast<Synonym>(rhs)->getEntityType();
        isRhsValid = rhsDesignEnt == EntityType::Procedure;
      }
      break;
    default:
      // Unknown RelRefType
      return false;
  }
  return isLhsValid && isRhsValid;
}

bool validateSemanticPattern(ClauseArgs clauseArgs) {
  std::shared_ptr<RelArg> lhs = clauseArgs.relArgs[0];
  EntityType lhsDesignEnt;
  bool isLhsValid = true;

  if (lhs->getRelArgType() == RelArgType::Synonym) {
    lhsDesignEnt = std::dynamic_pointer_cast<Synonym>(lhs)->getEntityType();
    isLhsValid = lhsDesignEnt == EntityType::Variable;
  }
  return isLhsValid;
}

bool validateSemanticWith(ClauseArgs clauseArgs) {
  std::shared_ptr<RelArg> lhs = clauseArgs.relArgs[0];
  std::shared_ptr<RelArg> rhs = clauseArgs.relArgs[1];
  bool isLhsValid = true, isRhsValid = true;

  if (lhs->getRelArgType() == RelArgType::AttrRef) {
    // Synonym of attrRef must match the attrName
    std::shared_ptr<AttrRef> ar = std::dynamic_pointer_cast<AttrRef>(lhs);
    isLhsValid =
        validateSemanticAttrRef(ar->getEntityType(), ar->getAttrName());
  }

  if (rhs->getRelArgType() == RelArgType::AttrRef) {
    std::shared_ptr<AttrRef> ar = std::dynamic_pointer_cast<AttrRef>(rhs);
    isRhsValid =
        validateSemanticAttrRef(ar->getEntityType(), ar->getAttrName());
  }

  return isLhsValid && isRhsValid && getWithRefType(lhs) == getWithRefType(rhs);
}

bool validateSemanticAttrRef(EntityType entityType, AttrName attrName) {
  switch (entityType) {
    case EntityType::Assign:
    case EntityType::If:
    case EntityType::Stmt:
    case EntityType::While:
      return attrName == AttrName::StmtNo;
    case EntityType::Print:
    case EntityType::Read:
      return attrName == AttrName::StmtNo || attrName == AttrName::VarName;
    case EntityType::Procedure:
      return attrName == AttrName::ProcName;
    case EntityType::Call:
      return attrName == AttrName::ProcName || attrName == AttrName::StmtNo;
    case EntityType::Constant:
      return attrName == AttrName::Value;
    case EntityType::Variable:
      return attrName == AttrName::VarName;
    default:
      // Should not reach here.
      return false;
  }
}

bool validateWildCard(PQLList tokens) {
  return tokens.size() == 1 &&
         tokens.advanceIfMatch(PQLToken::Delimiter::Underscore);
}

bool validateInteger(Token token) {
  // An invalid number has leading 0's
  return (token.val == "0" || token.val[0] != '0');
}

bool validateIdent(Token token) {
  return token.tokenType == TokenType::Identifier;
}
