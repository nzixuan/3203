#pragma once

#include "Token.h"

// Encapsulates all valid PQL Tokens that could be used in a query
namespace PQLToken {
namespace DesignEntity {
const Token Statement{TokenType::Identifier, "stmt"};
const Token Read{TokenType::Identifier, "read"};
const Token Print{TokenType::Identifier, "print"};
const Token Call{TokenType::Identifier, "call"};
const Token While{TokenType::Identifier, "while"};
const Token If{TokenType::Identifier, "if"};
const Token Assign{TokenType::Identifier, "assign"};
const Token Variable{TokenType::Identifier, "variable"};
const Token Constant{TokenType::Identifier, "constant"};
const Token Procedure{TokenType::Identifier, "procedure"};
}  // namespace DesignEntity

const Token Select{TokenType::Identifier, "Select"};
const Token Boolean{TokenType::Identifier, "BOOLEAN"};

namespace Clause {
const Token Such{TokenType::Identifier, "such"};
const Token That{TokenType::Identifier, "that"};
const Token Pattern{TokenType::Identifier, "pattern"};
const Token With{TokenType::Identifier, "with"};
const Token And{TokenType::Identifier, "and"};
}  // namespace Clause

namespace Relationship {
const Token Follows{TokenType::Identifier, "Follows"};
const Token FollowsT{TokenType::Identifier, "Follows*"};
const Token Parent{TokenType::Identifier, "Parent"};
const Token ParentT{TokenType::Identifier, "Parent*"};
const Token Uses{TokenType::Identifier, "Uses"};
const Token Modifies{TokenType::Identifier, "Modifies"};
const Token Calls{TokenType::Identifier, "Calls"};
const Token CallsT{TokenType::Identifier, "Calls*"};
const Token Next{TokenType::Identifier, "Next"};
const Token NextT{TokenType::Identifier, "Next*"};
const Token Affects{TokenType::Identifier, "Affects"};
const Token AffectsT{TokenType::Identifier, "Affects*"};
const Token Pattern{TokenType::Identifier, "pattern"};
const Token With{TokenType::Identifier, "with"};
}  // namespace Relationship

namespace Delimiter {
const Token Comma{TokenType::Delimiter, ","};
const Token Semicolon{TokenType::Delimiter, ";"};
const Token Underscore{TokenType::Delimiter, "_"};
const Token DoubleQuotes{TokenType::Delimiter, "\""};
const Token Period{TokenType::Delimiter, "."};
const Token Hashtag{TokenType::Delimiter, "#"};
}  // namespace Delimiter

namespace Bracket {
const Token LeftAngularBracket{TokenType::ComparisonOperator, "<"};
const Token RightAngularBracket{TokenType::ComparisonOperator, ">"};
const Token LeftBracket{TokenType::Delimiter, "("};
const Token RightBracket{TokenType::Delimiter, ")"};
}  // namespace Bracket

namespace Operator {
const Token Plus{TokenType::Operator, "+"};
const Token Minus{TokenType::Operator, "-"};
const Token Star{TokenType::Operator, "*"};
const Token Divide{TokenType::Operator, "/"};
const Token Mod{TokenType::Operator, "%"};
const Token Equals{TokenType::Operator, "="};
}  // namespace Operator

namespace AttrName {
const Token ProcName{TokenType::Identifier, "procName"};
const Token VarName{TokenType::Identifier, "varName"};
const Token Value{TokenType::Identifier, "value"};
const Token StmtNo{TokenType::Identifier, "stmt#"};
}  // namespace AttrName
};  // namespace PQLToken
