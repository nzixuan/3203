#include "stdafx.h"

#include <algorithm>

#include "CppUnitTest.h"
#include "QueryEvaluator.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(EvalPatternIfWhileTest) {
  struct EvalPatternIfWhileStub : PKB {
    std::list<PKBEntityPair> getPatternSyn(EntityType type) override {
      if (type == EntityType::If) {
        return {
            {{"1023", EntityType::If}, {"v1", EntityType::Variable}},
            {{"35", EntityType::If}, {"abc34", EntityType::Variable}},
            {{"14", EntityType::If}, {"v1", EntityType::Variable}},
            {{"304", EntityType::If}, {"variable1234", EntityType::Variable}}};
      }
      return {};
    }

    std::list<PKBEntity> getPatternWildcard(EntityType type) override {
      if (type == EntityType::If) {
        return {{"23", EntityType::If},
                {"38", EntityType::If},
                {"94", EntityType::If}};
      }
      return {};
    }

    std::list<PKBEntity> getPatternConst(std::string constVal,
                                         EntityType type) override {
      if (constVal == "var123" && type == EntityType::While) {
        return {{"13", EntityType::If}, {"9", EntityType::If}};
      }
      return {};
    }
  };

  PKB* pkb = new EvalPatternIfWhileStub();

  TEST_METHOD(TestIfSyn) {
    // if ifs; variable v; Select <ifs, v> pattern ifs (v, _, _)
    ClauseArgs ca;
    ca.addRelArg(varSyn);
    ca.addRelArg(wildcard);
    ca.addRelArg(wildcard);
    Pattern p{ca, ifSyn};

    Query q;
    q.getClauseManager()->addClause(p);
    q.addSelectArg(ifSyn);
    q.addSelectArg(varSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "14 v1") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "304 variable1234") !=
                   lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "35 abc34") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "1023 v1") != lst.end());
    Assert::IsTrue(lst.size() == 4);
  }
  TEST_METHOD(TestIfWildcard) {
    // if ifs; Select ifs pattern ifs (_, _, _)
    ClauseArgs ca;
    ca.addRelArg(wildcard);
    ca.addRelArg(wildcard);
    ca.addRelArg(wildcard);
    Pattern p{ca, ifSyn};

    Query q;
    q.getClauseManager()->addClause(p);
    q.addSelectArg(ifSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "23") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "38") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "94") != lst.end());
    Assert::IsTrue(lst.size() == 3);
  }
  TEST_METHOD(TestWhileConst) {
    // while w; Select w pattern w ("var123", _)
    ClauseArgs ca;
    ca.addRelArg(SourceArg{"var123"});
    ca.addRelArg(wildcard);
    Pattern p{ca, whileSyn};

    Query q;
    q.getClauseManager()->addClause(p);
    q.addSelectArg(whileSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "13") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "9") != lst.end());
    Assert::IsTrue(lst.size() == 2);
  }
};
}  // namespace UnitTesting
