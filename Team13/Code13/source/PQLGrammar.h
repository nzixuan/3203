#pragma once

#include <list>
#include <map>
#include <set>

#include "PQLToken.h"
#include "common.h"

/*
   This header file stores all the necessary data structures to
   parse PQL grammar.
*/
enum class ClauseType { None, SuchThat, Pattern, With };

enum class RefType { StmtRef, EntRef, ExprSpec, WithRef, Wildcard };

// Allows for variable number of arguments
typedef std::list<RefType> RelRefFormat;

// This enum further specifies the lhs/rhs of a clause for easier evaluation
enum class RelArgType { Synonym, Wildcard, Number, SourceArg, AttrRef };

// Used for attrRef
enum class AttrName { ProcName, VarName, Value, StmtNo };

enum class WithRefType { Name, Integer };

const std::set<EntityType> patternEntSet = {EntityType::Assign, EntityType::If,
                                            EntityType::While};

const std::set<EntityType> stmtEntSet = {
    EntityType::Assign, EntityType::If,    EntityType::Print, EntityType::Read,
    EntityType::Stmt,   EntityType::While, EntityType::Call};

const std::set<Token> designEntSet = {
    PQLToken::DesignEntity::Assign,   PQLToken::DesignEntity::Call,
    PQLToken::DesignEntity::Constant, PQLToken::DesignEntity::If,
    PQLToken::DesignEntity::Print,    PQLToken::DesignEntity::Procedure,
    PQLToken::DesignEntity::Read,     PQLToken::DesignEntity::Statement,
    PQLToken::DesignEntity::Variable, PQLToken::DesignEntity::While};

const std::set<Token> relRefSet = {
    PQLToken::Relationship::Follows,  PQLToken::Relationship::FollowsT,
    PQLToken::Relationship::Modifies, PQLToken::Relationship::Parent,
    PQLToken::Relationship::ParentT,  PQLToken::Relationship::Calls,
    PQLToken::Relationship::CallsT,   PQLToken::Relationship::Next,
    PQLToken::Relationship::NextT,    PQLToken::Relationship::Affects,
    PQLToken::Relationship::AffectsT, PQLToken::Relationship::Uses,
    PQLToken::Relationship::Pattern};

const std::map<Token, EntityType> tokenToEntMap = {
    {PQLToken::DesignEntity::Assign, EntityType::Assign},
    {PQLToken::DesignEntity::Call, EntityType::Call},
    {PQLToken::DesignEntity::Constant, EntityType::Constant},
    {PQLToken::DesignEntity::If, EntityType::If},
    {PQLToken::DesignEntity::Print, EntityType::Print},
    {PQLToken::DesignEntity::Procedure, EntityType::Procedure},
    {PQLToken::DesignEntity::Read, EntityType::Read},
    {PQLToken::DesignEntity::Statement, EntityType::Stmt},
    {PQLToken::DesignEntity::Variable, EntityType::Variable},
    {PQLToken::DesignEntity::While, EntityType::While}};

const std::map<Token, std::list<RelRefFormat>> relToRefTypeMap = {
    {PQLToken::Relationship::Follows,
     {RelRefFormat{RefType::StmtRef, RefType::StmtRef}}},
    {PQLToken::Relationship::FollowsT,
     {RelRefFormat{RefType::StmtRef, RefType::StmtRef}}},
    {PQLToken::Relationship::Parent,
     {RelRefFormat{RefType::StmtRef, RefType::StmtRef}}},
    {PQLToken::Relationship::ParentT,
     {RelRefFormat{RefType::StmtRef, RefType::StmtRef}}},
    {PQLToken::Relationship::Calls,
     {RelRefFormat{RefType::EntRef, RefType::EntRef}}},
    {PQLToken::Relationship::CallsT,
     {RelRefFormat{RefType::EntRef, RefType::EntRef}}},
    {PQLToken::Relationship::Next,
     {RelRefFormat{RefType::StmtRef, RefType::StmtRef}}},
    {PQLToken::Relationship::NextT,
     {RelRefFormat{RefType::StmtRef, RefType::StmtRef}}},
    {PQLToken::Relationship::Affects,
     {RelRefFormat{RefType::StmtRef, RefType::StmtRef}}},
    {PQLToken::Relationship::AffectsT,
     {RelRefFormat{RefType::StmtRef, RefType::StmtRef}}},
    {PQLToken::Relationship::Modifies,
     {RelRefFormat{RefType::StmtRef, RefType::EntRef},
      RelRefFormat{RefType::EntRef, RefType::EntRef}}},
    {PQLToken::Relationship::Uses,
     {RelRefFormat{RefType::StmtRef, RefType::EntRef},
      RelRefFormat{RefType::EntRef, RefType::EntRef}}},
    {PQLToken::Relationship::Pattern,
     {
         RelRefFormat{RefType::EntRef, RefType::ExprSpec},
     }},
    {PQLToken::Relationship::With,
     {
         RelRefFormat{RefType::WithRef, RefType::WithRef},
     }},
};

const std::map<Token, RelRefType> tokenToEnumRRMap = {
    {PQLToken::Relationship::Follows, RelRefType::Follows},
    {PQLToken::Relationship::FollowsT, RelRefType::FollowsT},
    {PQLToken::Relationship::Modifies, RelRefType::Modifies},
    {PQLToken::Relationship::Parent, RelRefType::Parent},
    {PQLToken::Relationship::ParentT, RelRefType::ParentT},
    {PQLToken::Relationship::Uses, RelRefType::Uses},
    {PQLToken::Relationship::Calls, RelRefType::Calls},
    {PQLToken::Relationship::CallsT, RelRefType::CallsT},
    {PQLToken::Relationship::Next, RelRefType::Next},
    {PQLToken::Relationship::NextT, RelRefType::NextT},
    {PQLToken::Relationship::Affects, RelRefType::Affects},
    {PQLToken::Relationship::AffectsT, RelRefType::AffectsT},
};

const std::map<Token, AttrName> tokenToAttrNameMap = {
    {PQLToken::AttrName::ProcName, AttrName::ProcName},
    {PQLToken::AttrName::VarName, AttrName::VarName},
    {PQLToken::AttrName::Value, AttrName::Value},
    {PQLToken::AttrName::StmtNo, AttrName::StmtNo},
};

// Used to delimit when a clause ends
const std::set<Token> clauseKeywords = {
    PQLToken::Clause::And, PQLToken::Clause::Pattern, PQLToken::Clause::Such,
    PQLToken::Clause::With};
