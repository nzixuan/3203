#include "stdafx.h"

#include <algorithm>

#include "CppUnitTest.h"
#include "QueryEvaluator.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(EvalModifiesPTest) {
  struct EvalModifiesPStub : PKB {
    std::list<PKBEntity> getProcList() override {
      return {{"A", EntityType::Procedure}, {"B", EntityType::Procedure}};
    }

    bool hasLhsConstantRhsConstantForRel(QueryConstant lhsConstant,
                                         QueryConstant rhsConstant,
                                         RelRefType relRefType) override {
      return lhsConstant.type == QueryConstantType::Identifier &&
             rhsConstant.type == QueryConstantType::Identifier &&
             lhsConstant.val == "main" && rhsConstant.val == "x" &&
             relRefType == RelRefType::Modifies;
    }

    std::list<PKBEntity> getLhsSynRhsConstantForRel(
        EntityType lhsType, QueryConstant rhsConstant,
        RelRefType relRefType) override {
      if (lhsType == EntityType::Procedure &&
          rhsConstant.type == QueryConstantType::Identifier &&
          rhsConstant.val == "A" && relRefType == RelRefType::Modifies) {
        return {{"main", EntityType::Procedure}};
      }
      return {};
    }

    std::list<PKBEntity> getLhsSynRhsWildcardForRel(
        EntityType lhsType, RelRefType relRefType) override {
      if (lhsType == EntityType::Procedure &&
          relRefType == RelRefType::Modifies) {
        return {{"main", EntityType::Procedure}, {"B", EntityType::Procedure}};
      }
      return {};
    }
  };

  PKB* pkb = new EvalModifiesPStub();

 public:
  TEST_METHOD(TestConstConst) {
    // procedure p; Select p such that Modifies("main", "x");
    ClauseArgs ca;
    ca.addRelArg(SourceArg{"main"});
    ca.addRelArg(SourceArg{"x"});
    SuchThat s = {ca, RelRefType::Modifies};

    Query q;
    q.getClauseManager()->addClause(s);
    q.addSelectArg(procSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "B") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "A") != lst.end());
    Assert::IsTrue(lst.size() == 2);
  }
  TEST_METHOD(TestSynConst) {
    // procedure p; Select p such that Modifies(p, "A")
    ClauseArgs ca;
    ca.addRelArg(procSyn);
    ca.addRelArg(SourceArg{"A"});
    SuchThat s = {ca, RelRefType::Modifies};

    Query q;
    q.getClauseManager()->addClause(s);
    q.addSelectArg(procSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "main") != lst.end());
    Assert::IsTrue(lst.size() == 1);
  }
  TEST_METHOD(TestSynConstBool) {
    // procedure p; Select BOOLEAN such that Modifies(p, "A")
    ClauseArgs ca;
    ca.addRelArg(procSyn);
    ca.addRelArg(SourceArg{"A"});
    SuchThat s = {ca, RelRefType::Modifies};

    Query q;
    q.getClauseManager()->addClause(s);
    q.setBooleanSelectFlag();
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(lst.size() == 1);
    Assert::IsTrue(lst.front() == "TRUE");
  }
  TEST_METHOD(TestSynWildcard) {
    // procedure p; Select p such that Modifies(p, _)
    ClauseArgs ca;
    ca.addRelArg(procSyn);
    ca.addRelArg(Wildcard{});
    SuchThat s = {ca, RelRefType::Modifies};

    Query q;
    q.getClauseManager()->addClause(s);
    q.addSelectArg(procSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "main") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "B") != lst.end());
    Assert::IsTrue(lst.size() == 2);
  }
};
}  // namespace UnitTesting
