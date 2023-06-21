#include "stdafx.h"

#include <algorithm>

#include "CppUnitTest.h"
#include "QueryEvaluator.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(EvalCallsTest) {
  struct EvalCallsStub : PKB {
    std::list<PKBEntity> getProcList() override {
      return {{"A", EntityType::Procedure}, {"B", EntityType::Procedure}};
    }

    bool hasLhsConstantRhsConstantForRel(QueryConstant lhsConstant,
                                         QueryConstant rhsConstant,
                                         RelRefType relRefType) override {
      return lhsConstant.val == "main" &&
             lhsConstant.type == QueryConstantType::Identifier &&
             rhsConstant.val == "B" &&
             rhsConstant.type == QueryConstantType::Identifier &&
             relRefType == RelRefType::Calls;
    }

    std::list<PKBEntity> getLhsSynRhsConstantForRel(
        EntityType lhsType, QueryConstant rhsConstant,
        RelRefType relRefType) override {
      if (lhsType == EntityType::Procedure && rhsConstant.val == "B" &&
          rhsConstant.type == QueryConstantType::Identifier &&
          relRefType == RelRefType::Calls) {
        return {{"main", EntityType::Procedure},
                {"proc1", EntityType::Procedure}};
      }
      return {};
    }
  };

  PKB* pkb = new EvalCallsStub();

 public:
  TEST_METHOD(TestConstConst) {
    // procedure p; Select p such that Calls("main", "B")
    ClauseArgs ca;
    ca.addRelArg(SourceArg{"main"});
    ca.addRelArg(SourceArg{"B"});
    SuchThat s{ca, RelRefType::Calls};

    Query q;
    q.getClauseManager()->addClause(s);
    q.addSelectArg(procSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "A") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "B") != lst.end());
    Assert::IsTrue(lst.size() == 2);
  }
  TEST_METHOD(TestSynConst) {
    // procedure p; Select p such that Calls(p, "B")
    ClauseArgs ca;
    ca.addRelArg(procSyn);
    ca.addRelArg(SourceArg{"B"});
    SuchThat s{ca, RelRefType::Calls};

    Query q;
    q.getClauseManager()->addClause(s);
    q.finalize();
    q.addSelectArg(procSyn);

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "main") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "proc1") != lst.end());
    Assert::IsTrue(lst.size() == 2);
  }
};
}  // namespace UnitTesting
